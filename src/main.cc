#include <accelerator/linear_list.h>
#include <cameras/pinhole_camera.h>
#include <cores/render.h>
#include <material/matte_material.h>
#include <math/geometry.hpp>
#include <shapes/sphere.h>
#include <shapes/triangle.h>
#include <utils/high_resolution_timer.h>
#include <utils/parse_scene.hpp>
using namespace DR;
int main(int argc, char **argv) {
  DR::Scene scene;
  std::string filename{"scene.toml"};
  if (argc > 1)
    filename = argv[1];

  int spp = 32;
  parse_scene(filename, &scene, &spp);
  // triangle test
  std::shared_ptr<Material> Red =
      std::make_shared<MatteMaterial>(Vector3f{0.9, 0.0, 0.0});
  auto trans = std::make_shared<Transform>();
  int vertexIndices[] = {0, 1, 2};
  Point3f arr[] = {{-10, -1, -5}, {1, -1, 0}, {5, 5, -5}};
  Normal3f nor[] = {{0, 0, 1}, {0, 0, 1}, {0, 0, 1}};
  std::shared_ptr<TriangleMesh> mesh = std::make_shared<TriangleMesh>(
      trans, 1, 3, vertexIndices, arr, nor, nullptr, nullptr);
  std::shared_ptr<Shape> tri =
      std::make_shared<Triangle>(trans, trans, 0, mesh, 0);
  std::shared_ptr<Primitive> tri_prim =
      std::make_shared<GeometricPrimitive>(tri, Red);

  scene.add(tri_prim);
  // triangle end
  int nthreads = std::thread::hardware_concurrency();
  std::cout << "Threads: " << nthreads << std::endl;
#ifdef NDEBUG
  DR::Render rd(spp, nthreads);
#else
  DR::Render rd(1, nthreads);
#endif
  HRTimer timer;
  timer.start();
  rd.render(scene);
  timer.end();
  std::cout << timer.elapsed() << " ms" << std::endl;
  return 0;
}
