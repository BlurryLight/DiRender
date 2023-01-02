#include <cameras/pinhole_camera.h>
#include <sampler/halton_sampler.hh>
#include <sampler/naive_sampler.hh>
#include <cores/scene.h>
using namespace DR;

Ray PinholeCamera::get_ray(float u, float v) const {
  // Vector3f d{left_bottom_corner_ + u * horizontal_ + v * vertical_};
  
  static Point3f origin{0.0f};
  Point3f p{u,v,0.0};
  Point3f nearP = (*proj_trans_inverse_)(p);
  Ray r(origin, (nearP - origin).normalize());
  return (*view_trans_inverse_)(r);
}

PinholeCamera::PinholeCamera(Point3f origin, Vector3f WorldUp, Vector3f target,
                             float fov, uint height, uint width,
                             observer_ptr<Scene> scene, bool gamma)
    : Camera(origin, WorldUp, target, fov, height, width, scene, gamma) {
  float far_clip = 1000.0f;
  float near_clip = 0.001f;
  float recip = 1.0f / (far_clip - near_clip), cot = 1.0f / std::tan(deg2rad(fov / 2.0f));
  
  mat4 perspective;
  perspective[0][0] = cot;
  perspective[1][1] = cot;
  perspective[2][2] = far_clip  * recip;
  perspective[2][3] = -near_clip * far_clip  * recip;
  // 注意定义的坐标系和nori略有不同，主要是Z轴的朝向问题
  perspective[3][2] = -1;
  perspective[3][3] = 0;

  mat4 translate= mat4::Translate(Vector3f(1.0f * aspect_ratio_,1.0f,0.0f));
  mat4 scale = mat4::Scale(Vector3f(0.5f / aspect_ratio_,0.5f,1.0f));
  mat4 trans = scale * translate * perspective;

  auto [proj_ptr,proj_inv_ptr] = scene->trans_table.get_tf_and_inv(trans);
  this->proj_trans_ = proj_ptr;
  this->proj_trans_inverse_ = proj_inv_ptr;
  sampler_ = std::make_unique<HaltonSampler>();
}
