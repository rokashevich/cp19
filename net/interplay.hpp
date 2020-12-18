#pragma once

namespace Interplay {
class Server {
 public:
  Server(const char*);
  ~Server();

 private:
  static const int backlog_ = 20;
};

class Client {
 public:
  Client(const char*);
  ~Client();

 private:
  static const int max_data_size_ = 20;
};
}  // namespace Interplay
