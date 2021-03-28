#pragma once

#include <math/geometry.hpp>
#include <math/transform.h>
#include <memory>
#include <sampler/sampler.hh>
#include <string>
#include <utils/stb_image_wrapper.h>
NAMESPACE_BEGIN(DR)
struct Film {
  Film(uint w, uint h, bool gamma = false)
      : height(h), width(w), gamma_(gamma) {
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
  bool gamma_ = false;
  void write(const std::string &filename, PicType type);
  void write_ppm(const std::string &filename);
};

struct Scene;
class Camera {
public:
  static Transform look_at(Point3f origin, Vector3f WorldUp, Vector3f target);
  virtual Transform look_at() const final { return *view_trans_; }
  Camera(Point3f origin, Vector3f WorldUp, Vector3f target, float fov,
         uint height, uint width, observer_ptr<Scene> scene,
         bool gamma = false);
  virtual ~Camera() = default;
  virtual Ray get_ray(float u, float v) const = 0;
  std::unique_ptr<Film> film_ptr_;
  std::unique_ptr<Sampler> sampler_;

  // protected:
  Point3f position_;
  observer_ptr<Transform> view_trans_;
  observer_ptr<Transform> view_trans_inverse_;
  float fov_;
  float aspect_ratio_;
  const Scene *scene_;
};

NAMESPACE_END(DR)
