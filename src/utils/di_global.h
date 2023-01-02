#pragma once
#include <iostream>

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

// copy from
// https://stackoverflow.com/questions/3767869/adding-message-to-assert
#ifdef NDEBUG
#define assert_msg(condition, message)
#else
#define assert_msg(condition, message)                                         \
  (!(condition))                                                               \
      ? (std::cerr << "Assertion failed: (" << #condition << "), "             \
                   << "function " << __FUNCTION__ << ", file " << __FILE__     \
                   << ", line " << __LINE__ << "." << std::endl                \
                   << message << std::endl,                                    \
         abort(), 0)                                                           \
      : 1
#endif

#define ensure_msg(condition, message)                                         \
  (!(condition))                                                               \
      ? (std::cerr << "ensure failed: (" << #condition << "), "             \
                   << "function " << __FUNCTION__ << ", file " << __FILE__     \
                   << ", line " << __LINE__ << "." << std::endl                \
                   << message << std::endl,                                    \
        0)                                                           \
      : 1

NAMESPACE_BEGIN(DR)
using uint = unsigned int;
using byte = std::uint8_t;

template <typename T> using observer_ptr = const T *;

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
