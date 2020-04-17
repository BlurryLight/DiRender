#include <gtest/gtest.h>
#include <stdio.h>

TEST(sample_test_case, sample_test) { EXPECT_EQ(1, 1); }
int main(int argc, char **argv) {
  printf("Running ALL tests from tests.cc\n");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
