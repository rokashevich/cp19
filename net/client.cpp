#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <iostream>

#include "interplay.hpp"

int main() {
  boost::asio::io_context io_context;
  Interplay::Client client;
  // std::thread t([&io_context]() { io_context.run(); });
  std::string str = "fhjgfhfjh\n";
  client.Connect("127.0.0.1", 12345);
  client.SendMessage(str);
  while (true) {
    std::getline(std::cin, str);
    client.SendMessage(str + '\n');
  }

  // t.join();

  return 0;
}
