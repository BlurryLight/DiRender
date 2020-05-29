#include <utils/stb_image_wrapper.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../third_party/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb_image.h"
using namespace DR;
bool Image::write_image(const std::string &path, bool flip) {
  bool success = false;
  stbi_flip_vertically_on_write(flip);
  switch (type_) {
  case PicType::kBMP:
    /* code */
    break;

  default: // kPNG
    success = (stbi_write_png(path.c_str(), width_, height_, nChannels_, image_,
                              width_ * nChannels_)) == 0
                  ? true
                  : false;
    break;
  }
  return success;
}
bool Image::load_image(const std::string &path, bool flip) {
  ignore(path);
  ignore(flip);
  return false;
}
