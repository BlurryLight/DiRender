#include <cores/primitive.h>
#include <cores/ray.hpp>
#include <cores/render.h>
#include <shapes/sphere.h>
using namespace DR;
void Render::render() {
  int height = 400;
  int width = 400;
  this->framebuffer_.resize(height * width);

  Point3f origin{0.0f, 0.0f, 0.0f};
  Point3f low_left{-2.0f, -2.0f, -1.0f};
  Vector3f horizontal{4.0f, 0.0f, 0.0f};
  Vector3f vertical{0.0f, 4.0f, 0.0f};

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

  //  int tile_num = 16 ;
  int tile_height = 4;
  int tile_width = 4;

  int tile_height_pixels = std::ceil(height / tile_height);
  int tile_width_pixels = std::ceil(width / tile_width);

  auto render_tile = [&](int height, int width, int blockheight, int blockwidth,
                         int blockheightId, int blockwidthId) {
    for (int i = 0; i < blockheight; i++) {
      for (int j = 0; j < blockwidth; j++) {
        int trueJ = blockwidth * blockwidthId + j;
        int trueI = blockheight * blockheightId + i;
        if (trueJ >= width || trueI >= height)
          return;
        float u = float(trueJ) / (width - 1);
        float v = float(height - 1 - trueI) / (height - 1);
        Ray r(origin, (low_left + u * horizontal + v * vertical) - origin);
        framebuffer_[trueI * width + trueJ] = cast_ray(r);
      }
    }
  };

  for (int i = 0; i < tile_height; i++) {
    for (int j = 0; j < tile_width; j++) {
      // parallel
      //      this->pool_.enqueue_task(render_tile, height, width,
      //      tile_height_pixels,
      //                               tile_width_pixels, i, j);
      // single thread
      render_tile(height, width, tile_height_pixels, tile_width_pixels, i, j);
    }
  }
  FILE *fp = fopen("binary.ppm", "wb");
  (void)fprintf(fp, "P6\n%d %d\n255\n", height, width);
  for (auto i = 0; i < width * height; ++i) {
    static unsigned char color[3];
    color[0] =
        (unsigned char)(255 *
                        std::pow(clamp(0.0f, 1.0f, framebuffer_[i].x), 0.6f));
    color[1] =
        (unsigned char)(255 *
                        std::pow(clamp(0.0f, 1.0f, framebuffer_[i].y), 0.6f));
    color[2] =
        (unsigned char)(255 *
                        std::pow(clamp(0.0f, 1.0f, framebuffer_[i].z), 0.6f));
    fwrite(color, 1, 3, fp);
  }
  fclose(fp);
}
