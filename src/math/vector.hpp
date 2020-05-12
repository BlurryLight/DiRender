#pragma once
#include <iostream>
#include <math/math_utils.hpp>
#include <utils/di_global.h>
NAMESPACE_BEGIN(DR)

class Vector2f;
class Vector3f;
class Point2f;
class Point3f;
// Should we really need to build a Vector3f-like Normal3f class,
// or just have an alias to hint it's a normal vector?
// class Normal2f;
// class Normal3f;

// alias
using vec3 = Vector3f;
using vec2 = Vector2f;
// Normal2f is really rare
// using Normal2f = Vector2f;
using Normal3f = Vector3f;

class Vector2f {
public:
  union {
    struct {
      float x, y;
    };
    struct {
      float u, v;
    };
  };

public:
  inline Vector2f() : x(0), y(0) {}
  inline Vector2f(float xx, float yy) : x(xx), y(yy) {}
  inline Vector2f(float v) : x(v), y(v) {}
  inline Vector2f(const Vector2f &other) : x(other.x), y(other.y) {}
  explicit inline Vector2f(const Point2f &other);
  inline Vector2f &operator=(const Vector2f &other) {
    x = other.x;
    y = other.y;
    return *this;
  }
  inline bool has_nan() const { return (std::isnan(x)) || (std::isnan(y)); }

  // operator
  // immutable / const

  inline bool operator==(const Vector2f &other) const {
    //    return (x == other.x) && (y == other.y);
    return (almost_equal(x, other.x)) && (almost_equal(y, other.y));
  }

  inline bool operator!=(const Vector2f &other) const {
    return !((*this) == other);
  }
  inline Vector2f operator-() const { return Vector2f{-x, -y}; }
  inline Vector2f operator+(const Vector2f &other) const {
    return Vector2f{x + other.x, y + other.y};
  }
  inline Vector2f operator-(const Vector2f &other) const {
    return Vector2f{x - other.x, y - other.y};
  }
  inline Vector2f operator*(float other) const {
    return Vector2f{x * other, y * other};
  }
  inline Vector2f operator/(float other) const {
    float inv = 1.0f / other;
    return Vector2f{x * inv, y * inv};
  }

  inline friend Vector2f operator*(const float &other, const Vector2f &v) {
    return Vector2f{v.x * other, v.y * other};
  }

  inline friend Vector2f operator/(const float &other, const Vector2f &v) {
    return Vector2f{other / v.x, other / v.y};
  }

  inline friend Vector2f operator+(const float &other, const Vector2f &v) {
    return Vector2f{other + v.x, other + v.y};
  }

  inline friend Vector2f operator-(const float &other, const Vector2f &v) {
    return Vector2f{other - v.x, other - v.y};
  }

  // mutable

  inline Vector2f &operator+=(const Vector2f &other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  inline Vector2f &operator-=(const Vector2f &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  inline Vector2f &operator*=(float other) {
    x *= other;
    y *= other;
    return *this;
  }

  inline Vector2f &operator/=(float other) {
    float inv = 1.0f / other;
    x *= inv;
    y *= inv;
    return *this;
  }
  // debug
  inline friend std::ostream &operator<<(std::ostream &os, const Vector2f &v) {
    os << "[ " << v.x << ", " << v.y << " ]";
    return os;
  }

  inline float length() const { return std::sqrt(x * x + y * y); }
  inline float l2_norm() const { return length(); }
  inline float squared_length() const { return (x * x + y * y); }

  inline float &operator[](int index) {
    // no validation
    return (&x)[index];
  }
  inline float operator[](int index) const {
    // no validation
    return (&x)[index];
  }

  inline float at(int index) const {
    if (index < 0 || index > 1) {
      throw std::out_of_range("Vector2f index out of range\n");
    }
    return (&x)[index];
  }

  inline float &at(int index) {
    if (index < 0 || index > 1) {
      throw std::out_of_range("Vector2 index out of range\n");
    }
    return (&x)[index];
  }

  inline Vector2f normalize() const { return (*this) / this->length(); }

  inline float dot(const Vector2f &other) const {
    return x * other.x + y * other.y;
  }

  inline Vector3f cross(const Vector2f &other) const;

  template <class T> static T min(const T &lhs, const T &rhs) = delete;
  template <class T>
  static T max(const T &lhs, const T &rhs) = delete; // forbid other type
  static Vector2f min(const Vector2f &lhs, const Vector2f &rhs) {
    return Vector2f{std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y)};
  }
  static Vector2f max(const Vector2f &lhs, const Vector2f &rhs) {
    return Vector2f{std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y)};
  }
};

