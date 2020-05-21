#include <cameras/pinhole_camera.h>
#include <cores/primitive.h>
#include <cores/ray.hpp>
#include <cores/render.h>
#include <shapes/sphere.h>
using namespace DR;
void Render::render() {
  int height = 400;
  int width = 400;

  Point3f origin{1.0f, 1.0f, 0.0f};
  auto mat4 = Matrix4();
  mat4.m[1][3] = 0;
  mat4.m[0][3] = 0;
  mat4.m[2][3] = -2;
  auto trans = std::make_shared<Transform>(mat4);
  auto trans_inv = std::make_shared<Transform>(Transform::Inverse(*trans));
  auto sphere = std::make_shared<Sphere>(trans, trans_inv, false, 1);

  auto cast_ray = [&sphere](const Ray &r) {
    Vector3f unit_vec = r.direction_.normalize();
    auto t = 0.5f * (unit_vec.y + 1.0f);
    if (sphere->Intersect(r)) {
      return Vector3f{1.0, 0.0, 0.0};
    }
    return (1.0 - t) * vec3(1.0f) + t * vec3(0.5, 0.7, 1.0);
  };

  PinholeCamera cam(origin, {0.0, 1.0, 0.0}, {0.0, 0.0, -1.0}, 90.0, 400, 400);
  int spp = 32;

  auto render_tile = [&](int height, int width, int blockheight, int blockwidth,
                         int blockheightId, int blockwidthId, int spp) {
    for (int i = 0; i < blockheight; i++) {
      for (int j = 0; j < blockwidth; j++) {
        int trueJ = blockwidth * blockwidthId + j;
        int trueI = blockheight * blockheightId + i;
        if (trueJ >= width || trueI >= height)
          return;
        for (int k = 0; k < spp; k++) {
          float u = float(trueJ + get_random_float()) / (width - 1);
          float v =
              float(height - 1 - trueI + get_random_float()) / (height - 1);
          auto r = cam.get_ray(u, v);
          cam.film_ptr_->framebuffer_.at(trueI * width + trueJ) +=
              cast_ray(r) / spp;
        }
      }
    }
  };

  std::vector<std::future<void>> futures;
  for (int i = 0; i < cam.film_ptr_->tile_height; i++) {
    for (int j = 0; j < cam.film_ptr_->tile_width; j++) {
      // parallel
      futures.emplace_back(this->pool_.enqueue_task(
          render_tile, height, width, cam.film_ptr_->tile_height_pixels,
          cam.film_ptr_->tile_width_pixels, i, j, spp));
      // single thread
      //      render_tile(height, width, cam.film_ptr_->tile_height_pixels,
      //                  cam.film_ptr_->tile_width_pixels, i, j);
    }
  }
  for (auto &i : futures)
    i.wait();
  cam.film_ptr_->write_ppm("output.ppm");
}
