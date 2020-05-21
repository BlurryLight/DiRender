#pragma once
#include <cameras/camera.h>
NAMESPACE_BEGIN(DR)
class PinholeCamera : public Camera {
public:
  Ray get_ray(float u, float v) const override;
  PinholeCamera(Point3f origin, Vector3f WorldUp, Vector3f target, float fov,
                uint height, uint width);

private:
  Vector3f left_bottom_corner_;
  Vector3f horizontal_;
  Vector3f vertical_;
};
NAMESPACE_END(DR)
