#pragma once

#include <boost/asio.hpp>
#include <iostream>

#define ASIO_STANDALONE

namespace Interplay {

namespace io = boost::asio;
namespace ip = io::ip;
using tcp = ip::tcp;
using error_code = boost::system::error_code;

class Session : public std::enable_shared_from_this<Session> {
 public:
  Session(boost::asio::ip::tcp::socket&& socket) : socket_(std::move(socket)) {}

  void Start() {
    boost::asio::async_read_until(
        socket_, streambuf_, '\n',
        [self = shared_from_this()](boost::system::error_code error,
                                    std::size_t bytes_transferred) {
          std::cout << std::istream(&self->streambuf_).rdbuf();
        });
  }

 private:
  boost::asio::ip::tcp::socket socket_;
  boost::asio::streambuf streambuf_;
};

class Server {
 public:
  Server(boost::asio::io_context& io_context, std::uint16_t port)
      : io_context_(io_context),
        acceptor_(io_context, boost::asio::ip::tcp::endpoint(
                                  boost::asio::ip::tcp::v4(), port)) {}

  void async_accept() {
    socket_.emplace(io_context_);

    acceptor_.async_accept(*socket_, [&](boost::system::error_code error) {
      std::make_shared<Session>(std::move(*socket_))->Start();
      async_accept();
    });
  }

 private:
  boost::asio::io_context& io_context_;
  boost::asio::ip::tcp::acceptor acceptor_;
  std::optional<boost::asio::ip::tcp::socket> socket_;
};
////////////////////////////////////

class Client {
 public:
  Client(io::io_context& io_context, std::string const& hostname)
      : socket_(io_context) {
    message_ = "Initialization of client\n";

    std::cout << "Address: " << hostname << "\n";
    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::address::from_string(hostname), 12345);
    socket_.async_connect(
        endpoint, std::bind(&Client::on_connect, this, std::placeholders::_1));
  }

  void SendMessage(const std::string& msg) { message_ = msg; }

 private:
  void on_connect(const boost::system::error_code& error) {
    std::cout << "Connect: " << error.message() << "\n";
    io::async_write(socket_, io::buffer(message_),
                    std::bind(&Client::on_write, this, std::placeholders::_1,
                              std::placeholders::_2));
  }

  void on_write(error_code error, std::size_t bytes_transferred) {
    std::cout << "Write: " << error.message()
              << ", bytes transferred: " << bytes_transferred << "\n";
    io::async_read(socket_, response_,
                   std::bind(&Client::on_read, this, std::placeholders::_1,
                             std::placeholders::_2));
  }

  void on_read(error_code error, std::size_t bytes_transferred) {
    std::cout << "Read: " << error.message()
              << ", bytes transferred: " << bytes_transferred << "\n\n"
              << std::istream(&response_).rdbuf() << "\n";
  }

  tcp::socket socket_;
  std::string message_;
  io::streambuf response_;
};

}  // namespace Interplay
