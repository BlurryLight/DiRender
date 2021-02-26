//
// Created by panda on 2021/2/26.
//

#include <cameras/camera.h>
#include <cores/scene.h>
using namespace DR;
void Film::write_ppm(const std::string &filename) {
  auto fp = std::unique_ptr<FILE, decltype(&fclose)>(
      fopen(filename.c_str(), "wb"), &fclose);
  (void)fprintf(fp.get(), "P6\n%d %d\n255\n", height, width);
  for (uint i = 0; i < width * height; ++i) {
    static unsigned char color[3];
    color[0] =
        (unsigned char)(255 *
                        std::pow(clamp(0.0f, 1.0f, framebuffer_[i].x), 0.6f));
    color[1] =
        (unsigned char)(255 *
                        std::pow(clamp(0.0f, 1.0f, framebuffer_[i].y), 0.6f));
    color[2] =
        (unsigned char)(255 *
                        std::pow(clamp(0.0f, 1.0f, framebuffer_[i].z), 0.6f));
    fwrite(color, 1, 3, fp.get());
  }
}
Transform Camera::look_at(Point3f origin, Vector3f WorldUp, Vector3f target) {
  auto cam_origin = static_cast<Vector3f>(origin);
  auto cam_target = (static_cast<Vector3f>(origin) - target).normalize();
  auto cam_right = cross(WorldUp.normalize(), cam_target).normalize();
  auto cam_up = cross(cam_target, cam_right).normalize();

  Matrix4 part2 = Matrix4(1.0f);
  part2.m[0][3] = -cam_origin[0];
  part2.m[1][3] = -cam_origin[1];
  part2.m[2][3] = -cam_origin[2];

  Matrix4 part1 = Matrix4(1.0f);
  part1.m[0][0] = cam_right[0];
  part1.m[0][1] = cam_right[1];
  part1.m[0][2] = cam_right[2];

  part1.m[1][0] = cam_up[0];
  part1.m[1][1] = cam_up[1];
  part1.m[1][2] = cam_up[2];

  part1.m[2][0] = cam_target[0];
  part1.m[2][1] = cam_target[1];
  part1.m[2][2] = cam_target[2];

  return Transform(part1 * part2);
}

Camera::Camera(Point3f origin, Vector3f WorldUp, Vector3f target, float fov,
               uint height, uint width, observer_ptr<Scene> scene)
    : position_(origin), fov_(fov), aspect_ratio_(float(width) / height),
      scene_(scene) {
  auto view_trans = Camera::look_at(origin, WorldUp, target);
  auto [trans, trans_inv] = scene->trans_table.get_tf_and_inv(view_trans);
  view_trans_ = trans;
  view_trans_inverse_ = trans_inv;
  film_ptr_ = std::make_unique<Film>(width, height);
}

void Film::write(const std::string &filename, PicType type) {
  std::unique_ptr<uint8_t[]> data{
      new uint8_t[height * width * 3]}; // don't support alpha & HDR
  for (uint i = 0; i < height * width; i++) {
    for (uint j = 0; j < 3; j++) {
      auto tmp = static_cast<uint8_t>(
          std::pow(clamp(0.0f, 1.0f, framebuffer_[i][j]), 0.6) * 254.0f);
      data[3 * i + j] = tmp;
    }
  }
  Image img(data.get(), height, width, type, 3);
  if (!img.write_image(filename, false)) {
    std::cerr << "Error: Writing " + filename << "failed" << std::endl;
  }
}
