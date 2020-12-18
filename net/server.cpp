#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>

#include "interplay.hpp"

int main() {
  std::cout << "before start server" << std::endl;
  auto server = Interplay::Server("12345");
  std::cout << "after stared server" << std::endl;
  return 0;
}
