#pragma once

#include <math/geometry.hpp>
#include <memory>
NAMESPACE_BEGIN(DR)
struct film {
  film(int w, int h) : height(h), width(w) {}
  std::vector<Vector3f> framebuffer_;
  int height = 400;
  int width = 400;
  int tile_height = 4;
  int tile_width = 4;
  int tile_height_pixels = std::ceil(height / tile_height);
  int tile_width_pixels = std::ceil(width / tile_width);
  void write_ppm() {
    auto fp = std::unique_ptr<FILE, decltype(&fclose)>(
        fopen("output.ppm", "wb"), &fclose);
    (void)fprintf(fp.get(), "P6\n%d %d\n255\n", height, width);
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
      fwrite(color, 1, 3, fp.get());
    }
  }
};
NAMESPACE_END(DR)
