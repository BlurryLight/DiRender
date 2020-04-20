#include <math/vector.hpp>
#include <gtest/gtest.h>
using namespace DR;
TEST(VECTOR,vector2)
{
  //ctor,dtor
  {
    Vector2f test;
    EXPECT_FLOAT_EQ(test.x,0.0f);
    EXPECT_FLOAT_EQ(test.y,0.0f);
    EXPECT_FLOAT_EQ(test.u,0.0f);
    EXPECT_FLOAT_EQ(test.v,0.0f);
  }
  //nan
  {
    Vector2f ftest{std::numeric_limits<float>::quiet_NaN(),0.0f};
    Vector2f ftest1;
    EXPECT_TRUE(ftest.has_nan());
    EXPECT_FALSE(ftest1.has_nan());
  }
  //operators
  {
    //float
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
  //other
  {
    Vector2f ftest{1.0f};
    EXPECT_FLOAT_EQ(ftest.length(),M_SQRT2);
    EXPECT_FLOAT_EQ(ftest.squared_length(),M_SQRT2 * M_SQRT2);

    EXPECT_FLOAT_EQ(ftest[0],1.0f);
    EXPECT_FLOAT_EQ(ftest[1],1.0f);
    EXPECT_FLOAT_EQ(ftest.at(0),1.0f);
    EXPECT_FLOAT_EQ(ftest.at(1),1.0f);

    ftest.at(1) = 2.0f;
    EXPECT_FLOAT_EQ(ftest.at(1),2.0f);
    ftest.at(0) = 0.0f;//ftest={0.0f,2.0f}
    ftest = ftest.normalize();
    EXPECT_EQ(ftest,Vector2f(0.0f,1.0f));
  }


}
