#include <gtest/gtest.h>
#include <math/matrix.hpp>

using namespace DR;
TEST(MATRIX4, basic) {
  // ctor & dtor
  {
    auto mat4 = Matrix4();
    ignore(mat4);
  }

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
TEST(MATRIX4, Transform) {

  {
    auto mat = Matrix4(4);
    auto vec = vec3(1, 1, 1);
    auto mat_new = Matrix4::Translate(mat, vec);
    // clang-format off
    auto ground_truth = mat4( 4,0,0,4,
                              0,4,0,4,
                              0,0,4,4,
                              0,0,0,4);
    // clang-format on
    EXPECT_EQ(mat_new, ground_truth);
  }
  // rotation by x
  {
    auto mat_rot = Matrix4::Rotate(kPi_2, vec3(1.0, 0.0, 0.0));
    // clang-format off
      auto ground_truth = mat4( 1,0,0,0,
                                0,0,-1,0,
                                0,1,0,0,
                                0,0,0,1);
    // clang-format on
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        EXPECT_NEAR(ground_truth[i][j], mat_rot[i][j], 0.01);
      }
    }
  }

  // rotation by y
  {
    auto mat_rot = Matrix4::Rotate(kPi_2, vec3(0.0, 1.0, 0.0));
    // clang-format off
    auto ground_truth = mat4( 0,0,1,0,
                              0,1,0,0,
                              -1,0,0,0,
                              0,0,0,1);
    // clang-format on
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        EXPECT_NEAR(ground_truth[i][j], mat_rot[i][j], 0.01);
      }
    }
  }

  // rotation by z
  {
    auto mat_rot = Matrix4::Rotate(kPi_2, vec3(0.0, 0.0, 1.0));
    // clang-format off
    auto ground_truth = mat4( 0,-1,0,0,
                              1,0,0,0,
                              0,0,1,0,
                              0,0,0,1);
    // clang-format on
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        EXPECT_NEAR(ground_truth[i][j], mat_rot[i][j], 0.01);
      }
    }
  }

  // rotation by any axis
  {
    auto mat_rot = Matrix4::Rotate(kPi_2, vec3(1.0, 1.0, 1.0));
    // clang-format off
    auto ground_truth = mat4( 0.333333,-0.244017,0.910684,0,
                              0.910684,0.333333,-0.244017,0,
                              -0.244017,0.910684,0.333333,0,
                              0,0,0,1);
    // clang-format on
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        EXPECT_NEAR(ground_truth[i][j], mat_rot[i][j], 0.01);
      }
    }
  }

  // scale
  {
    auto gt = mat4(0.5);
    gt[3][3] = 1.0f;
    auto mat_scale = Matrix4::Scale(vec3(0.5f));

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        EXPECT_NEAR(gt[i][j], mat_scale[i][j], 0.01);
      }
    }
  }
}
