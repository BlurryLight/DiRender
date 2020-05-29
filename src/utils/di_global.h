#pragma once

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
// suppress the unused-parameter warning
template <class T> void ignore(const T &) {}

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
