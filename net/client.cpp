#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#include "interplay.hpp"

int main() {
  boost::asio::io_context io_context;
  Interplay::Client client(io_context, "127.0.0.1");
  std::thread t([&io_context]() { io_context.run(); });
  //  std::string str = "fhjgfhfjh\n";
  //  client.SendMessage(str);
  //  while (true) {
  //    std::getline(std::cin, str);
  client.SendMessage("first\n");
  client.SendMessage("second\n");
  client.SendMessage("third\n");
  client.SendMessage("fourth\n");
  client.SendMessage("fifth\n");
  //  }

  t.join();

  return 0;
}
