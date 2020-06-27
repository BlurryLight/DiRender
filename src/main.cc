#include <accelerator/linear_list.h>
#include <cameras/pinhole_camera.h>
#include <material/matte_material.h>
#include <math/geometry.hpp>
#include <renderer/direct_light_renderer.h>
#include <renderer/path_tracing_renderer.h>
#include <shapes/sphere.h>
#include <shapes/triangle.h>
#include <utils/OBJ_Loader_wrapper.h>
#include <utils/high_resolution_timer.h>
#include <utils/parse_scene.hpp>
//#define NDEBUG
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
int main(int argc, char **argv) {
  auto basic_logger =
      spdlog::basic_logger_mt("basic_logger", "Dirender_log.txt", true);
  spdlog::set_default_logger(basic_logger);
  auto ray_logger = spdlog::basic_logger_mt<spdlog::async_factory>(
      "ray_logger", "Dirender_ray.txt", true);
  ray_logger->set_pattern("%v");

  DR::Scene scene;
  std::string filename{"scene.toml"};
  if (argc > 1)
    filename = argv[1];

  int spp = 32;
  DR::parse_scene(filename, &scene, &spp);
#ifdef NDEBUG
  int nthreads = std::thread::hardware_concurrency();
  std::cout << "Threads: " << nthreads << std::endl;
  DR::PathTracingRenderer rd(spp, nthreads);
//  DR::DirectLightRenderer rd(spp, nthreads);
#else
  std::cout << "Running in Debug Mode: MultiThread Mode has been off."
            << std::endl;
  DR::PathTracingRenderer rd(1, 1);
//  DR::DirectLightRenderer rd(1, 1);
#endif
  DR::HRTimer timer;
  timer.start();
  rd.render(scene);
  timer.end();
  std::cout << std::endl;
  std::cout << timer.elapsed() << " ms" << std::endl;
  std::cout << timer.elapsed() / 1000 << " s" << std::endl;
  std::cout << timer.elapsed() / (1000 * 60) << " mins" << std::endl;
  std::cout << timer.elapsed() / (1000 * 60 * 60) << " hours" << std::endl;
  basic_logger->info("Elapsed: {} ms", timer.elapsed());
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
