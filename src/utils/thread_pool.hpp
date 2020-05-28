#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utils/di_global.h>
#include <vector>

NAMESPACE_BEGIN(DR)
class ThreadPool final {
public:
  ThreadPool();
  ThreadPool(size_t nthreads);
  size_t size() const{ return workers_.size();}
  ThreadPool(const ThreadPool &other) = delete;
  ThreadPool(ThreadPool &&other) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;
  ThreadPool &operator=(ThreadPool &&) = delete;
  template <typename F, typename... Args>
  decltype(auto) enqueue_task(F &&func, Args &&... args);
  ~ThreadPool();

private:
  std::queue<std::function<void()>> tasks_;
  std::vector<std::thread> workers_;
  std::mutex queue_mutex_;
  std::condition_variable queue_cv_;
  void init(size_t nthreads);
  bool stop_ = false;
};

inline void ThreadPool::init(size_t nthreads) {
  for (size_t i = 0; i < nthreads; i++) {
    workers_.emplace_back([this] {
      while (true) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lk(queue_mutex_);
          queue_cv_.wait(
              lk, [this] { return (this->stop_ || !this->tasks_.empty()); });
          // Be awaken, it may be going to deconstruct or just have tasks to do
          if (this->stop_ && this->tasks_.empty()) {
            // receive stop signal and no more tasks
            // thread exit
            return;
          }
          task = std::move(this->tasks_.front());
          tasks_.pop();
        }
        task();
      }
    });
  }
}

inline ThreadPool::ThreadPool() {
  size_t n = std::thread::hardware_concurrency();
  this->init(n);
}

inline ThreadPool::ThreadPool(size_t nthreads) { this->init(nthreads); }

template <typename F, typename... Args>
decltype(auto) ThreadPool::enqueue_task(F &&func, Args &&... args) {
  using res_type = typename std::result_of<F(Args...)>::type;
  auto task = std::make_shared<std::packaged_task<res_type()>>(
      std::bind(std::forward<F>(func), std::forward<Args>(args)...));
  std::future<res_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lk(queue_mutex_);
    if (!this->stop_) {
      tasks_.emplace([task]() { (*task)(); });
    }
  }
  this->queue_cv_.notify_one();
  return res;
}

inline ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lk(queue_mutex_);
    this->stop_ = true;
  }
  // notify all to stop
  queue_cv_.notify_all();
  for (auto &t : workers_) {
    t.join();
  }
}

NAMESPACE_END(DR)
