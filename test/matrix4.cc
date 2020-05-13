#include <gtest/gtest.h>
#include <math/matrix.hpp>

using namespace DR;
TEST(MATRIX4,basic)
{
  //ctor & dtor
  {
    auto mat4 = Matrix4();
  }

  {
    auto mat = Matrix4(2.0f);
    auto mat_inv = Matrix4::Inverse(mat);
    auto mat2 = Matrix4(0.5f);
    EXPECT_EQ(mat2,mat_inv);
  }

  {
    auto mat = Matrix4(0.0f);
    try {
      Matrix4::Inverse(mat);
      FAIL()<<"Expected std::runtime_error";
    }
    catch(std::runtime_error const& err)
    {
      ASSERT_STREQ(err.what(),"Matrix4 is not invertible");
    }
    catch (...) {
      FAIL()<<"Expected std::runtime_error";
    }
  }

  {
    auto mat = Matrix4(5.0f);
    auto mat_inv_true = mat4(0.2,0,0,0,
                             0,0.2,0,0,
                             0,0,0.2,0,
                             0,0,0,0.2
                             );
    auto mat_inv = mat4::Inverse(mat);
    EXPECT_EQ(mat_inv,mat_inv_true);
  }
  {
    auto mat = Matrix4(1.0f);
    auto mat_inv = Matrix4::Inverse(mat);
    EXPECT_EQ(mat_inv,mat);
//    std::cout<<mat_inv<<std::endl;
  }
}
