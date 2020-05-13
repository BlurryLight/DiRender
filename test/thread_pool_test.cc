#include "../src/utils/thread_pool.hpp"
#include <gtest/gtest.h>
#include <vector>

TEST(THREAD_POOL, RAII) {
  DR::ThreadPool pool;
  DR::ThreadPool pool1{8};
}

TEST(THREAD_POOL, add_task) {
  DR::ThreadPool pool{4};
  std::vector<std::future<int>> res;
  auto func = [](int i) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return ++i;
  };
  for (int j = 0; j < 8; j++) {
    // push 0-7
    res.push_back(pool.enqueue_task(func, j));
    // should return 1-8
  }

  int i = 1;
  for (auto &it : res) {
    EXPECT_EQ(it.get(), i++);
  }
}
