#pragma once

#include <boost/asio.hpp>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>

#define ASIO_STANDALONE

namespace Interplay {

namespace asio = boost::asio;

// Queue
class NetQueue {
 public:
  NetQueue() = default;
  NetQueue(const NetQueue&) = delete;
  virtual ~NetQueue() { Clear(); }

 public:
  // Returns and maintains item at front of Queue
  const std::string& Front() {
    std::lock_guard<std::mutex> lock(mutex_queue_);
    return deq_queue.front();
  }

  // Returns and maintains item at back of Queue
  const std::string& Back() {
    std::lock_guard<std::mutex> lock(mutex_queue_);
    return deq_queue.back();
  }

  // Removes and returns item from front of Queue
  std::string PopFront() {
    std::lock_guard<std::mutex> lock(mutex_queue_);
    auto t = std::move(deq_queue.front());
    deq_queue.pop_front();
    return t;
  }

  // Removes and returns item from back of Queue
  std::string PopBack() {
    std::lock_guard<std::mutex> lock(mutex_queue_);
    auto t = std::move(deq_queue.back());
    deq_queue.pop_back();
    return t;
  }

  // Adds an item to back of Queue
  void PushBack(const std::string& item) {
    std::lock_guard<std::mutex> lock(mutex_queue_);
    deq_queue.emplace_back(std::move(item));

    std::unique_lock<std::mutex> ul(mutex_blocking_);
    cv_blocking_.notify_one();
  }

  // Adds an item to front of Queue
  void PushFront(const std::string& item) {
    std::lock_guard<std::mutex> lock(mutex_queue_);
    deq_queue.emplace_front(std::move(item));

    std::unique_lock<std::mutex> ul(mutex_blocking_);
    cv_blocking_.notify_one();
  }

  // Returns true if Queue has no items
  bool Empty() {
    std::lock_guard<std::mutex> lock(mutex_queue_);
    return deq_queue.empty();
  }

  // Returns number of items in Queue
  size_t Size() {
    std::lock_guard<std::mutex> lock(mutex_queue_);
    return deq_queue.size();
  }

  // Clears Queue
  void Clear() {
    std::lock_guard<std::mutex> lock(mutex_queue_);
    deq_queue.clear();
  }

  void Wait() {
    while (Empty()) {
      std::unique_lock<std::mutex> ul(mutex_blocking_);
      cv_blocking_.wait(ul);
    }
  }

 protected:
  std::mutex mutex_queue_;
  std::deque<std::string> deq_queue;
  std::condition_variable cv_blocking_;
  std::mutex mutex_blocking_;
};

class Connection : public std::enable_shared_from_this<Connection> {
 public:
  enum class Owner { Server, Client };

  Connection(Owner parent, asio::io_context& io_context,
             asio::ip::tcp::socket socket, NetQueue& qIn)
      : io_context_(io_context),
        socket_(std::move(socket)),
        message_queue_in_(qIn) {
    owner_ = parent;
  }

  void ConnectToClient() {
    if (owner_ == Owner::Server) {
      if (socket_.is_open()) {
        Read();
      }
    }
  }

  void ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints) {
    if (owner_ == Owner::Client) {
      // Request asio attempts to connect to an endpoint
      asio::async_connect(
          socket_, endpoints,
          [this](std::error_code ec, asio::ip::tcp::endpoint endpoint) {
            if (!ec) {
              Read();
            }
          });
    }
  }

  void Disconnect() {
    if (IsConnected()) asio::post(io_context_, [this]() { socket_.close(); });
  }

  bool IsConnected() const { return socket_.is_open(); }

  void Send(const std::string& msg) {
    asio::post(io_context_, [this, msg]() {
      // If no messages were available to be written, then start the process of
      // writing the message at the front of the queue.
      bool write_in_progress = !message_queue_out_.Empty();
      message_queue_out_.PushBack(msg);
      if (!write_in_progress) {
        Write();
      }
    });
  }

 private:
  void Read() {
    asio::async_read_until(
        socket_, streambuf_, '\n',
        [this](std::error_code ec, std::size_t length) {
          std::cout << "[SERVER] Read from " << socket_.remote_endpoint()
                    << std::endl;
          if (!ec) {
            if (streambuf_.size()) {
              std::string message((std::istreambuf_iterator<char>(&streambuf_)),
                                  std::istreambuf_iterator<char>());

              message.erase(std::find_if(message.rbegin(), message.rend(),
                                         [](unsigned char ch) {
                                           return !std::isspace(ch);
                                         })
                                .base(),
                            message.end());

              // std::cout << "[SERVER] received message \"" << message << "\""
              //<< std::endl;
              message_queue_in_.PushBack(
                  socket_.remote_endpoint().address().to_string() + ' ' +
                  message);
              streambuf_.consume(length);
              Read();
            }
          } else {
            std::cout << "Read Fail.\n";
            socket_.close();
          }
        });
  }

  void Write() {
    asio::async_write(socket_, asio::buffer(message_queue_out_.Front()),
                      [this](std::error_code ec, std::size_t length) {
                        if (!ec) {
                          // Sending was successful, so remove it from the queue
                          message_queue_out_.PopFront();

                          // If the queue still has messages in it, then send
                          // the next message.
                          if (!message_queue_out_.Empty()) {
                            Write();
                          }
                        } else {
                          std::cout << "Write Fail.\n";
                          socket_.close();
                        }
                      });
  }

