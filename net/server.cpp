#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

#include "interplay.hpp"

int main() {
  Interplay::Server srv(12345);
  srv.Start();

  while (1) {
    srv.Update(-1, true);
  }

  return 0;
}
