#include <cameras/pinhole_camera.h>
using namespace DR;

Ray PinholeCamera::get_ray(float u, float v) const {
  Point3f origin{0.0f};
  Vector3f d{left_bottom_corner_ + u * horizontal_ + v * vertical_};
  Ray r(origin, d);
  return (*view_trans_inverse_)(r);
}

PinholeCamera::PinholeCamera(Point3f origin, Vector3f WorldUp, Vector3f target,
                             float fov, uint height, uint width,
                             observer_ptr<Scene> scene, bool gamma)
    : Camera(origin, WorldUp, target, fov, height, width, scene, gamma) {
  float theta = deg2rad(fov);
  float h = std::tan(theta / 2.0f);
  h = 2 * h;
  float w = aspect_ratio_ * h;
  horizontal_ = Vector3f{w, 0.0, 0.0};
  vertical_ = Vector3f{0.0, h, 0.0};
  left_bottom_corner_ = Vector3f{0.0f} - horizontal_ * 0.5f - vertical_ * 0.5f -
                        Vector3f{0.0f, 0.0f, 1.0f};
}
