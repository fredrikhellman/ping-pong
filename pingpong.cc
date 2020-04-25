#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <functional>

class Toggle {
public:
  void wait_loop(bool to_value) {
    while(true) {
      std::unique_lock<std::mutex> lock(mx);
      cv.wait(lock, [&] {
        return value != to_value || num_toggles == 0;
      });
    
      if (num_toggles == 0) {
        break;
      }
      else {
        num_toggles--;
        value = to_value;
        cv.notify_one();
      }
    }
  }

  void spin_loop(bool to_value) {
    while(true) {
      std::unique_lock<std::mutex> lock(mx);
      if(value == to_value && num_toggles != 0) {
        continue;
      };
    
      if (num_toggles == 0) {
        break;
      }
      else {
        num_toggles--;
        value = to_value;
      }
    }
  }
  
  long long num_toggles{100'000};
  bool value{false};

private:
  std::mutex mx;
  std::condition_variable cv;
};

int main(int argc, const char *argv[])
{
  std::function<void(Toggle&, bool)> loop_function;

  auto print_usage = [&argv] { std::cout << "usage: " << argv[0] << " wait|spin" << std::endl; };
  
  if (argc != 2) {
    print_usage();
    return 1;
  }
  
  if (std::string(argv[1]) == "wait") {
    loop_function = &Toggle::wait_loop;
  }
  else if (std::string(argv[1]) == "spin") {
    loop_function = &Toggle::spin_loop;
  }
  else {
    print_usage();
    return 1;
  }

  for (int i = 0; i < 100; i++) {
    Toggle toggle;

    auto thread_0 = std::thread([&] { loop_function(toggle, false); });
    auto thread_1 = std::thread([&] { loop_function(toggle, true); });

    thread_0.join();
    thread_1.join();
  }

  return 0;
}
