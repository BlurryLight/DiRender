#include <accelerator/linear_list.h>
#include <cameras/pinhole_camera.h>
#include <cores/primitive.h>
#include <cores/ray.hpp>
#include <material/matte_material.h>
#include <renderer/path_tracing_renderer.h>
#include <shapes/sphere.h>
#include <tuple> //for std::tie
using namespace DR;
Vector3f PathTracingRenderer::cast_ray(
    const Ray &r, std::shared_ptr<Primitive> prim,
    const std::vector<std::shared_ptr<Primitive>> &lights, int depth = 0) {
  float russian_roulette = 0.8f;
  if (depth > 5 || (depth != 0 && get_random_float() > russian_roulette))
    return {0};

  Intersection isect;
  if (!prim->Intersect(r, &isect))
    return {0};

  // Shadow ray
  Intersection light_pos;
  float light_pdf = 0;
  int size = lights.size();
  if (size == 0) {
    throw std::runtime_error("PathTracingRenderer needs at least one light!");
  }

  int index = std::floor(size * get_random_float(0.0, 0.99));
  // we hope the sample is visible from the isect.coords
  // if return {pos,0.0}, the sample is not visible.
  std::tie(light_pos, light_pdf) = lights.at(index)->sample(isect.coords);
  Vector3f L_in = {0};
  Vector3f new_direction;
  float pdf;
  if (light_pdf == 0.0f ||
      get_random_float() <
          0.5) { // if not visible, we simply let the ray scatter
    std::tie(new_direction, pdf) =
        isect.mat_ptr->sampleScatter(r.direction_, isect);
  } else {
    float emit_area = 0;
    for (const auto &i : lights) {
      emit_area += i->Area();
    }
    light_pdf = 1 / emit_area;
    new_direction = (light_pos.coords - isect.coords).normalize();
    float distance2 = (light_pos.coords - isect.coords).squared_length();
    float cosine = std::fabs(dot(-new_direction, light_pos.normal));
    if (cosine < 0.01f) {
      return isect.mat_ptr->evalEmitted(r.direction_, isect);
    }
    pdf = distance2 * (1 / cosine) * (light_pdf);
  }
  Ray r_new(isect.coords, new_direction);
  auto brdf = isect.mat_ptr->evalBxDF(r.direction_, isect, r_new.direction_);
  L_in = isect.mat_ptr->evalEmitted(r.direction_, isect);

  auto part1 = cast_ray(r_new, prim, lights, depth + 1);
  auto part2 = multiply(brdf, part1);
  L_in += part2 * std::max(dot(r_new.direction_, isect.normal), 0.0f) /
          (pdf * russian_roulette + kEpsilon); // avoid zero
  return L_in;
}
void PathTracingRenderer::render_tile(
    std::shared_ptr<Camera> cam, std::shared_ptr<Primitive> prim,
    const std::vector<std::shared_ptr<Primitive>> &lights, int height,
    int width, int blockheight, int blockwidth, int blockheightId,
    int blockwidthId, int spp) {
  UpdateProgrss(cam->film_ptr_->tile_width_nums *
                cam->film_ptr_->tile_height_nums);
  for (int i = 0; i < blockheight; i++) {
    for (int j = 0; j < blockwidth; j++) {
      int trueJ = blockwidth * blockwidthId + j;
      int trueI = blockheight * blockheightId + i;
      if (trueJ >= width || trueI >= height)
        continue;
      for (int k = 0; k < spp; k++) {
        float u = float(trueJ + get_random_float()) / (width);
        float v = float(height - 1 - trueI + get_random_float()) / (height);
        auto r = cam->get_ray(u, v);
        auto tmp = cast_ray(r, prim, lights, 0) / spp;
        cam->film_ptr_->framebuffer_.at(trueI * width + trueJ) += tmp;
      }
    }
  }
}
//#define NDEBUG
void PathTracingRenderer::render(const Scene &scene) {

  std::shared_ptr<Primitive> hit_list = nullptr;
  if (scene.prims_.size() > 0) {
    if (scene.prims_.size() > 1) {
      std::vector<std::shared_ptr<Primitive>> prims;
      for (auto &i : scene.prims_)
        prims.push_back(i);
      hit_list = std::make_shared<LinearList>(prims);
    } else {
      hit_list = scene.prims_.at(0); // only one object : maybe a bvh tree
    }
  } else {
    throw std::runtime_error("Scene has no primitive to render!");
    exit(EXIT_FAILURE);
  }

  int index = 0;
  for (const auto &cam : scene.cams_) {
    std::vector<std::future<void>> futures;
    futures.reserve(cam->film_ptr_->tile_width_nums *
                    cam->film_ptr_->tile_height_nums);
    for (uint i = 0; i < cam->film_ptr_->tile_height_nums; i++) {
      for (uint j = 0; j < cam->film_ptr_->tile_width_nums; j++) {
        // parallel
#ifdef NDEBUG
        futures.push_back(this->pool_.enqueue_task(
            // Interesting syntax here
            &PathTracingRenderer::render_tile, this, cam, hit_list,
            scene.light_shapes_, cam->film_ptr_->height, cam->film_ptr_->width,
            cam->film_ptr_->tile_height, cam->film_ptr_->tile_width, i, j,
            spp_));
#else
        // single thread
        render_tile(cam, hit_list, scene.light_shapes_, cam->film_ptr_->height,
                    cam->film_ptr_->width, cam->film_ptr_->tile_height,
                    cam->film_ptr_->tile_width, i, j, spp_);
#endif
      }
    }
#ifdef NDEBUG
    size_t index_test = 0;
    try {
      for (index_test = 0; index_test < futures.size(); index_test++) {
        auto &&result = futures.at(index_test);
        result.get();
      }
    } catch (std::exception &e) {
      std::cout << e.what() << std::endl;
      std::cout << "size:" << futures.size() << std::endl;
      std::cout << "Index test:" << index_test << std::endl;
    }
#endif
    cam->film_ptr_->write("output_" + std::to_string(index++) +
                              std::string(MapTypeToSuffix()(PicType::kJPG)),
                          PicType::kJPG);
    //    cam->film_ptr_->write_ppm("output" + std::to_string(index++) +
    //    ".ppm");
  }
}
