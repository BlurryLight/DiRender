#include <gtest/gtest.h>
#include <math/geometry.hpp>
#include <utils/thread_pool.hpp>
using namespace DR;

TEST(UTILS, util_test) {
  // clamp
  {
    float fltmax = kFloatMax;
    float fltmin = kFloatMin;
    float fltlst = kFloatLowest;
    EXPECT_FLOAT_EQ(clamp(0.0f, 1.0f, fltmax), 1.0f);
    EXPECT_FLOAT_EQ(clamp(0.0f, 1.0f, fltmin), fltmin);
    EXPECT_FLOAT_EQ(clamp(0.0f, 1.0f, fltlst), 0.0f);
  }

  // deg2rad / rad2deg
  {
    float pi_over_6 = kPi / 6.0f;
    EXPECT_FLOAT_EQ(deg2rad(30.0f), pi_over_6);
    EXPECT_FLOAT_EQ(rad2deg(pi_over_6), 30.0f);

    EXPECT_FLOAT_EQ(deg2rad(180.0f), kPi);
    EXPECT_FLOAT_EQ(rad2deg(kPi), 180.0f);

    EXPECT_FLOAT_EQ(deg2rad(0.0f), 0.0f);
    EXPECT_FLOAT_EQ(rad2deg(0.0f), 0.0f);

    EXPECT_FLOAT_EQ(deg2rad(-180.0f), -kPi);
    EXPECT_FLOAT_EQ(rad2deg(-kPi), -180.0f);
  }
  // lerp
  {
    float fltmax = kFloatMax;
    float fltmin = kFloatMin;
    float fltlst = kFloatLowest;
    EXPECT_FLOAT_EQ(lerp(0.0f, 1.0f, fltmax), 1.0f);
    EXPECT_FLOAT_EQ(lerp(0.0f, 1.0f, fltmin), fltmin);
    EXPECT_FLOAT_EQ(lerp(0.0f, 1.0f, fltlst), 0.0f);
    EXPECT_FLOAT_EQ(lerp(0.0f, 1.0f, 0.5f), 0.5f);
  }
  // random
  {
    std::vector<std::vector<float>> buckets;
    buckets.resize(8);
    auto eval_mean_variance = [&](int index, float min, float max) -> bool {
      double mean = std::accumulate(buckets[index].cbegin(),
                                    buckets[index].cend(), 0.0f) /
                    buckets[index].size();
      double variance = 0.0f;
      for (auto i : buckets[index]) {
        variance += (i - mean) * (i - mean) / 10000;
      }
      return (std::abs(mean - (min + max) / 2) < 0.1f) &&
             (std::abs(variance - ((min - max) * (min - max)) / 12.0f) < 0.1f);
    };

    auto init_vec = [&](int index, float min, float max) {
      for (int i = 0; i < 10000; i++) {
        buckets[index].push_back(get_random_float(min, max));
      }
    };

    std::vector<std::future<void>> result1;
    ThreadPool pool;
    for (int i = 0; i < 8; i++) {
      int index = i;
      result1.push_back(pool.enqueue_task(init_vec, index, index, index + 1));
    }
    for (auto &i : result1) {
      i.wait();
    }
    // sync

    std::vector<std::future<bool>> result;
    for (int i = 0; i < 8; i++) {
      int index = i;
      auto res = pool.enqueue_task(eval_mean_variance, index, index, index + 1);
      result.push_back(std::move(res));
    }

    // variance & mean
    for (int i = 0; i < 8; i++) {
      EXPECT_TRUE(result[i].get());
    }

    //是否每个线程独立分布
    buckets.clear();
    buckets.resize(8);
    for (int i = 0; i < 8; i++) {
      int index = i;
      result1.push_back(pool.enqueue_task(init_vec, index, 0, 1));
    }
    for (auto &i : result1) {
      i.wait();
    }
    for (int i = 0; i < 8; i++) {
      for (int j = i + 1; j < 8; j++) {
        EXPECT_TRUE(buckets[i] != buckets[j]);
      }
    }
  }
}

TEST(GEOMETRY, sample_test) {
  // uniform sample on sphere
  {
    std::vector<std::pair<Vector3f, float>> result;
    for (int i = 0; i < 10000; i++) {
      result.push_back(uniform_sample_sphere());
    }
    auto pdf = 1.0f / (4 * kPi);
    for (auto &i : result) {
      EXPECT_FLOAT_EQ(i.second, pdf);
      EXPECT_TRUE(almost_equal(i.first.length(), 1.0f));
      EXPECT_TRUE(i.first.x <= 1.0 && i.first.x >= -1.0f);
      EXPECT_TRUE(i.first.y <= 1.0 && i.first.y >= -1.0f);
      EXPECT_TRUE(i.first.z <= 1.0 && i.first.z >= -1.0f);
    }
  }

  {
    std::vector<std::pair<Vector3f, float>> result;
    for (int i = 0; i < 10000; i++) {
      result.push_back(uniform_sample_hemisphere());
    }
    auto pdf = 1.0f / (2 * kPi);
    for (auto &i : result) {
      EXPECT_FLOAT_EQ(i.second, pdf);
      EXPECT_TRUE(almost_equal(i.first.length(), 1.0f));
      EXPECT_TRUE(i.first.x <= 1.0 && i.first.x >= -1.0f);
      EXPECT_TRUE(i.first.y <= 1.0 && i.first.y >= -1.0f);
      EXPECT_TRUE(i.first.z <= 1.0 && i.first.z >= 0.0f);
    }
  }

  {
    std::vector<std::pair<Vector3f, float>> result;
    for (int i = 0; i < 10000; i++) {
      result.push_back(cosine_sample_hemisphere());
    }
    for (auto &i : result) {
      EXPECT_TRUE(i.second <= 1 * k1_Pi && i.second >= 0.0f);
      EXPECT_TRUE(almost_equal(i.first.length(), 1.0f));
      EXPECT_TRUE(i.first.x <= 1.0 && i.first.x >= -1.0f);
      EXPECT_TRUE(i.first.y <= 1.0 && i.first.y >= -1.0f);
      EXPECT_TRUE(i.first.z <= 1.0 && i.first.z >= 0.0f);
    }
  }
}
