#include <accelerator/linear_list.h>
#include <cameras/pinhole_camera.h>
#include <cores/render.h>
#include <material/matte_material.h>
#include <math/geometry.hpp>
#include <shapes/sphere.h>
using namespace DR;
int main(void) {
  DR::Render rd(32);
  DR::Scene scene;

  Point3f origin{0.0f, 0.0f, 0.0f};
  auto cam =
      std::make_shared<PinholeCamera>(origin, Vector3f{0.0, 1.0, 0.0},
                                      Vector3f{0.0, 0.0, -1.0}, 90.0, 400, 400);

  auto mat4 = Matrix4();
  mat4.m[1][3] = 0;
  mat4.m[0][3] = 0;
  mat4.m[2][3] = -2;
  auto trans = std::make_shared<Transform>(mat4);
  auto trans_inv = std::make_shared<Transform>(Transform::Inverse(*trans));
  auto sphere_shape_1 = std::make_shared<Sphere>(trans, trans_inv, false, 1);

  mat4.m[0][3] = 0;
  mat4.m[1][3] = -50;
  mat4.m[2][3] = -2;
  trans = std::make_shared<Transform>(mat4);
  trans_inv = std::make_shared<Transform>(Transform::Inverse(*trans));
  auto sphere_shape_2 = std::make_shared<Sphere>(trans, trans_inv, false, 49);
  auto diffuse_blue = std::make_shared<MatteMaterial>(Vector3f{0.1, 0.1, 0.8f});
  auto diffuse_red = std::make_shared<MatteMaterial>(Vector3f{0.8, 0.1, 0.1f});

  auto sphere_blue =
      std::make_shared<GeometricPrimitive>(sphere_shape_1, diffuse_blue);
  auto sphere_red =
      std::make_shared<GeometricPrimitive>(sphere_shape_2, diffuse_red);
  scene.add(cam);
  scene.add(sphere_red);
  scene.add(sphere_blue);
  rd.render(scene);
  return 0;
}
