#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <iostream>

#include "interplay.hpp"

int main() {
  Interplay::Client client;
  std::string str = "Connected\n";
  client.Connect("127.0.0.1", 12345);
  client.SendMessage(str);
  while (true) {
    std::getline(std::cin, str);
    client.SendMessage(str + '\n');
  }

  return 0;
}
