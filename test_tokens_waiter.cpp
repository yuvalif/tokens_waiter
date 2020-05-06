#include "tokens_waiter.h"
#include <cstdlib>
#include <iostream>

class DemoManager {
  constexpr static auto bulk_size = 20;
  boost::asio::io_context io_context;

  void process_bulk(boost::asio::yield_context yield) {
    tokens_waiter waiter(io_context);
    // loop through some potantially async work
    for (auto i = 0; i < bulk_size; ++i) {
      boost::asio::spawn(io_context, [this, i, &waiter](boost::asio::yield_context yield) {
        const auto token = waiter.make_token();
        const auto duration = rand() % 10 + 1;
        if (duration == 7) {
          // this is to demonstrate the RAII nature of the token
          std::cout << "no processing of: " << i << std::endl;
          return;
        }
        std::cout << "start async processing of: " << i << std::endl;
        boost::asio::deadline_timer timer(io_context);
        timer.expires_from_now(boost::posix_time::seconds(duration));
        boost::system::error_code ec;
        timer.async_wait(yield[ec]);
        std::cout << "finished async processing of: " << i << std::endl;
      });
    }

    // wait for work to finish
    std::cout << "wait for processing of: " << bulk_size << " items to finish" << std::endl;
    waiter.async_wait(yield);

    // schedule itself again
    std::cout << "processing of: " << bulk_size << " items finished. start again..." << std::endl;
    boost::asio::spawn(io_context, [this](boost::asio::yield_context yield) {
        process_bulk(yield);
    });
  }

public:
  DemoManager() {
    boost::asio::spawn(io_context, [this](boost::asio::yield_context yield) {
      process_bulk(yield);
    });
    io_context.run();
  }

  ~DemoManager() {
    io_context.stop();
  }
};

int main() {
  DemoManager manager;

  return 0;
}
