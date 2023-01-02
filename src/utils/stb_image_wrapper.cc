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

bool write_ppm(const std::string &filename, int width,int height,const uint8_t* data) {
  if (data == nullptr) {
    assert(0);
    return false;
  }
  auto fp = std::unique_ptr<FILE, decltype(&fclose)>(
      fopen(filename.c_str(), "wb"), &fclose);
  (void)fprintf(fp.get(), "P6\n%d %d\n255\n", height, width);
  for (uint i = 0; i < width * height; ++i) {
    static unsigned char color[3];
    color[0] =
        (unsigned char)(data[i * 3]);
    color[1] =
        (unsigned char)(data[i * 3 + 1]);
    color[2] =
        (unsigned char)(data[i * 3 + 2]);
    fwrite(color, 1, 3, fp.get());
  }
  return true;
}
bool Image::write_image(const std::string &path, bool flip) {
  bool success = false;
  stbi_flip_vertically_on_write(flip);
  switch (type_) {
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
