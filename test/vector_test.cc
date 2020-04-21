#include <gtest/gtest.h>
#include <math/vector.hpp>
using namespace DR;
TEST(VECTOR, vector2) {
  // ctor,dtor
  {
    Vector2f test;
    EXPECT_FLOAT_EQ(test.x, 0.0f);
    EXPECT_FLOAT_EQ(test.y, 0.0f);
    EXPECT_FLOAT_EQ(test.u, 0.0f);
    EXPECT_FLOAT_EQ(test.v, 0.0f);
  }
  // nan
  {
    Vector2f ftest{std::numeric_limits<float>::quiet_NaN(), 0.0f};
    Vector2f ftest1;
    EXPECT_TRUE(ftest.has_nan());
    EXPECT_FALSE(ftest1.has_nan());
  }
  // operators
  {
    // float
    Vector2f ftest1{1.0f};
    Vector2f ftest2{-1.0f};
    EXPECT_FALSE(ftest1 == ftest2);
    ftest2 = -ftest2;
    EXPECT_TRUE(ftest1 == ftest2);
    ftest2 += 1.0f; //{2.0f,2.0f}
    EXPECT_FALSE(ftest1 == ftest2);
    ftest2 -= 1.0f; //{1.0f,1.0f}
    EXPECT_TRUE(ftest1 == ftest2);
    ftest2 *= -1;
    EXPECT_TRUE(-ftest1 == ftest2);
    ftest2 /= -1;
    EXPECT_TRUE(ftest1 == ftest2);
  }
  // other
  {
    Vector2f ftest{1.0f};
    EXPECT_FLOAT_EQ(ftest.length(), M_SQRT2);
    EXPECT_FLOAT_EQ(ftest.squared_length(), M_SQRT2 * M_SQRT2);

    EXPECT_FLOAT_EQ(ftest[0], 1.0f);
    EXPECT_FLOAT_EQ(ftest[1], 1.0f);
    EXPECT_FLOAT_EQ(ftest.at(0), 1.0f);
    EXPECT_FLOAT_EQ(ftest.at(1), 1.0f);

    ftest.at(1) = 2.0f;
    EXPECT_FLOAT_EQ(ftest.at(1), 2.0f);
    ftest.at(0) = 0.0f; // ftest={0.0f,2.0f}
    ftest = ftest.normalize();
    EXPECT_EQ(ftest, Vector2f(0.0f, 1.0f));
  }

  // dot & cross
  {
    Vector2f x{1.0f, 0.0f};
    Vector2f y{0.0f, 1.0f};
    float xdoty = x.dot(y);
    float xdoty1 = dot(x, y);

    auto xcy = x.cross(y);
    auto xcy1 = cross(x, y);

    EXPECT_FLOAT_EQ(xdoty, 0.0f);
    EXPECT_EQ(xcy, Vector3f(0.0f, 0.0f, 1.0f));

    EXPECT_FLOAT_EQ(xdoty, xdoty1);
    EXPECT_EQ(xcy, xcy1);
  }
  // max_min
  {
    Vector2f x{1.0f, 0.0};
    Vector2f y{0.0f, 1.0f};

    auto t1 = Vector2f::max(x, y);
    auto t2 = Vector2f::min(x, y);
    EXPECT_EQ(t1, Vector2f(1.0f, 1.0f));
    EXPECT_EQ(t2, Vector2f(0.0f, 0.0f));
  }
}

