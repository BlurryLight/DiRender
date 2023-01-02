#pragma once
#include <cameras/camera.h>
NAMESPACE_BEGIN(DR)
struct Scene;
class PinholeCamera : public Camera {
public:
  Ray get_ray(float u, float v) const override;
  PinholeCamera(Point3f origin, Vector3f WorldUp, Vector3f target, float fov,
                uint height, uint width, observer_ptr<Scene> scene,
                bool gamma = false);

private:
  // Vector3f left_bottom_corner_;
  // Vector3f horizontal_;
  // Vector3f vertical_;
  observer_ptr<Transform> proj_trans_;
  observer_ptr<Transform> proj_trans_inverse_;
};
NAMESPACE_END(DR)