class Vector3f {

public:
  union {
    struct {
      float x, y, z;
    };
    struct {
      float u, v, w;
    };
  };

public:
  inline Vector3f() : x(0), y(0), z(0) {}
  inline Vector3f(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
  inline Vector3f(float v) : x(v), y(v), z(v) {}
  inline Vector3f(const Vector2f &other) : x(other.x), y(other.y), z(0.0f) {}
  inline Vector3f(const Vector3f &other) : x(other.x), y(other.y), z(other.z) {}
  explicit inline Vector3f(const Point3f &other);
  inline Vector3f &operator=(const Vector2f &other) {
    x = other.x;
    y = other.y;
    z = 0.0f;
    return *this;
  }

  inline Vector3f &operator=(const Vector3f &other) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
  }

  inline bool has_nan() const {
    return (std::isnan(x)) || (std::isnan(y)) || (std::isnan(z));
  }

  // operator
  // immutable / const

  inline bool operator==(const Vector3f &other) const {
    //    return (x == other.x) && (y == other.y) && (z == other.z);
    return (almost_equal(x, other.x)) && (almost_equal(y, other.y)) &&
           (almost_equal(z, other.z));
  }

  inline bool operator!=(const Vector3f &other) const {
    return !((*this) == other);
  }
  inline Vector3f operator-() const { return Vector3f{-x, -y, -z}; }
  inline Vector3f operator+(const Vector3f &other) const {
    return Vector3f{x + other.x, y + other.y, z + other.z};
  }
  inline Vector3f operator-(const Vector3f &other) const {
    return Vector3f{x - other.x, y - other.y, z - other.z};
  }
  inline Vector3f operator*(float other) const {
    return Vector3f{x * other, y * other, z * other};
  }
  inline Vector3f operator/(float other) const {
    float inv = 1.0f / other;
    return Vector3f{x * inv, y * inv, z * inv};
  }

  inline friend Vector3f operator*(const float &other, const Vector3f &v) {
    return Vector3f(v.x * other, v.y * other, v.z * other);
  }

  inline friend Vector3f operator/(const float &other, const Vector3f &v) {
    return Vector3f(other / v.x, other / v.y, other / v.z);
  }

  inline friend Vector3f operator+(const float &other, const Vector3f &v) {
    return Vector3f(other + v.x, other + v.y, other + v.z);
  }

  inline friend Vector3f operator-(const float &other, const Vector3f &v) {
    return Vector3f(other - v.x, other - v.y, other - v.z);
  }

  // mutable

  inline Vector3f &operator+=(const Vector3f &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }
  inline Vector3f &operator-=(const Vector3f &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  inline Vector3f &operator*=(float other) {
    x *= other;
    y *= other;
    z *= other;
    return *this;
  }

  inline Vector3f &operator/=(float other) {
    float inv = 1.0f / other;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
  }
  // debug
  inline friend std::ostream &operator<<(std::ostream &os, const Vector3f &v) {
    os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
    return os;
  }

  inline float length() const { return std::sqrt(x * x + y * y + z * z); }
  inline float l2_norm() const { return length(); }
  inline float squared_length() const { return (x * x + y * y + z * z); }

  inline float &operator[](int index) {
    // no validation
    return (&x)[index];
  }
  inline float operator[](int index) const {
    // no validation
    return (&x)[index];
  }

  inline float at(int index) const {
    if (index < 0 || index > 2) {
      throw std::out_of_range("Vector3f index out of range\n");
    }
    return (&x)[index];
  }

  inline float &at(int index) {
    if (index < 0 || index > 2) {
      throw std::out_of_range("Vector3f index out of range\n");
    }
    return (&x)[index];
  }

  inline Vector3f normalize() const { return (*this) / this->length(); }
  inline float dot(const Vector3f &other) const {
    return x * other.x + y * other.y + z * other.z;
  }
  inline Vector3f cross(const Vector3f &other) const {
    return Vector3f{this->y * other.z - this->z * other.y,
                    this->z * other.x - this->x * other.z,
                    this->x * other.y - this->y * other.x

    };
  }

  template <class T> static T min(const T &lhs, const T &rhs) = delete;
  template <class T>
  static T max(const T &lhs, const T &rhs) = delete; // forbid other type

  static Vector3f min(const Vector3f &lhs, const Vector3f &rhs) {
    return Vector3f{std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y),
                    std::min(lhs.z, rhs.z)};
  }
  static Vector3f max(const Vector3f &lhs, const Vector3f &rhs) {
    return Vector3f{std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y),
                    std::max(lhs.z, rhs.z)};
  }
};

