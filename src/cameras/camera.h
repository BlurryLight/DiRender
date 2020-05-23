#pragma once

#include <math/geometry.hpp>
#include <math/transform.h>
#include <memory>
#include <string>
NAMESPACE_BEGIN(DR)
struct Film {
  Film(int w, int h) : height(h), width(w) {
    framebuffer_.resize(height * width);
  }
  Film() : Film(400, 400) {}
  std::vector<Vector3f> framebuffer_;
  int height = 400;
  int width = 400;
  int tile_height = 4;
  int tile_width = 4;
  int tile_height_nums = std::ceil(height / tile_height);
  int tile_width_nums = std::ceil(width / tile_width);
  void write_ppm(const std::string &filename);
};

class Camera {
public:
  static Transform look_at(Point3f origin, Vector3f WorldUp, Vector3f target);
  virtual Transform look_at() const final { return *view_trans_; }
  Camera(Point3f origin, Vector3f WorldUp, Vector3f target, float fov,
         uint height, uint width);
  virtual ~Camera(){};
  virtual Ray get_ray(float u, float v) const = 0;
  std::unique_ptr<Film> film_ptr_;

protected:
  std::shared_ptr<Transform> view_trans_;
  std::shared_ptr<Transform> view_trans_inverse_;
};

inline void Film::write_ppm(const std::string &filename) {
  auto fp = std::unique_ptr<FILE, decltype(&fclose)>(
      fopen(filename.c_str(), "wb"), &fclose);
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

inline Transform Camera::look_at(Point3f origin, Vector3f WorldUp,
                                 Vector3f target) {
  auto cam_origin = static_cast<Vector3f>(origin);
  auto cam_target = (static_cast<Vector3f>(origin) - target).normalize();
  auto cam_right = cross(WorldUp.normalize(), cam_target).normalize();
  auto cam_up = cross(cam_target, cam_right).normalize();

  Matrix4 part2 = Matrix4(1.0f);
  part2.m[0][3] = -cam_origin[0];
  part2.m[1][3] = -cam_origin[1];
  part2.m[2][3] = -cam_origin[2];

  Matrix4 part1 = Matrix4(1.0f);
  part1.m[0][0] = cam_right[0];
  part1.m[0][1] = cam_right[1];
  part1.m[0][2] = cam_right[2];

  part1.m[1][0] = cam_up[0];
  part1.m[1][1] = cam_up[1];
  part1.m[1][2] = cam_up[2];

  part1.m[2][0] = cam_target[0];
  part1.m[2][1] = cam_target[1];
  part1.m[2][2] = cam_target[2];

  return Transform(part1 * part2);
}

inline Camera::Camera(Point3f origin, Vector3f WorldUp, Vector3f target,
                      float fov, uint height, uint width) {
  ignore(fov);
  auto view_trans = Camera::look_at(origin, WorldUp, target);
  this->view_trans_ = std::make_shared<Transform>(view_trans);
  this->view_trans_inverse_ =
      std::make_shared<Transform>(Transform::Inverse(view_trans));
  Transform::TransformTable.insert({*view_trans_, this->view_trans_});
  Transform::TransformTable.insert(
      {*view_trans_inverse_, this->view_trans_inverse_});
  film_ptr_ = std::make_unique<Film>(width, height);
}

NAMESPACE_END(DR)
