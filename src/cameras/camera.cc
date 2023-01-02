//
// Created by panda on 2021/2/26.
//

#include <cameras/camera.h>
#include <cores/scene.h>

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

#define TINYEXR_USE_THREAD 1
#define TINYEXR_IMPLEMENTATION
#include <tinyexr/tinyexr.h>

#include <spdlog/spdlog.h>
using namespace DR;

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
               uint height, uint width, observer_ptr<Scene> scene, bool gamma)
    : position_(origin), fov_(fov), aspect_ratio_(float(width) / height),
      scene_(scene) {
  auto view_trans = Camera::look_at(origin, WorldUp, target);
  auto [trans, trans_inv] = scene->trans_table.get_tf_and_inv(view_trans);
  view_trans_ = trans;
  view_trans_inverse_ = trans_inv;
  film_ptr_ = std::make_unique<Film>(width, height, gamma);
}

void Film::write(const std::string &filename, PicType type, uint spp) const {
  auto img_name = filename + std::string(MapTypeToSuffix(type));
  switch(type)
  {
    case PicType::kPNG:
      this->write_ldr(img_name,type,spp);
      break;
    case PicType::kEXR:
      this->write_exr(img_name, spp);
    default:
      spdlog::error("Error: Unknown Image Type");
      std::exit(-1);
  }
}

void DR::Film::write_ldr(const std::string &filename,PicType type,uint spp) const {
  std::unique_ptr<uint8_t[]> data{
      new uint8_t[height * width * 3]}; // don't support alpha & HDR
  float inv_spp = 1.0f / float(spp);
  for (uint i = 0; i < height * width; i++) {
    for (uint j = 0; j < 3; j++) {
      uint8_t tmp = 0;

      static constexpr float gamma_index = 1 / 2.2f;
      // Tone mapping
      auto AcesFilmicToneMapping = [](float color) {
        float a = 2.51f;
        float b = 0.03f;
        float c = 2.43f;
        float d = 0.59f;
        float e = 0.14f;

        float new_cl =
            clamp(0.0f, 1.0f,
                  (color * (a * color + b)) / (color * (c * color + d) + e));
        return new_cl;
      };
      float value = tone_mapping_ ? AcesFilmicToneMapping(framebuffer_[i][j] * inv_spp) : framebuffer_[i][j] * inv_spp;
      if (gamma_) {
        tmp = static_cast<uint8_t>(
            std::pow(clamp(0.0f, kOneMinusEps, value), gamma_index) * 255);
      } else {
        tmp = static_cast<uint8_t>(clamp(0.0f, kOneMinusEps, value) * 255);
      }
      data[3 * i + j] = tmp;
    }
  }
  Image img(data.get(), height, width, type, 3);
  if (!img.write_image(filename, false)) {
    std::cerr << "Error: Writing " + filename << "failed" << std::endl;
  }
  
}

void DR::Film::write_exr(const std::string &filename,uint spp) const {
  std::unique_ptr<float[]> data{
      new float[height * width * 3]}; // don't support alpha & HDR
  float inv_spp = 1.0f / float(spp);
  for (uint i = 0; i < height * width; i++) {
    for (uint j = 0; j < 3; j++) {
      float value =  framebuffer_[i][j] * inv_spp;
      data[3 * i + j] = value;
    }
  }
  const char* err = nullptr;
  int ret = SaveEXR(data.get(), width, height, 3, false, filename.c_str(), &err);
  if(ret != TINYEXR_SUCCESS)
  {
    fmt::print("SaveExr Error:{}",err);
    FreeEXRErrorMessage(err);
  }
}
