/*!
 * @file  TimeoutBlockingClient.cpp
 * @brief Network access client using Boost.Asio
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * @date  2014-02-13
 */
#include "TimeoutBlockingClient.h"

#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>


namespace openrtm_network_camera {
namespace utility {

/*!
 * @brief コンストラクタ
 *
 * @caution
 * タイムアウト処理用にタイマーを初期化する
 */
TimeoutBlockingClient::TimeoutBlockingClient()
    : io_service_(),
      socket_(io_service_),
      deadline_(io_service_),
      input_buffer_()
{
  // No deadline is required until the first socket operation is started. We
  // set the deadline to positive infinity so that the actor takes no action
  // until a specific deadline is set.
  deadline_.expires_at(boost::posix_time::pos_infin);

  // Start the persistent actor that checks for deadline expiry.
  check_deadline();
}

TimeoutBlockingClient::~TimeoutBlockingClient()
{
}

void TimeoutBlockingClient::connect(const std::string& host, const std::string& service, boost::posix_time::time_duration timeout)
{
  using boost::asio::ip::tcp;
  using boost::lambda::var;
  using boost::lambda::_1;

  // バッファのクリア
  input_buffer_.consume(input_buffer_.size());

  // Resolve the host name and service to a list of endpoints.
  tcp::resolver::query query(host, service);
  tcp::resolver::iterator iter = tcp::resolver(io_service_).resolve(query);

  // Set a deadline for the asynchronous operation. As a host name may
  // resolve to multiple endpoints, this function uses the composed operation
  // async_connect. The deadline applies to the entire operation, rather than
  // individual connection attempts.
  deadline_.expires_from_now(timeout);

  // Set up the variable that receives the result of the asynchronous
  // operation. The error code is set to would_block to signal that the
  // operation is incomplete. Asio guarantees that its asynchronous
  // operations will never fail with would_block, so any other value in
  // ec indicates completion.
  boost::system::error_code ec = boost::asio::error::would_block;

  // Start the asynchronous operation itself. The boost::lambda function
  // object is used as a callback and will update the ec variable when the
  // operation completes. The blocking_udp_client.cpp example shows how you
  // can use boost::bind rather than boost::lambda.
  boost::asio::async_connect(socket_, iter, var(ec) = _1);

  // Block until the asynchronous operation has completed.
  do {
    io_service_.run_one();
  } while (ec == boost::asio::error::would_block);

  // Determine whether a connection was successfully established. The
  // deadline actor may have had a chance to run and close our socket, even
  // though the connect operation notionally succeeded. Therefore we must
  // check whether the socket is still open before deciding if we succeeded
  // or failed.
  if (ec || !socket_.is_open()) {
    std::cout << "TimeoutBlockingClient::connect, Exception: " << ec << std::endl;
    throw boost::system::system_error(
        ec ? ec : boost::asio::error::operation_aborted);
  }
}

size_t TimeoutBlockingClient::read(boost::posix_time::time_duration timeout)
{
  using boost::lambda::var;
  using boost::lambda::_1;
  using boost::lambda::_2;

  // Set a deadline for the asynchronous operation. Since this function uses
  // a composed operation (async_read_until), the deadline applies to the
  // entire operation, rather than individual reads from the socket.
  deadline_.expires_from_now(timeout);

  // Set up the variable that receives the result of the asynchronous
  // operation. The error code is set to would_block to signal that the
  // operation is incomplete. Asio guarantees that its asynchronous
  // operations will never fail with would_block, so any other value in
  // ec indicates completion.
  boost::system::error_code ec = boost::asio::error::would_block;

  // Start the asynchronous operation itself. The boost::lambda function
  // object is used as a callback and will update the ec variable when the
  // operation completes. The blocking_udp_client.cpp example shows how you
  // can use boost::bind rather than boost::lambda.
  size_t length;
  boost::asio::async_read(socket_, input_buffer_, boost::asio::transfer_all(), (var(ec) = _1, var(length) = _2) );

  // Block until the asynchronous operation has completed.
  do {
    io_service_.run_one();
  } while (ec == boost::asio::error::would_block);

  // すべてのデータを読み込んだときは、eofが返るのでエラーとはしない
  if (ec && ec != boost::asio::error::eof) {
    std::cout << "TimeoutBlockingClient::read, Exception: " << ec << std::endl;
    throw boost::system::system_error(ec);
  }
  return length;
}
size_t TimeoutBlockingClient::read_until(const std::string& delimiter, boost::posix_time::time_duration timeout)
{
  using boost::lambda::var;
  using boost::lambda::_1;
  using boost::lambda::_2;

  // Set a deadline for the asynchronous operation. Since this function uses
  // a composed operation (async_read_until), the deadline applies to the
  // entire operation, rather than individual reads from the socket.
  deadline_.expires_from_now(timeout);

  // Set up the variable that receives the result of the asynchronous
  // operation. The error code is set to would_block to signal that the
  // operation is incomplete. Asio guarantees that its asynchronous
  // operations will never fail with would_block, so any other value in
  // ec indicates completion.
  boost::system::error_code ec = boost::asio::error::would_block;

  // Start the asynchronous operation itself. The boost::lambda function
  // object is used as a callback and will update the ec variable when the
  // operation completes. The blocking_udp_client.cpp example shows how you
  // can use boost::bind rather than boost::lambda.
  size_t length;
  boost::asio::async_read_until(socket_, input_buffer_, delimiter, (var(ec) = _1, var(length) = _2) );

  // Block until the asynchronous operation has completed.
  do {
    io_service_.run_one();
  } while (ec == boost::asio::error::would_block);

  if (ec) {
    std::cout << "TimeoutBlockingClient::read_until, Exception: " << ec << std::endl;
    throw boost::system::system_error(ec);
  }
  return length;
}

void TimeoutBlockingClient::write(boost::asio::streambuf& buf, boost::posix_time::time_duration timeout)
{
  using boost::lambda::var;
  using boost::lambda::_1;

  // Set a deadline for the asynchronous operation. Since this function uses
  // a composed operation (async_write), the deadline applies to the entire
  // operation, rather than individual writes to the socket.
  deadline_.expires_from_now(timeout);

  // Set up the variable that receives the result of the asynchronous
  // operation. The error code is set to would_block to signal that the
  // operation is incomplete. Asio guarantees that its asynchronous
  // operations will never fail with would_block, so any other value in
  // ec indicates completion.
  boost::system::error_code ec = boost::asio::error::would_block;

  // Start the asynchronous operation itself. The boost::lambda function
  // object is used as a callback and will update the ec variable when the
  // operation completes. The blocking_udp_client.cpp example shows how you
  // can use boost::bind rather than boost::lambda.
  boost::asio::async_write(socket_, buf, var(ec) = _1);

  // Block until the asynchronous operation has completed.
  do {
    io_service_.run_one();
  } while (ec == boost::asio::error::would_block);

  if (ec) {
    std::cout << "TimeoutBlockingClient::write, Exception: " << ec << std::endl;
    throw boost::system::system_error(ec);
  }
}

void TimeoutBlockingClient::check_deadline()
{
  // Check whether the deadline has passed. We compare the deadline against
  // the current time since a new asynchronous operation may have moved the
  // deadline before this actor had a chance to run.
  if (deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())
  {
    // The deadline has passed. The socket is closed so that any outstanding
    // asynchronous operations are cancelled. This allows the blocked
    // connect(), read_line() or write_line() functions to return.
    boost::system::error_code ignored_ec;
    socket_.close(ignored_ec);

    std::cout << "TimeoutBlockingClient, request is timeout\n";

    // There is no longer an active deadline. The expiry is set to positive
    // infinity so that the actor takes no action until a new deadline is set.
    deadline_.expires_at(boost::posix_time::pos_infin);
  }

  // Put the actor back to sleep.
  deadline_.async_wait(boost::lambda::bind(&TimeoutBlockingClient::check_deadline, this));
}

} // utility
} // openrtm_network_camera
