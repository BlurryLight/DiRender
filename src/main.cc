#include <accelerator/linear_list.h>
#include <cameras/pinhole_camera.h>
#include <material/matte_material.h>
#include <math/geometry.hpp>
#include <renderer/bling_phong_renderer.h>
#include <renderer/direct_light_renderer.h>
#include <renderer/path_tracing_renderer.h>
#include <shapes/sphere.h>
#include <shapes/triangle.h>
#include <utils/OBJ_Loader_wrapper.h>
#include <utils/cmake_vars.h>
#include <utils/high_resolution_timer.h>
#include <utils/parse_scene.hpp>
#include <utils/parse_scene_txt.hpp>
//#define NDEBUG
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
int main(int argc, char **argv) {
  std::cout << "DiRender:\n"
            << "Built by:" << DR::BUILD_COMPILER << " " << DR::CXX_VER << " on "
            << DR::BUILD_UTC_TIMESTAMP << '\n'
            << "System: " << DR::BUILD_SYSTEM_NAME << " "
            << DR::BUILD_SYSTEM_VERSION << std::endl;

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

  std::unique_ptr<DR::Renderer> rd{nullptr};
  int spp = 32;
#ifdef NDEBUG
  int nthreads = std::thread::hardware_concurrency();
  std::cout << "Threads: " << nthreads << std::endl;
#else
  std::cout << "Running in Debug Mode: MultiThread Mode has been off."
            << std::endl;
#endif
  if (argc > 2 && !strcmp(argv[2], "txt")) {
    DR::IMPL::parse_scene_txt(filename, &scene, &spp);
    scene.background_ += {1.0};
    rd = std::make_unique<DR::BlingPhongRenderer>(1, 1);
  } else {
    DR::parse_scene(filename, &scene, &spp);
#ifdef NDEBUG
    rd = std::make_unique<DR::PathTracingRenderer>(spp, nthreads);
#else
    rd = std::make_unique<DR::PathTracingRenderer>(1, 1);
#endif
  }
  DR::HRTimer timer;
  timer.start();
  rd->render(scene);
  timer.end();
  std::cout << std::endl;
  std::cout << timer.elapsed() << " ms" << std::endl;
  std::cout << timer.elapsed() / 1000 << " s" << std::endl;
  std::cout << timer.elapsed() / (1000 * 60) << " mins" << std::endl;
  std::cout << timer.elapsed() / (1000 * 60 * 60) << " hours" << std::endl;
  basic_logger->info("Elapsed: {} ms", timer.elapsed());
  return 0;
}
