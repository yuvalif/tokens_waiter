#pragma once

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/spawn.hpp>

class tokens_waiter {
  const boost::posix_time::time_duration infinite_duration = boost::posix_time::hours(1000);
  size_t pending_tokens = 0;
  boost::asio::deadline_timer timer;
 
  struct token {
    tokens_waiter& waiter;
    token(tokens_waiter& _waiter) : waiter(_waiter){
      ++waiter.pending_tokens;
    }
  
    ~token() {
      --waiter.pending_tokens;
      if (waiter.pending_tokens == 0) {
        waiter.timer.cancel();
      }
    }
 };
  
 public:

   tokens_waiter(boost::asio::io_context& io_context) : timer(io_context) {} 
 
   void async_wait(boost::asio::yield_context yield) { 
     timer.expires_from_now(infinite_duration);
     boost::system::error_code ec;
     timer.async_wait(yield[ec]);
     // TODO: make sure that the timer was cancelled and not rexpired
   }
 
   token make_token() {    
    return token(*this);
   }
};
