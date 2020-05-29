#include <accelerator/linear_list.h>
#include <cameras/pinhole_camera.h>
#include <cores/primitive.h>
#include <cores/ray.hpp>
#include <cores/render.h>
#include <material/matte_material.h>
#include <shapes/sphere.h>
#include <tuple> //for std::tie
using namespace DR;

static Vector3f cast_ray(const Ray &r, std::shared_ptr<Primitive> prim,
                         int depth = 0) {
  float russian_roulette = 0.8f;
  if (!prim->Intersect_test(r)) {
    Vector3f unit_vec = r.direction_.normalize();
    auto t = 0.5f * (unit_vec.y + 1.0f);
    return (1.0 - t) * vec3(1.0f) + t * vec3(0.5, 0.7, 1.0);
  }
  if (depth > 4)
    return {};
  Intersection isect;
  if (get_random_float() < russian_roulette && prim->Intersect(r, &isect)) {
    Vector3f new_direction;
    float pdf;
    std::tie(new_direction, pdf) =
        isect.mat_ptr->sampleScatter(r.direction_, isect);
    Ray r_new(isect.coords, new_direction);
    auto brdf = isect.mat_ptr->evalBxDF(r.direction_, isect, r_new.direction_);
    return isect.mat_ptr->evalEmitted(r.direction_, isect) +
           multiply(brdf, cast_ray(r_new, prim, depth + 1)) *
               dot(r_new.direction_, isect.normal) / (pdf * russian_roulette);
  }
  return {};
}
static int count = 0;
static std::mutex mutex;
static void UpdateProgrss(int num_of_tiles) {
  std::unique_lock<std::mutex> lk(mutex);
  count++;
  std::cout << "Tiles Complete:" << count << " Total: " << num_of_tiles << " "
            << (float)count / num_of_tiles * 100.0 << "%" << '\r';
  fflush(stdout);
}

static void render_tile(std::shared_ptr<Camera> cam,
                        std::shared_ptr<Primitive> prim, int height, int width,
                        int blockheight, int blockwidth, int blockheightId,
                        int blockwidthId, int spp) {
  UpdateProgrss(cam->film_ptr_->tile_width_nums *
                cam->film_ptr_->tile_height_nums);
  for (int i = 0; i < blockheight; i++) {
    for (int j = 0; j < blockwidth; j++) {
      int trueJ = blockwidth * blockwidthId + j;
      int trueI = blockheight * blockheightId + i;
      if (trueJ >= width || trueI >= height)
        return;
      for (int k = 0; k < spp; k++) {
        float u = float(trueJ + get_random_float()) / (width - 1);
        float v = float(height - 1 - trueI + get_random_float()) / (height - 1);
        auto r = cam->get_ray(u, v);
        cam->film_ptr_->framebuffer_.at(trueI * width + trueJ) +=
            cast_ray(r, prim, 0) / spp;
      }
    }
  }
}

void Render::render(const Scene &scene) {

  std::shared_ptr<Primitive> hit_list = nullptr;
  if (scene.Prims_.size() > 0) {
    if (scene.Prims_.size() > 1) {
      std::vector<std::shared_ptr<Primitive>> prims;
      for (auto &i : scene.Prims_)
        prims.push_back(i);
      hit_list = std::make_shared<LinearList>(prims);
    } else {
      hit_list = scene.Prims_.at(0); // only one object : maybe a bvh tree
    }
  } else {
    throw std::runtime_error("Scene has no primitive to render!");
    exit(EXIT_FAILURE);
  }

  int index = 0;
  for (const auto &cam : scene.cams_) {
    std::vector<std::future<void>> futures;
    for (uint i = 0; i < cam->film_ptr_->tile_height_nums; i++) {
      for (uint j = 0; j < cam->film_ptr_->tile_width_nums; j++) {
        // parallel
        futures.emplace_back(this->pool_.enqueue_task(
            render_tile, cam, hit_list, cam->film_ptr_->height,
            cam->film_ptr_->width, cam->film_ptr_->tile_height,
            cam->film_ptr_->tile_width, i, j, spp_));
        // single thread
        //        render_tile(cam, hit_list, cam->film_ptr_->height,
        //                    cam->film_ptr_->width,
        //                    cam->film_ptr_->tile_height_pixels,
        //                    cam->film_ptr_->tile_width_pixels, i, j, spp_);
      }
    }
    for (auto &i : futures)
      i.wait();
    cam->film_ptr_->write("output_" + std::to_string(index++) +
                              std::string(MapTypeToSuffix()(PicType::kPNG)),
                          PicType::kPNG);
  }
}
