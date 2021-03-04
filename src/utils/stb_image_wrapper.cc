#include <utils/stb_image_wrapper.h>

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../third_party/stb_image_write.h"
#endif
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb_image.h"
#endif
using namespace DR;
bool Image::write_image(const std::string &path, bool flip) {
  bool success = false;
  stbi_flip_vertically_on_write(flip);
  switch (type_) {
  case PicType::kBMP:
    // Note: Each function in stbi returns 0 on failure and non-0 on success.
    success = stbi_write_bmp(path.c_str(), width_, height_, nChannels_, image_);
    break;
  case PicType::kJPG:
    success =
        // 90 quality
        stbi_write_jpg(path.c_str(), width_, height_, nChannels_, image_, 90);
    break;
  default: // kPNG
    success = stbi_write_png(path.c_str(), width_, height_, nChannels_, image_,
                             width_ * nChannels_);
    break;
  }
  return success;
}
bool Image::load_image(const std::string &path, bool flip) {
  ignore(path);
  ignore(flip);
  return false;
}
