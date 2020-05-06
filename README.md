# `tokens_waiter`

the `tokens.waiter.h` file implement an object that can wait for multiple coroutines to finish.

this is intended to be used together with [boost asio coroutines](https://www.boost.org/doc/libs/1_73_0/doc/html/boost_asio/overview/core/spawn.html).

to build the test use:
```
g++ -DBOOST_COROUTINES_NO_DEPRECATION_WARNING -Wall -o test_tokens_waiter test_tokens_waiter.cpp  -lpthread -lboost_coroutine
```