class Point2f {
public:
  float x, y;

public:
  inline Point2f() : x(0), y(0) {}
  inline Point2f(float xx, float yy) : x(xx), y(yy) {}
  inline Point2f(float v) : x(v), y(v) {}
  inline Point2f(const Point2f &other) : x(other.x), y(other.y) {}
  inline Point2f &operator=(const Point2f &other) {
    x = other.x;
    y = other.y;
    return *this;
  }
  inline bool has_nan() const { return (std::isnan(x)) || (std::isnan(y)); }

  // operator
  // immutable / const

  inline bool operator==(const Point2f &other) const {
    //    return (x == other.x) && (y == other.y);
    return (almost_equal(x, other.x) && almost_equal(y, other.y));
  }

  inline bool operator!=(const Point2f &other) const {
    return !((*this) == other);
  }
  inline Point2f operator-() const { return Point2f{-x, -y}; }
  inline Point2f operator+(const Vector2f &other) const {
    return Point2f{x + other.x, y + other.y};
  }
  inline Point2f operator-(float other) const {
    return Point2f{x - other, y - other};
  }
  inline Point2f operator+(float other) const {
    return Point2f{x + other, y + other};
  }
  inline Point2f operator-(const Vector2f &other) const {
    return Point2f{x - other.x, y - other.y};
  }
  inline Vector2f operator-(const Point2f &other) const {
    return Vector2f{x - other.x, y - other.y};
  }
  inline Point2f operator*(float other) const {
    return Point2f{x * other, y * other};
  }
  inline Point2f operator/(float other) const {
    float inv = 1.0f / other;
    return Point2f{x * inv, y * inv};
  }

  inline friend Point2f operator*(const float &other, const Point2f &v) {
    return Point2f{v.x * other, v.y * other};
  }

  inline friend Point2f operator/(const float &other, const Point2f &v) {
    return Point2f{other / v.x, other / v.y};
  }

  inline friend Point2f operator+(const float &other, const Point2f &v) {
    return Point2f{other + v.x, other + v.y};
  }

  inline friend Point2f operator-(const float &other, const Point2f &v) {
    return Point2f{other - v.x, other - v.y};
  }

  // mutable

  inline Point2f &operator+=(const Vector2f &other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  inline Point2f &operator-=(const Vector2f &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  inline Point2f &operator*=(float other) {
    x *= other;
    y *= other;
    return *this;
  }

  inline Point2f &operator/=(float other) {
    float inv = 1.0f / other;
    x *= inv;
    y *= inv;
    return *this;
  }
  // debug
  inline friend std::ostream &operator<<(std::ostream &os, const Point2f &v) {
    os << "[ " << v.x << ", " << v.y << " ]";
    return os;
  }

  inline float &operator[](int index) {
    // no validation
    return (&x)[index];
  }
  inline float operator[](int index) const {
    // no validation
    return (&x)[index];
  }

  inline float at(int index) const {
    if (index < 0 || index > 1) {
      throw std::out_of_range("Point2f index out of range\n");
    }
    return (&x)[index];
  }

  inline float &at(int index) {
    if (index < 0 || index > 1) {
      throw std::out_of_range("Point2f index out of range\n");
    }
    return (&x)[index];
  }

  inline Vector2f operator()() const { return Vector2f{*this}; }

  template <class T> static T min(const T &lhs, const T &rhs) = delete;
  template <class T>
  static T max(const T &lhs, const T &rhs) = delete; // forbid other type
  static Point2f max(const Point2f &lhs, const Point2f &rhs) {
    return Point2f{std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y)};
  }
  static Point2f min(const Point2f &lhs, const Point2f &rhs) {
    return Point2f{std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y)};
  }
};

class Point3f {
public:
  float x, y, z;

public:
  inline Point3f() : x(0), y(0), z(0) {}
  inline Point3f(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
  inline Point3f(float v) : x(v), y(v), z(v) {}
  inline Point3f(const Point3f &other) : x(other.x), y(other.y), z(other.z) {}
  inline Point3f(const Point2f &other) : x(other.x), y(other.y), z(0.0f) {}
  inline Point3f &operator=(const Point3f &other) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
  }
  inline bool has_nan() const {
    return (std::isnan(x)) || (std::isnan(y)) || std::isnan(z);
  }

