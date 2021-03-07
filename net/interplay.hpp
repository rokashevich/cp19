#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <queue>

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
        [self = shared_from_this(), this](boost::system::error_code error,
                                          std::size_t bytes_transferred) {
          std::cout << std::istream(&self->streambuf_).rdbuf() << std::endl;
          // streambuf_.consume(bytes_transferred);
          Reply();
          Start();
        });
  }

  void Reply() {
    std::cout << "replying" << std::endl;
    boost::asio::async_write(
        socket_, io::buffer("message recieved\n"),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
          if (!ec) {
            std::cout << "replying ok\n" << std::endl;
          } else {
            socket_.close();
          }
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
        socket_(io_context),
        acceptor_(io_context, boost::asio::ip::tcp::endpoint(
                                  boost::asio::ip::tcp::v4(), port)) {}

  void AsyncAccept() {
    acceptor_.async_accept(*socket_, [&](boost::system::error_code error) {
      std::make_shared<Session>(std::move(*socket_))->Start();
      AsyncAccept();
    });
  }

 private:
  boost::asio::io_context& io_context_;
  std::optional<boost::asio::ip::tcp::socket> socket_;
  boost::asio::ip::tcp::acceptor acceptor_; 
};
////////////////////////////////////

class Client {
 public:
  Client(io::io_context& io_context, std::string const& hostname)
      : socket_(io_context), io_context_(io_context) {
    std::cout << "Address: " << hostname << "\n";
    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::address::from_string(hostname), 12345);

    DoConnect(endpoint);
  }

  void SendMessage(const std::string& msg) {
    boost::asio::post(io_context_, [this, msg]() {
      bool write_in_progress = !write_msgs_.empty();
      write_msgs_.push(msg);
      if (!write_in_progress) {
        DoWrite();
      }
    });
  }

  void Close() {
    boost::asio::post(io_context_, [this]() { socket_.close(); });
  }

 private:
  void DoConnect(const boost::asio::ip::tcp::endpoint endpoint) {
    socket_.async_connect(endpoint,
                          [this](const boost::system::error_code& ec) {
                            if (!ec) {
                              DoRead();
                            }
                          });
  }

  void DoWrite() {
    boost::asio::async_write(
        socket_,
        io::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
          if (!ec) {
            std::cout << "writing message\n";
            write_msgs_.pop();
            if (!write_msgs_.empty()) {
              DoWrite();
            }
          } else {
            socket_.close();
          }
        });
  }

  void DoRead() {
    std::cout << "reading" << std::endl;
    boost::asio::async_read_until(
        socket_, response_, '\n',
        [this](error_code error, std::size_t bytes_transferred) {
          if (!error) {
            std::cout << "Read: " << error.message()
                      << ", bytes transferred: " << bytes_transferred << "\n\n"
                      << std::istream(&response_).rdbuf() << "\n";
            DoRead();
          } else {
            socket_.close();
          }
        });
  }

  io::io_context& io_context_;
  tcp::socket socket_;
  io::streambuf response_;
  std::queue<std::string> write_msgs_;
};

}  // namespace Interplay
