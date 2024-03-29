#include <math/geometry.hpp>
#include <renderer/bling_phong_renderer.h>
#include <renderer/direct_light_renderer.h>
#include <renderer/path_tracing_renderer.h>
#include <utils/cmake_vars.h>
#include <utils/high_resolution_timer.h>
#include <utils/parse_scene.hh>
#include <utils/parse_scene_txt.hh>
//#define NDEBUG
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
int main(int argc, char **argv) {
  std::cout << "DiRender:\n"
            << "Compiler: " << DR::BUILD_COMPILER << " " << DR::CXX_VER << " on "
            << DR::BUILD_UTC_TIMESTAMP << '\n'
            << "System: " << DR::BUILD_SYSTEM_NAME << " "
            << DR::BUILD_SYSTEM_VERSION << '\n'
            << "Build type: " << DR::BUILD_TYPE << std::endl;

  auto basic_logger =
      spdlog::basic_logger_mt("basic_logger", "Dirender_log.txt", true);
  spdlog::set_default_logger(basic_logger);
  auto ray_logger = spdlog::basic_logger_mt<spdlog::async_factory>(
      "ray_logger", "Dirender_ray.txt", true);
  ray_logger->set_pattern("%v");

  DR::Scene scene;

  try {
    std::string filename{"scene.toml"};
    if (argc > 1)
      filename = argv[1];

    std::unique_ptr<DR::Renderer> rd{nullptr};
    if (argc > 2 && !strcmp(argv[2], "txt")) {
      DR::impl::parse_scene_txt(filename, &scene);
    } else {
      DR::parse_scene(filename, &scene);
    }
  } catch (std::runtime_error e) {
    fmt::print("Error throwed in parser: {}", e.what());
    std::exit(EXIT_FAILURE);
  }
  DR::HRTimer timer;
  timer.start();
  scene.render();
  timer.end();
  std::cout << std::endl;
  std::cout << timer.elapsed() << " ms" << std::endl;
  std::cout << timer.elapsed() / 1000 << " s" << std::endl;
  std::cout << timer.elapsed() / (1000 * 60) << " mins" << std::endl;
  std::cout << timer.elapsed() / (1000 * 60 * 60) << " hours" << std::endl;
  basic_logger->info("Elapsed: {} ms", timer.elapsed());
  return 0;
}
