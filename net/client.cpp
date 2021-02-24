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
  io_context.run();

  return 0;
}
