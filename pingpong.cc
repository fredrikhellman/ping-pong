#include <atomic>
#include <condition_variable>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

const long long start_value = 99'999; // will yield 100'000 decrements
const int num_runs = 10;

class Toggle {
public:
  virtual ~Toggle() = default;
  virtual void loop(int num_threads, int my_remainder) = 0;
  virtual void validate_and_reset() = 0;
};

class ToggleWait : public Toggle {
public:
  void loop(int num_threads, int my_remainder) override {
    while (true) {
      std::unique_lock<std::mutex> lock(mx);
      if (value < my_remainder) {
        break;
      }

      cv.wait(lock, [&] { return value % num_threads == my_remainder; });

      value--;
      cv.notify_all();
    }
  }

  void validate_and_reset() override {
    if (value != -1) {
      throw std::runtime_error("toggle didn't work properly.");
    }

    value = start_value;
  }

private:
  long long value{start_value};
  std::mutex mx;
  std::condition_variable cv;
};

class ToggleSpin : public Toggle {
public:
  void loop(int num_threads, int my_remainder) override {
    {
      std::unique_lock<std::mutex> lock(mx);
      if (value < my_remainder) {
        return;
      }
    }

    while (true) {
      std::unique_lock<std::mutex> lock(mx);
      if (value % num_threads != my_remainder) {
        continue;
      }

      value--;

      if (value < my_remainder) {
        break;
      }
    }
  }

  void validate_and_reset() override {
    if (value != -1) {
      throw std::runtime_error("toggle didn't work properly.");
    }

    value = start_value;
  }

private:
  long long value{start_value};
  std::mutex mx;
};

class ToggleAtomic : public Toggle {
public:
  void loop(int num_threads, int my_remainder) override {
    while (value >= my_remainder) {
      while (value % num_threads != my_remainder) {
      };
      value--;
    }
  }

  void validate_and_reset() override {
    if (value != -1) {
      throw std::runtime_error("toggle didn't work properly.");
    }

    value = start_value;
  }

private:
  std::atomic<long long> value{start_value};
};

int main(int argc, const char *argv[]) {
  try {
    if (argc != 3) {
      throw std::runtime_error("requires two arguments");
    }
    auto toggle_of_mode =
        [](const std::string &mode) -> std::unique_ptr<Toggle> {
      if (mode == "wait") {
        return std::make_unique<ToggleWait>();
      } else if (mode == "spin") {
        return std::make_unique<ToggleSpin>();
      } else if (mode == "atomic") {
        return std::make_unique<ToggleAtomic>();
      } else {
        throw std::runtime_error("mode must be one of wait|spin|atomic");
      }
    };

    const int num_threads = std::stoi(argv[2]);

    auto toggle = toggle_of_mode(argv[1]);

    for (int i = 0; i < num_runs; i++) {
      {
        std::vector<std::future<void>> futures;
        for (int j = 0; j < num_threads; j++) {
          futures.push_back(std::async(
              std::launch::async, [&, j] { toggle->loop(num_threads, j); }));
        }
      }

      toggle->validate_and_reset();
    }

  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    std::cout << "usage: " << argv[0] << " wait|spin|atomic num_threads"
              << std::endl;
    return 1;
  }

  return 0;
}
