#include <gtest/gtest.h>
#include <math/matrix.hpp>

using namespace DR;
TEST(MATRIX4, basic) {
  // ctor & dtor
  { auto mat4 = Matrix4();ignore(mat4); }

  {
    auto mat = Matrix4(2.0f);
    auto mat_inv = Matrix4::Inverse(mat);
    auto mat2 = Matrix4(0.5f);
    EXPECT_EQ(mat2, mat_inv);
  }

  {
    auto mat = Matrix4(0.0f);
    try {
      Matrix4::Inverse(mat);
      FAIL() << "Expected std::runtime_error";
    } catch (std::runtime_error const &err) {
      ASSERT_STREQ(err.what(), "Matrix4 is not invertible");
    } catch (...) {
      FAIL() << "Expected std::runtime_error";
    }
  }

  {
    auto mat = Matrix4(5.0f);
    auto mat_inv_true =
        mat4(0.2, 0, 0, 0, 0, 0.2, 0, 0, 0, 0, 0.2, 0, 0, 0, 0, 0.2);
    auto mat_inv = mat4::Inverse(mat);
    EXPECT_EQ(mat_inv, mat_inv_true);
  }
  {
    auto mat = Matrix4(1.0f);
    auto mat_inv = Matrix4::Inverse(mat);
    EXPECT_EQ(mat_inv, mat);
    //    std::cout<<mat_inv<<std::endl;
  }

  {
    // clang-format off
    auto mat = mat4(         1,2,3,4,
                             0,1,0,0,
                             0,0,1,0,
                             1,0,0,1
                             );
    auto mat_t = mat4(       1,0,0,1,
                             2,1,0,0,
                             3,0,1,0,
                             4,0,0,1
                             );
    // clang-format on
    EXPECT_EQ(mat_t, Matrix4::Transpose(mat));
    EXPECT_EQ(mat_t, mat.transpose());
  }

  {
    // clang-format off
    auto scale3 = mat4(3.0f);
    auto mat = mat4(         1,0,0,1,
                             2,1,0,0,
                             3,0,1,0,
                             4,0,0,1
                             );
    auto mat_scaled = mat4(  3,0,0,3,
                             6,3,0,0,
                             9,0,3,0,
                             12,0,0,3
                             );
    // clang-format on
    EXPECT_EQ(mat_scaled, Matrix4::Multiply(scale3, mat));
    EXPECT_EQ(mat_scaled, scale3.multiply(mat));
  }
  // inverse

  {
    // clang-format off
    auto mat = mat4(         1,0,0,1,
                             0,1,0,2,
                             0,0,1,3,
                             0,0,0,1
                             );
    auto mat_inv = mat4(     1,0,0,-1,
                             0,1,0,-2,
                             0,0,1,-3,
                             0,0,0,1
                             );
    // clang-format on
    EXPECT_EQ(mat_inv, Matrix4::Inverse(mat));
  }
}
