#include <accelerator/linear_list.h>
#include <cameras/pinhole_camera.h>
#include <cores/primitive.h>
#include <cores/ray.hpp>
#include <material/matte_material.h>
#include <renderer/path_tracing_renderer.h>
#include <shapes/sphere.h>
#include <tuple> //for std::tie

#ifndef NDEBUG
// log
// https://github.com/gabime/spdlog/wiki/0.-FAQ#winapi-minmax-macro-definitions
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <spdlog/spdlog.h>
#endif
using namespace DR;
Vector3f PathTracingRenderer::cast_ray(
    const Ray &r, std::shared_ptr<Primitive> prim,
    const std::vector<std::shared_ptr<Primitive>> &lights, int depth,
    float mis_weight, bool includeLe) {

  Intersection isect;
  if (!prim->Intersect(r, &isect))
    return background_;

#ifndef NDEBUG
  // ray.origin; isect.coords; isect.normal
  static auto ray_logger = spdlog::get("ray_logger");
  ray_logger->info(
      "{:.4f},{:.4f},{:.4f};{:.4f},{:.4f},{:.4f};{:.4f},{:.4f},{:.4f}",
      r.origin_.x, r.origin_.y, r.origin_.z, isect.coords.x, isect.coords.y,
      isect.coords.z, isect.normal.x, isect.normal.y, isect.normal.z);
#endif
  auto emission = isect.mat_ptr->evalEmitted(r.direction_, isect);
  Vector3f Le{};
  Le = includeLe ? mis_weight * emission : Le;

  static float russian_roulette = 0.8f;
  if (depth > 5 || (depth > 0 && get_random_float() > russian_roulette))
    return Le;

  // be more defensive
  // ray's direction is towards the surface

  //  assert(r.direction_.dot(isect.normal) <= 0);
  // Shadow ray
  Intersection light_pos;
  float light_pdf = 0;
  int size = lights.size();
  if (size == 0) {
    throw std::runtime_error("PathTracingRenderer needs at least one light!");
  }
  int index = std::floor(size * get_random_float(0.0, 0.99));
  std::tie(light_pos, light_pdf) = lights.at(index)->sample();

  // multi importance sampling
  float emit_area = 0.0f;
  for (const auto &i : lights) {
    emit_area += i->Area();
  }
  light_pdf = 1 / emit_area;

  auto [scatter_direction, scatter_pdf] =
      isect.mat_ptr->sampleScatter(r.direction_, isect);

  float pdf_sum = scatter_pdf;
  float shadow_mis_weight = 0.0f;
  float brdf_mis_weight = 1.0f;

  Vector3f L_in = {0};
  Vector3f L_shadowray = {0};
  if (!isect.mat_ptr->specular) {
    Vector3f shadowray_direction =
        (light_pos.coords - isect.coords).normalize();
    Ray shadow_ray(isect.coords, shadowray_direction);
    float distance2 = (light_pos.coords - isect.coords).squared_length();
    float cosine2 = std::max(dot(-shadowray_direction, light_pos.normal), 0.0f);

    float true_light_pdf = light_pdf * distance2 / cosine2;
    pdf_sum += true_light_pdf;
    shadow_mis_weight = true_light_pdf / pdf_sum;
    auto L_shadow =
        cast_ray(shadow_ray, prim, lights, kIntMax, shadow_mis_weight, true);
    if (L_shadow.squared_length() > 0.0f) {
      float cosine1 = std::max(dot(shadowray_direction, isect.normal), 0.0f);
      auto brdf =
          isect.mat_ptr->evalBxDF(r.direction_, isect, shadowray_direction);
      L_shadowray = multiply(brdf, L_shadow) * cosine1 / true_light_pdf;
    }
  }

  // now we have both pdf(brdf sampling and light sampling)

  brdf_mis_weight = scatter_pdf / pdf_sum;
  Ray r_new(isect.coords, scatter_direction);

  //  assert(scatter_direction.dot(isect.normal) >= 0);
  auto brdf = isect.mat_ptr->evalBxDF(r.direction_, isect, r_new.direction_);

  auto part1 = cast_ray(r_new, prim, lights, depth + 1, brdf_mis_weight,
                        false); // doesn't allow hit the light source
  auto part2 = multiply(brdf, part1);
  // it cannot be std::max(dot(r_new.direction_,isct.normal),0.0f)
  // because of the dielctric refraction ray will be negative
  L_in = part2 * std::abs(dot(r_new.direction_, isect.normal)) /
         (scatter_pdf * russian_roulette + kEpsilon); // avoid zero
  return Le + L_in + L_shadowray;
}
void PathTracingRenderer::render_tile(
    std::shared_ptr<Camera> cam, std::shared_ptr<Primitive> prim,
    const std::vector<std::shared_ptr<Primitive>> &lights, int height,
    int width, int blockheight, int blockwidth, int blockheightId,
    int blockwidthId, int sppstep) {
  UpdateProgrss(cam->film_ptr_->tile_width_nums *
                cam->film_ptr_->tile_height_nums);
  for (int i = 0; i < blockheight; i++) {
    for (int j = 0; j < blockwidth; j++) {
      int trueJ = blockwidth * blockwidthId + j;
      int trueI = blockheight * blockheightId + i;
      if (trueJ >= width || trueI >= height)
        continue;
      for (int k = 0; k < sppstep; k++) {
        auto [x, y] = cam->sampler_->get_2d();
        float u = float(trueJ + x) / (width);
        float v = float(height - 1 - trueI + y) / (height);
        auto r = cam->get_ray(u, v);
        auto tmp = cast_ray(r, prim, lights, 0, 1, true);
        cam->film_ptr_->framebuffer_.at(trueI * width + trueJ) += tmp;
      }
    }
  }
}
//#define NDEBUG
void PathTracingRenderer::render(const Scene &scene) {
  scene_ = &scene;
  std::shared_ptr<Primitive> hit_list = nullptr;
  if (!scene.prims_.empty()) {
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
  }
  // set background
  background_ = scene.background_;
  //  std::cout << "background:"<< background_<<std::endl;

  int index = 0;
  int cam_index = 0;
  for (const auto &cam : scene.cams_) {

#ifdef NDEBUG
    std::vector<std::future<void>> futures;
    futures.reserve(cam->film_ptr_->tile_width_nums *
                    cam->film_ptr_->tile_height_nums);
#endif
    // progressive rendering
    // save an image per 16 spp
    uint step = 16;
    uint n = spp_ / 16 + 1;
    for (uint s = 0; s < n; s++) {
      std::cout << "Current Step:(" << s << "/" << n << ")" << std::endl;
      for (uint i = 0; i < cam->film_ptr_->tile_height_nums; i++) {
        for (uint j = 0; j < cam->film_ptr_->tile_width_nums; j++) {
          // parallel
#ifdef NDEBUG
          futures.push_back(this->pool_.enqueue_task(
              // Interesting syntax here
              &PathTracingRenderer::render_tile, this, cam, hit_list,
              scene.light_shapes_, cam->film_ptr_->height,
              cam->film_ptr_->width, cam->film_ptr_->tile_height,
              cam->film_ptr_->tile_width, i, j, step));
#else
          // single thread
          render_tile(cam, hit_list, scene.light_shapes_,
                      cam->film_ptr_->height, cam->film_ptr_->width,
                      cam->film_ptr_->tile_height, cam->film_ptr_->tile_width,
                      i, j, spp_);
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
      std::vector<std::future<void>>{}.swap(futures);
      {
        std::lock_guard<std::mutex> gd(mutex_);
        count_ = 0;
      }
#endif
      cam->film_ptr_->write("cam" + std::to_string(cam_index) + "_output_" +
                                std::to_string(index++) +
                                std::string(MapTypeToSuffix()(PicType::kPNG)),
                            PicType::kPNG, step * (s + 1));
      //    cam->film_ptr_->write_ppm("output" + std::to_string(index++) +
      //    ".ppm");
    }
  }
  cam_index++;
}