 private:
  asio::io_context& io_context_;
  asio::ip::tcp::socket socket_;
  std::string message_buf_;
  NetQueue& message_queue_in_;
  NetQueue message_queue_out_;
  asio::streambuf streambuf_;
  Owner owner_;
};

class Server {
 public:
  Server(std::uint16_t port)
      : acceptor_(io_context_,
                  asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}

  ~Server() { Stop(); }

  bool Start() {
    try {
      WaitForClientConnection();

      // Launch the context in a separate thread
      thread_context_ = std::thread([this]() { io_context_.run(); });
    } catch (std::exception& e) {
      // Something prohibited the server from listening
      std::cerr << "[SERVER] Exception: " << e.what() << "\n";
      return false;
    }

    std::cout << "[SERVER] Started!\n";
    return true;
  }

  void Stop() {
    io_context_.stop();

    if (thread_context_.joinable()) thread_context_.join();

    std::cout << "[SERVER] Stopped!\n";
  }

  void WaitForClientConnection() {
    acceptor_.async_accept([this](std::error_code ec,
                                  asio::ip::tcp::socket socket) {
      // Triggered by incoming connection request
      if (!ec) {
        std::cout << "[SERVER] New Connection: " << socket.remote_endpoint()
                  << "\n";

        // Create a new connection to handle this client
        std::shared_ptr<Connection> new_conn =
            std::make_shared<Connection>(Connection::Owner::Server, io_context_,
                                         std::move(socket), message_queue_in_);

        connections_deq_.push_back(std::move(new_conn));

        connections_deq_.back()->ConnectToClient();

        std::cout << "[SERVER] Connection Approved\n";
      } else {
        std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
      }

      // Wait for another connection
      WaitForClientConnection();
    });
  }

  void Update(size_t nMaxMessages = -1, bool bWait = false) {
    if (bWait) message_queue_in_.Wait();

    // Process as many messages as you can up to the value
    // specified
    size_t message_count = 0;
    while (message_count < nMaxMessages && !message_queue_in_.Empty()) {
      // Grab the front message
      auto msg = message_queue_in_.PopFront();

      // Pass to message handler
      OnMessage(msg);

      message_count++;
    }
  }

 protected:
  void OnMessage(/*std::shared_ptr<Connection> client, */ std::string& msg) {
    std::cout << "[SERVER] Received message \"" << msg << "\"" << std::endl;

    std::time_t result = std::time(nullptr);
    std::ofstream log_file;
    log_file.open("log.txt", std::ios_base::app);
    auto time = std::localtime(&result);
    log_file << std::setfill('0') << std::setw(2) << time->tm_mday << "/"
             << std::setfill('0') << std::setw(2) << time->tm_mon + 1 << "/"
             << time->tm_year + 1900 << ' ' << std::setfill('0') << std::setw(2)
             << time->tm_hour << ":" << std::setfill('0') << std::setw(2)
             << time->tm_min << ":" << std::setfill('0') << std::setw(2)
             << time->tm_sec << ' ';
    log_file << msg << std::endl;
    log_file.close();
  }

 private:
  boost::asio::io_context io_context_;
  boost::asio::ip::tcp::acceptor acceptor_;
  std::thread thread_context_;
  NetQueue message_queue_in_;
  std::deque<std::shared_ptr<Connection>> connections_deq_;
  std::condition_variable cv_blocking;
  std::mutex mux_blocking;
};

//////////////////////////////////////
class Client {
 public:
  Client() { Connect("127.0.0.1", 12345); }

  ~Client() { Disconnect(); }

  bool Connect(const std::string& hostname, const uint16_t port) {
    try {
      // Resolve hostname/ip-address into tangiable physical address
      asio::ip::tcp::resolver resolver(io_context_);
      asio::ip::tcp::resolver::results_type endpoints =
          resolver.resolve(hostname, std::to_string(port));

      // Create connection
      connection_ = std::make_unique<Connection>(
          Connection::Owner::Client, io_context_,
          asio::ip::tcp::socket(io_context_), message_queue_in_);

      // Tell the connection object to connect to server
      connection_->ConnectToServer(endpoints);

      // Start Context Thread
      thread_context_ = std::thread([this]() { io_context_.run(); });
    } catch (std::exception& e) {
      std::cerr << "Client Exception: " << e.what() << "\n";
      return false;
    }
    return true;
  }

  void Disconnect() {
    if (IsConnected()) {
      connection_->Disconnect();
    }

    // Cleaning up
    io_context_.stop();

    if (thread_context_.joinable()) thread_context_.join();

    connection_.release();
  }

  bool IsConnected() {
    if (connection_)
      return connection_->IsConnected();
    else
      return false;
  }

  void SendMessage(const std::string& msg) {
    if (IsConnected()) connection_->Send(msg);
  }

 protected:
  asio::io_context io_context_;
  std::thread thread_context_;
  std::unique_ptr<Connection> connection_;

 private:
  // This is the thread safe queue of incoming messages from server
  NetQueue message_queue_in_;
};

class SingletonClient {
 public:
  static Client& Instance() {
    static Client single_instance;
    return single_instance;
  }

 private:
  SingletonClient() {}
  SingletonClient(const SingletonClient& root) = delete;
  SingletonClient& operator=(const SingletonClient&) = delete;
};

}  // namespace Interplay
