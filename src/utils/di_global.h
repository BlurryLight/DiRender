#pragma once

#include <cassert>
#include <cstdint>
// macros
// namespace
#if !defined(NAMESPACE_BEGIN)
#define NAMESPACE_BEGIN(name) namespace name {
#endif
#if !defined(NAMESPACE_END)
#define NAMESPACE_END(name) }
#endif
NAMESPACE_BEGIN(DR)
using uint = unsigned int;
using byte = std::uint8_t;
// suppress the unused-parameter warning
template <class T> void ignore(const T &) {}

//#ifndef NDEBUG
//#define private public
//#endif

enum class PicType { kBMP = 0, kPNG, kJPG };
struct MapTypeToSuffix {
  //  static const char *PicTypeString;{".BMP", ".PNG", ".JPG"};
  const char *operator()(PicType type) {
    switch (type) {
    case PicType::kBMP:
      return ".bmp";
      break;
    case PicType::kJPG:
      return ".jpg";
      break;
    default:
      return ".png";
    }
  }
};

NAMESPACE_END(DR)
