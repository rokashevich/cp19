#pragma once
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <iostream>

#define ASIO_STANDALONE

namespace Interplay {

class session : public std::enable_shared_from_this<session> {
 public:
  session(boost::asio::ip::tcp::socket&& socket) : socket_(std::move(socket)) {}

  void start() {
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

class server {
 public:
  server(boost::asio::io_context& io_context, std::uint16_t port)
      : io_context_(io_context),
        acceptor_(io_context, boost::asio::ip::tcp::endpoint(
                                  boost::asio::ip::tcp::v4(), port)) {}

  void async_accept() {
    socket_.emplace(io_context_);

    acceptor_.async_accept(*socket_, [&](boost::system::error_code error) {
      std::make_shared<session>(std::move(*socket_))->start();
      async_accept();
    });
  }

 private:
  boost::asio::io_context& io_context_;
  boost::asio::ip::tcp::acceptor acceptor_;
  std::optional<boost::asio::ip::tcp::socket> socket_;
};
////////////////////////////////////

}  // namespace Interplay
