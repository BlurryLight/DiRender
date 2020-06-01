#include <accelerator/linear_list.h>
#include <cameras/pinhole_camera.h>
#include <cores/render.h>
#include <material/matte_material.h>
#include <math/geometry.hpp>
#include <shapes/sphere.h>
#include <shapes/triangle.h>
#include <utils/OBJ_Loader_wrapper.h>
#include <utils/high_resolution_timer.h>
#include <utils/parse_scene.hpp>
using namespace DR;
//#define NDEBUG
int main(int argc, char **argv) {
  DR::Scene scene;
  std::string filename{"scene.toml"};
  if (argc > 1)
    filename = argv[1];

  int spp = 32;
  parse_scene(filename, &scene, &spp);
#ifdef NDEBUG
  int nthreads = std::thread::hardware_concurrency();
  std::cout << "Threads: " << nthreads << std::endl;
  DR::Render rd(spp, nthreads);
#else
  std::cout << "Running in Debug Mode: MultiThread Mode has been off."
            << std::endl;
  DR::Render rd(1, 1);
#endif
  HRTimer timer;
  timer.start();
  rd.render(scene);
  timer.end();
  std::cout << timer.elapsed() << " ms" << std::endl;
  return 0;
}

// for debug use
//   triangle test
//  std::shared_ptr<Material> Red =
//      std::make_shared<MatteMaterial>(std::make_shared<CheckerTexture>());
//  auto trans = std::make_shared<Transform>();
//  uint vertexIndices[] = {0, 1, 2};
//  Point3f arr[] = {{-50, 50, -5}, {0, -50, -5}, {50, 50, -5}};
//  Normal3f nor[] = {{0, 0, 1}, {0, 0, 1}, {0, 0, 1}};
//  std::shared_ptr<TriangleMesh> mesh = std::make_shared<TriangleMesh>(
//      trans, 1, 3, vertexIndices, arr, nor, nullptr, nullptr);
//  std::shared_ptr<Shape> tri =
//      std::make_shared<Triangle>(trans, trans, 0, mesh, 0);
//  std::shared_ptr<Primitive> tri_prim =
//      std::make_shared<GeometricPrimitive>(tri, Red);
//  std::shared_ptr<Primitive> bvhTree = std::make_shared<BVHTree>(
//      std::vector<std::shared_ptr<Primitive>>{tri_prim});
//  scene.add(bvhTree);

//  scene.add(bvh);
// triangle end