TEST(VECTOR, vector3) {

  // ctor,dtor
  {
    Vector3f test;
    EXPECT_FLOAT_EQ(test.x, 0.0f);
    EXPECT_FLOAT_EQ(test.y, 0.0f);
    EXPECT_FLOAT_EQ(test.z, 0.0f);
    EXPECT_FLOAT_EQ(test.u, 0.0f);
    EXPECT_FLOAT_EQ(test.v, 0.0f);
    EXPECT_FLOAT_EQ(test.w, 0.0f);
  }
  // nan
  {
    Vector3f ftest{std::numeric_limits<float>::quiet_NaN(), 0.0f, 0.0f};
    Vector3f ftest1;
    EXPECT_TRUE(ftest.has_nan());
    EXPECT_FALSE(ftest1.has_nan());
  }
  // operators
  {
    // float
    Vector3f ftest1{1.0f};
    Vector3f ftest2{-1.0f};
    EXPECT_FALSE(ftest1 == ftest2);
    ftest2 = -ftest2;
    EXPECT_TRUE(ftest1 == ftest2);
    ftest2 += 1.0f; //{2.0f,2.0f,2.0f}
    EXPECT_FALSE(ftest1 == ftest2);
    ftest2 -= 1.0f; //{1.0f,1.0f,1.0f}
    EXPECT_TRUE(ftest1 == ftest2);
    ftest2 *= -1;
    EXPECT_TRUE(-ftest1 == ftest2);
    ftest2 /= -1;
    EXPECT_TRUE(ftest1 == ftest2);

    ftest2 = Vector3f{1.0f};
    ftest2 = ftest2 + 1.0f; //{2.0f,2.0f,2.0f}
    EXPECT_FALSE(ftest1 == ftest2);
    ftest2 = ftest2 - 1.0f; //{1.0f,1.0f,1.0f}
    EXPECT_TRUE(ftest1 == ftest2);
    ftest2 = 1.0f - ftest2; //{0.0f}
    EXPECT_TRUE(ftest2 == Vector3f{0.0f});
    ftest2 = 1 + ftest2;
    EXPECT_TRUE(ftest1 == ftest2);
  }
  // other
  {
    Vector3f ftest{1.0f};
    EXPECT_FLOAT_EQ(ftest.length(), std::sqrt(3.0f));
    EXPECT_FLOAT_EQ(ftest.squared_length(), std::pow(std::sqrt(3), 2));

    EXPECT_FLOAT_EQ(ftest[0], 1.0f);
    EXPECT_FLOAT_EQ(ftest[1], 1.0f);
    EXPECT_FLOAT_EQ(ftest[2], 1.0f);
    EXPECT_FLOAT_EQ(ftest.at(0), 1.0f);
    EXPECT_FLOAT_EQ(ftest.at(1), 1.0f);
    EXPECT_FLOAT_EQ(ftest.at(2), 1.0f);

    ftest.at(1) = 2.0f;
    EXPECT_FLOAT_EQ(ftest.at(1), 2.0f);
    ftest.at(0) = 0.0f; // ftest={0.0f,2.0f,1.0f}
    ftest = ftest.normalize();
    auto sqrt5 = std::sqrt(5);
    EXPECT_EQ(ftest, Vector3f(0.0f, 2.0f / sqrt5, 1.0f / sqrt5));
  }
  // dot & cross
  {
    Vector3f x{1.0f, 0.0f, 0.0f};
    Vector3f y{0.0f, 1.0f, 0.0f};
    float xdoty = x.dot(y);
    float xdoty1 = dot(x, y);

    auto xcy = x.cross(y);
    auto xcy1 = cross(x, y);

    EXPECT_FLOAT_EQ(xdoty, 0.0f);
    EXPECT_EQ(xcy, Vector3f(0.0f, 0.0f, 1.0f));

    EXPECT_FLOAT_EQ(xdoty, xdoty1);
    EXPECT_EQ(xcy, xcy1);
  }
  // max_min
  {
    Vector3f x{1.0f, 0.0f, 0.0f};
    Vector3f y{0.0f, 1.0f, 0.0f};

    auto t1 = Vector3f::max(x, y);
    auto t2 = Vector3f::min(x, y);
    EXPECT_EQ(t1, Vector3f(1.0f, 1.0f, 0.0f));
    EXPECT_EQ(t2, Vector3f(0.0f, 0.0f, 0.0f));
  }
}

TEST(POINT, Point2) {
  // ctor,dtor
  {
    Point2f test;
    EXPECT_FLOAT_EQ(test.x, 0.0f);
    EXPECT_FLOAT_EQ(test.y, 0.0f);
  }
  // nan
  {
    Point2f ftest{std::numeric_limits<float>::quiet_NaN(), 0.0f};
    Point2f ftest1;
    EXPECT_TRUE(ftest.has_nan());
    EXPECT_FALSE(ftest1.has_nan());
  }
  // operators
  {
    // float
    Point2f ftest1{1.0f};
    Point2f ftest2{-1.0f};
    EXPECT_FALSE(ftest1 == ftest2);
    ftest2 = -ftest2;
    EXPECT_TRUE(ftest1 == ftest2);
    ftest2 += 1.0f; //{2.0f,2.0f}
    EXPECT_FALSE(ftest1 == ftest2);
    ftest2 -= 1.0f; //{1.0f,1.0f}
    EXPECT_TRUE(ftest1 == ftest2);
    ftest2 *= -1;
    EXPECT_TRUE(-ftest1 == ftest2);
    ftest2 /= -1;
    EXPECT_TRUE(ftest1 == ftest2);

    ftest2 = 1 + ftest2;
    ftest2 = ftest2 + 1;
    EXPECT_FALSE(ftest1 == ftest2);
    ftest2 = 4.0 - ftest2;
    EXPECT_TRUE(ftest1 == ftest2);
  }
  // other
  {
    Point2f ftest{1.0f};
    EXPECT_FLOAT_EQ(ftest[0], 1.0f);
    EXPECT_FLOAT_EQ(ftest[1], 1.0f);
    EXPECT_FLOAT_EQ(ftest.at(0), 1.0f);
    EXPECT_FLOAT_EQ(ftest.at(1), 1.0f);

    ftest.at(1) = 2.0f;
    EXPECT_FLOAT_EQ(ftest.at(1), 2.0f);
    ftest.at(0) = 0.0f; // ftest={0.0f,2.0f}
    EXPECT_EQ(ftest, Point2f(0.0f, 2.0f));
  }

  // operator+
  {
    Point2f ftest1{1.0f};
    Point2f ftest2{1.0f};
//    auto test1 = ftest1 + ftest2; //error cannot implicitily convert to Vector2f
    auto test1 = ftest1 + static_cast<DR::Vector2f>(ftest2); //Ok
    auto test2 = ftest1 + Vector2f(ftest2); //Ok
    auto test3 = ftest1 - ftest2;
    EXPECT_EQ(test1,test2);
    EXPECT_EQ(test2,Point2f(2.0f));
    EXPECT_EQ(test3, Vector2f());
  }
  // max_min
  {
    Point2f x{1.0f, 0.0};
    Point2f y{0.0f, 1.0f};

    auto t1 = Point2f::max(x, y);
    auto t2 = Point2f::min(x, y);
    EXPECT_EQ(t1, Point2f(1.0f, 1.0f));
    EXPECT_EQ(t2, Point2f(0.0f, 0.0f));
  }
}

