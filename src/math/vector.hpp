#ifndef DI_VECTOR_HPP
#define DI_VECTOR_HPP
#include <iostream>
#include <math/math_utils.hpp>
#include <utils/di_global.h>
NAMESPACE_BEGIN(DR)

template <typename T> class Vector2;
template <typename T> class Point2;
template <typename T> class Normal2;
template <typename T> class Vector3;
template <typename T> class Point3;
template <typename T> class Normal3;
// alias
using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
using Vector3f = Vector3<float>;
using Vector3i = Vector2<int>;

using Point2f = Point2<float>;
using Point2i = Point2<int>;
using Point3f = Point3<float>;
using Point3i = Point2<int>;

using Normal2f = Normal2<float>;
using Normal2i = Normal2<int>;
using Normal3f = Normal3<float>;
using Normal3i = Normal2<int>;

template <typename T> class Vector2 {
public:
  union {
    struct {
      T x, y;
    };
    struct {
      T u, v;
    };
  };

public:
  inline Vector2() : x(0), y(0) {}
  inline Vector2(T xx, T yy) : x(xx), y(yy) {}
  inline Vector2(T v):x(v),y(v){}
  inline Vector2(const Vector2 &other) : x(other.x), y(other.y) {}
  template <typename U> inline Vector2<T> &operator=(const Vector2<U> &other) {
    x = static_cast<T>(other.x);
    y = static_cast<T>(other.y);
    return *this;
  }
  inline bool has_nan() const {
    if (std::is_integral<T>::value)
      return false;
    return (std::isnan(x)) || (std::isnan(y));
  }

  // operator
  // immutable / const

  inline bool operator==(const Vector2 &other) const {
    return (x == other.x) && (y == other.y);
  }

  inline bool operator!=(const Vector2 &other) const {
    return !((*this) == other);
  }
  inline Vector2<T> operator-() const { return Vector2<T>{-x, -y}; }
  inline Vector2<T> operator+(const Vector2<T> &other) const {
    return Vector2<T>{x + other.x, y + other.y};
  }
  inline Vector2<T> operator-(const Vector2<T> &other) const {
    return Vector2<T>{x - other.x, y - other.y};
  }
  template <typename U> inline Vector2<T> operator*(U other) const {
    return Vector2<T>(x * other, y * other);
  }
  template <typename U> inline Vector2<T> operator/(U other) const {
    float inv = 1.0f / other;
    return Vector2<T>(x * inv, y * inv);
  }

  inline friend Vector2 operator*(const float &other, const Vector2 &v) {
    return Vector2(v.x * other, v.y * other);
  }

  inline friend Vector2 operator/(const float &other, const Vector2 &v) {
    return Vector2(other / v.x, other / v.y);
  }

  // mutable

  inline Vector2<T> &operator+=(const Vector2<T> &other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  inline Vector2<T> &operator-=(const Vector2<T> &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  template <typename U> inline Vector2<T> &operator*=(U other) {
    x *= other;
    y *= other;
    return *this;
  }

  template <typename U> inline Vector2<T> &operator/=(U other) {
    float inv = 1.0f / other;
    x *= inv;
    y *= inv;
    return *this;
  }
  // debug
  inline friend std::ostream &operator<<(std::ostream &os, const Vector2 &v) {
    os << "[ " << v.x << ", " << v.y << " ]";
    return os;
  }

  inline float length() const { return std::sqrt(x * x + y * y); }
  inline float l2_norm() const { return length(); }
  inline float squared_length() const { return (x * x + y * y); }

  T &operator[](int index) {
    // no validation
    return (&x)[index];
  }
  T operator[](int index) const {
    // no validation
    return (&x)[index];
  }

  T at(int index) const {
    if (index < 0 || index > 2) {
      throw std::out_of_range("Vector2 index out of range\n");
    }
    return (&x)[index];
  }

  T &at(int index) {
    if (index < 0 || index > 2) {
      throw std::out_of_range("Vector2 index out of range\n");
    }
    return (&x)[index];
  }

  inline Vector2 normalize() { return (*this) / this->length(); }
};

NAMESPACE_END(DR)

#endif