  // operator
  // immutable / const

  inline bool operator==(const Point3f &other) const {
    //    return (x == other.x) && (y == other.y) && (z == other.z);
    return (almost_equal(x, other.x)) && (almost_equal(y, other.y)) &&
           (almost_equal(z, other.z));
  }

  inline bool operator!=(const Point3f &other) const {
    return !((*this) == other);
  }
  inline Point3f operator-() const { return Point3f{-x, -y, -z}; }
  inline Point3f operator+(const Vector3f &other) const {
    return Point3f{x + other.x, y + other.y, z + other.z};
  }
  inline Point3f operator-(const Vector3f &other) const {
    return Point3f{x - other.x, y - other.y, z - other.z};
  }
  inline Vector3f operator-(const Point3f &other) const {
    return Vector3f{x - other.x, y - other.y, z - other.z};
  }
  inline Point3f operator-(float other) const {
    return Point3f{x - other, y - other, z - other};
  }
  inline Point3f operator+(float other) const {
    return Point3f{x + other, y + other, z + other};
  }
  inline Point3f operator*(float other) const {
    return Point3f{x * other, y * other, z * other};
  }
  inline Point3f operator/(float other) const {
    float inv = 1.0f / other;
    return Point3f{x * inv, y * inv, z * inv};
  }

  inline friend Point3f operator*(const float &other, const Point3f &v) {
    return Point3f{v.x * other, v.y * other, v.z * other};
  }

  inline friend Point3f operator/(const float &other, const Point3f &v) {
    return Point3f{other / v.x, other / v.y, other / v.z};
  }

  inline friend Point3f operator+(const float &other, const Point3f &v) {
    return Point3f(other + v.x, other + v.y, other + v.z);
  }

  inline friend Point3f operator-(const float &other, const Point3f &v) {
    return Point3f(other - v.x, other - v.y, other - v.z);
  }

  // mutable

  inline Point3f &operator+=(const Vector3f &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }
  inline Point3f &operator-=(const Vector3f &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  inline Point3f &operator*=(float other) {
    x *= other;
    y *= other;
    z *= other;
    return *this;
  }

  inline Point3f &operator/=(float other) {
    float inv = 1.0f / other;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
  }
  // debug
  inline friend std::ostream &operator<<(std::ostream &os, const Point3f &v) {
    os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
    return os;
  }

  inline float &operator[](int index) {
    // no validation
    return (&x)[index];
  }
  inline float operator[](int index) const {
    // no validation
    return (&x)[index];
  }

  inline float at(int index) const {
    if (index < 0 || index > 2) {
      throw std::out_of_range("Point3f index out of range\n");
    }
    return (&x)[index];
  }

  inline float &at(int index) {
    if (index < 0 || index > 2) {
      throw std::out_of_range("Point3f index out of range\n");
    }
    return (&x)[index];
  }

  template <class T> static T min(const T &lhs, const T &rhs) = delete;
  template <class T>
  static T max(const T &lhs, const T &rhs) = delete; // forbid other type

  inline Vector3f operator()() const { return Vector3f{*this}; }
  static Point3f min(const Point3f &lhs, const Point3f &rhs) {
    return Point3f{std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y),
                   std::min(lhs.z, rhs.z)};
  }
  static Point3f max(const Point3f &lhs, const Point3f &rhs) {
    return Point3f{std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y),
                   std::max(lhs.z, rhs.z)};
  }
};

// some implementation
inline Vector2f::Vector2f(const Point2f &other) : x(other.x), y(other.y) {}

inline Vector3f::Vector3f(const Point3f &other)
    : x(other.x), y(other.y), z(other.z) {}

inline Vector3f Vector2f::cross(const Vector2f &other) const {
  Vector3f a(*this);
  Vector3f b(other);
  return Vector3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                  a.x * b.y - a.y * b.x);
}

inline Vector3f cross(const Vector3f &lhs, const Vector3f &rhs) {
  return lhs.cross(rhs);
}

inline Vector3f cross(const Vector2f &lhs, const Vector2f &rhs) {
  return lhs.cross(rhs);
}

inline float dot(const Vector2f &lhs, const Vector2f &rhs) {
  return lhs.dot(rhs);
}
inline float dot(const Vector3f &lhs, const Vector3f &rhs) {
  return lhs.dot(rhs);
}

NAMESPACE_END(DR)
