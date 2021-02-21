#pragma once

#include <math/geometry.hpp>
#include <math/transform.h>
#include <memory>
#include <string>
#include <utils/stb_image_wrapper.h>
NAMESPACE_BEGIN(DR)
struct Film {
  Film(uint w, uint h) : height(h), width(w) {
    framebuffer_.resize(height * width);
  }
  Film() : Film(400, 400) {}
  std::vector<Vector3f> framebuffer_;
  uint height = 400;
  uint width = 400;
  uint tile_height = 16;
  uint tile_width = 16;
  uint tile_height_nums = std::ceil(height / tile_height);
  uint tile_width_nums = std::ceil(width / tile_width);
  void write(const std::string&filename,PicType type);
  void write_ppm(const std::string &filename) {
    auto fp = std::unique_ptr<FILE, decltype(&fclose)>(
        fopen(filename.c_str(), "wb"), &fclose);
    (void)fprintf(fp.get(), "P6\n%d %d\n255\n", height, width);
    for (uint i = 0; i < width * height; ++i) {
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

class Camera {
public:
  static Transform look_at(Point3f origin, Vector3f WorldUp, Vector3f target);
  virtual Transform look_at() const final { return *view_trans_; }
  Camera(Point3f origin, Vector3f WorldUp, Vector3f target, float fov,
         uint height, uint width);
  virtual ~Camera(){};
  virtual Ray get_ray(float u, float v) const = 0;
  std::unique_ptr<Film> film_ptr_;

  // protected:
  Point3f position_;
  std::shared_ptr<Transform> view_trans_;
  std::shared_ptr<Transform> view_trans_inverse_;
  float fov_;
  float aspect_raio_;
};

inline void Film::write(const std::string&filename,PicType type)
{
  std::unique_ptr<uint8_t[]> data{new uint8_t[height * width * 3]}; //don't support alpha & HDR
  for(uint i =0;i<height*width;i++)
  {
    for(uint j =0;j<3;j++)
    {
      auto tmp = static_cast<uint8_t>(
          std::pow(clamp(0.0f, 1.0f, framebuffer_[i][j]), 0.6) * 254.0f);
      data[3 * i + j] = tmp;
    }
  }
  Image img(data.get(),height,width,type,3);
  if (!img.write_image(filename, 0)) {
    std::cerr << "Error: Writing " + filename << "failed" << std::endl;
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
                      float fov, uint height, uint width)
    : position_(origin), fov_(fov), aspect_raio_(float(width) / height) {
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