TEST(POINT, point3) {

  // ctor,dtor
  {
    Point3f test;
    EXPECT_FLOAT_EQ(test.x, 0.0f);
    EXPECT_FLOAT_EQ(test.y, 0.0f);
    EXPECT_FLOAT_EQ(test.z, 0.0f);
  }
  // nan
  {
    Point3f ftest{std::numeric_limits<float>::quiet_NaN(), 0.0f, 0.0f};
    Point3f ftest1;
    EXPECT_TRUE(ftest.has_nan());
    EXPECT_FALSE(ftest1.has_nan());
  }
  // operators
  {
    // float
    Point3f ftest1{1.0f};
    Point3f ftest2{-1.0f};
    EXPECT_FALSE(ftest1 == ftest2);
    ftest2 = -ftest2;
    EXPECT_TRUE(ftest1 == ftest2);
    ftest2 += 1.0f; //{2.0f,2.0f,2.0f}
    EXPECT_FALSE(ftest1 == ftest2);
    ftest2 -= 1.0f; //{1.0f,1.0f,1.0f}
    EXPECT_TRUE(ftest1 == ftest2);
    ftest2 *= -1;
    EXPECT_TRUE(-ftest1 == ftest2);
    ftest2 /= -1;
    EXPECT_TRUE(ftest1 == ftest2);

    ftest2 = Point3f{1.0f};
    ftest2 = ftest2 + 1.0f; //{2.0f,2.0f,2.0f}
    EXPECT_FALSE(ftest1 == ftest2);
    ftest2 = ftest2 - 1.0f; //{1.0f,1.0f,1.0f}
    EXPECT_TRUE(ftest1 == ftest2);
    ftest2 = 1.0f - ftest2; //{0.0f}
    EXPECT_TRUE(ftest2 == Point3f{0.0f});
    ftest2 = 1 + ftest2;
    EXPECT_TRUE(ftest1 == ftest2);
  }
  // other
  {
    Point3f ftest{1.0f};

    EXPECT_FLOAT_EQ(ftest[0], 1.0f);
    EXPECT_FLOAT_EQ(ftest[1], 1.0f);
    EXPECT_FLOAT_EQ(ftest[2], 1.0f);
    EXPECT_FLOAT_EQ(ftest.at(0), 1.0f);
    EXPECT_FLOAT_EQ(ftest.at(1), 1.0f);
    EXPECT_FLOAT_EQ(ftest.at(2), 1.0f);

    ftest.at(1) = 2.0f;
    EXPECT_FLOAT_EQ(ftest.at(1), 2.0f);
    ftest.at(0) = 0.0f; // ftest={0.0f,2.0f,1.0f}
    EXPECT_EQ(ftest, Point3f(0.0f, 2.0f , 1.0f ));
  }
  // +-
  {
    Point3f x{1.0f, 0.0f, 0.0f};
    Point3f y = x;

//    auto test1 = x + y; //trigger error

    auto test1 = x + static_cast<Vector3f>(y);
    auto test2 = x + Vector3f(y);
    auto test3 = x - y;

    EXPECT_EQ(test1,test2);
    EXPECT_EQ(test2,Point3f(2.0f,0.0f,0.0f));
    EXPECT_EQ(test3,Vector3f(0.0f));
  }
  // max_min
  {
    Point3f x{1.0f, 0.0f, 0.0f};
    Point3f y{0.0f, 1.0f, 0.0f};

    auto t1 = Point3f::max(x, y);
    auto t2 = Point3f::min(x, y);
    EXPECT_EQ(t1, Point3f(1.0f, 1.0f, 0.0f));
    EXPECT_EQ(t2, Point3f(0.0f, 0.0f, 0.0f));
  }
}
