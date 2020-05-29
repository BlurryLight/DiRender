#include <accelerator/linear_list.h>
#include <cameras/pinhole_camera.h>
#include <cores/render.h>
#include <material/matte_material.h>
#include <math/geometry.hpp>
#include <shapes/sphere.h>
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
  int nthreads = std::thread::hardware_concurrency();
  std::cout << "Threads: " << nthreads << std::endl;
  DR::Render rd(spp, nthreads);
  HRTimer timer;
  timer.start();
  rd.render(scene);
  timer.end();
  std::cout << timer.elapsed() << " ms" << std::endl;
  return 0;
}
