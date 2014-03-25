/*!
 * @file  HttpClientConcrete.cpp
 * @brief Http client using Boost.Asio
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * @date  2014-02-06
 */
#include "HttpClientConcrete.h"

#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <string>

#include <boost/asio.hpp>

#include "TimeoutBlockingClient.h"
#include "string_utility.h"


namespace openrtm_network_camera {
namespace utility {

// 定数定義
namespace {
  const char * CRLF = "\r\n";
  const char * CRLF2 = "\r\n\r\n";

  // ヘッダ名は小文字で処理する
  const std::string ContentTypeHeader = "content-type";
  const std::string ContentLengthHeader = "content-length";
}

HttpClientConcrete::HttpClientConcrete(void)
    : timeout_(DEFAULT_TIMEOUT_SEC),
      user_(""),
      password_(""),
      status_code_(ERROR_CODE),
      headers_(),
      content_type_(""),
      content_length_(0),
      contents_(NULL) {
}

HttpClientConcrete::~HttpClientConcrete(void) {
  if (contents_ != NULL) {
    delete [] contents_;
    contents_ = NULL;
  }
}

void HttpClientConcrete::doGet(const std::string& host_name, const std::string& path_name, const std::string& port) {
  using boost::asio::ip::tcp;

  // レスポンス用のメンバー変数の初期化
  response_member_init();

  try
  {
    // タイムアウト付き同期処理TCPクライアント
    TimeoutBlockingClient client;

    // 接続
    client.connect(host_name, port, boost::posix_time::seconds(timeout_));

    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET " << path_name << " HTTP/1.0" << CRLF;
    request_stream << "Host: " << host_name << ":" << port << CRLF;
    request_stream << "Accept: */*" << CRLF;
    request_stream << "User-Agent: " << "OpenRTM-NetworkCamera-HttpClient" << CRLF;

    // Basic認証
    if (0 != user_.size() && 0 != password_.size()) {
      std::string auth = base64encode(user_ + ":" + password_);
      request_stream << "Authorization: Basic " << auth << CRLF;
    }

    request_stream << "Connection: close" << CRLF << CRLF;

    // Send the request.
    client.write(request, boost::posix_time::seconds(timeout_));

    // Read the response status line. The response streambuf will automatically
    // grow to accommodate the entire line. The growth may be limited by passing
    // a maximum size to the streambuf constructor.
    client.read_until(CRLF, boost::posix_time::seconds(timeout_));
    boost::asio::streambuf& response = client.getStreambuf();

    // Check that response is OK.
    std::istream response_stream(&response);
    std::string http_version;
    response_stream >> http_version;
    response_stream >> status_code_;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
      std::cout << "Invalid response\n";
      status_code_ = ERROR_CODE;
      return;
    }
    if ((status_code_ != 200) && (status_code_ != 204))
    {
      std::cout << "Response returned with status code " << status_code_ << "\n";
      // ステータスコード OKまたはNo Content 以外は、ヘッダも解析しない
      return;
    }

    // ヘッダの解析
    processHeaders(client);

    setContentType();
    setContentLength();

    // コンテンツの取得
    processContents(client);
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << "\n";
    status_code_ = ERROR_CODE;
  }
}

void HttpClientConcrete::setBasicAuthenticationParameter(const std::string& user, const std::string& password) {
  user_ = user;
  password_ = password;
}

void HttpClientConcrete::setTimeout(const long timeout) {
  // ０以下の場合はデフォルト値とする
  if (0 < timeout) {
    timeout_ = timeout;
  } else {
    timeout_ = DEFAULT_TIMEOUT_SEC;
  }
}

void HttpClientConcrete::response_member_init() {
  status_code_ = ERROR_CODE;
  headers_.clear();
  content_type_.clear();
  content_length_ = 0;
  if (contents_ != NULL) {
    delete [] contents_;
    contents_ = NULL;
  }
}

void HttpClientConcrete::processHeaders(TimeoutBlockingClient& client) {
  headers_.clear();

  // Read the response headers, which are terminated by a blank line.
  client.read_until(CRLF2, boost::posix_time::seconds(timeout_));
  boost::asio::streambuf& response = client.getStreambuf();
  std::istream response_stream(&response);

  // Process the response headers.
  std::string header;
  while (std::getline(response_stream, header) && header != "\r") {
    std::string h = header;

    std::string::size_type pos = h.find_last_of("\r");
    if (std::string::npos != pos) {
      h = h.substr(0, pos);
    }
    headers_.push_back(h);
  }
}

void HttpClientConcrete::setContentType() {
  content_type_.clear();

  // Content-Type ヘッダの取得
  for(std::vector<std::string>::iterator iter = headers_.begin(); iter != headers_.end(); ++iter) {
    std::string t = convertToLower((*iter).substr(0, ContentTypeHeader.size()));
    if (t == ContentTypeHeader) {
      std::string type = (*iter).substr(ContentTypeHeader.size());

      content_type_ = getHeaderValue(type);
      break;
    }
  }
}

/*!
 * @brief コンテンツ長を設定する。
 *
 * ヘッダの Content-Length に記載のコンテンツ長を取得する。
 *
 * @caution
 * Content-Length 0またはない場合にかかわらず、コンテンツデータがあれば、
 * HttpClientConcrete#processContents の処理時に正しいコンテンツ長が設定される。
 * 
 * @return コンテンツ長
 */
void HttpClientConcrete::setContentLength() {
  content_length_ = 0;

  // Content-Length ヘッダの取得
  for(std::vector<std::string>::iterator iter = headers_.begin(); iter != headers_.end(); ++iter) {
    std::string t = convertToLower((*iter).substr(0, ContentLengthHeader.size()));
    if (t == ContentLengthHeader) {
      std::string type = (*iter).substr(ContentLengthHeader.size());

      std::string tmp = getHeaderValue(type);
      content_length_ = ::atoi(tmp.c_str());
      break;
    }
  }
}

/*!
 * @breif コンテンツの処理。
 *
 * レスポンスデータを処理して、コンテンツを取り出す。
 * streambuf に先行して読み込み済みのデータがあるので、
 * それも含めてコンテンツとして取得する。
 *
 * @caution
 * コンテンツ長が0の場合でも、実際のデータがあればそちらに従い、
 * コンテンツ長を再設定する。
 */
void HttpClientConcrete::processContents(TimeoutBlockingClient& client) {
  if (contents_ != NULL) {
    delete [] contents_;
    contents_ = NULL;
  }

  // コンテンツサイズの確認
  const size_t length = getContentLength();
  //if (0 == length) {
  //  std::cout << "Maybe no Content-Length header.\n";
  //}

  boost::asio::streambuf& response = client.getStreambuf();

  // バッファに読み込み済みデータを出力
  std::vector<char> buf_pre(0);
  size_t pre_readed = response.size();
  if (pre_readed > 0) {
    buf_pre.reserve(pre_readed);
    const char * st = boost::asio::buffer_cast<const char*>(response.data());
    std::copy(st, st + pre_readed, std::back_inserter(buf_pre));
    response.consume(pre_readed);
  }

  // Read until EOF, writing data to output as we go.
  size_t bytes = client.read(boost::posix_time::seconds(timeout_));

  // メッセージボディ長のチェック
  if (0 == length) {
    content_length_ = pre_readed + bytes;  // 更新

    // メッセージボディに何もなかった場合
    if (0 == content_length_) {
      std::cout << "Content Length is 0.\n";
      return;
    }

  } else if ((pre_readed + bytes) != length) {
    std::cout << "Content Length maybe invalid.\n";
    status_code_ = ERROR_CODE;
    return;
  }

  // 2つ目のバッファに残りデータを読み込み
  std::vector<char> buf_second(0);
  if (0 < bytes) {
    buf_second.reserve(bytes);
    const char * st = boost::asio::buffer_cast<const char*>(response.data());
    std::copy(st, st + bytes, std::back_inserter(buf_second));
  }

  // コンテンツを保存
  contents_ = new char [pre_readed + bytes];  // 領域の確保

  std::copy(buf_pre.begin(), buf_pre.end(),  contents_);
  std::copy(buf_second.begin(), buf_second.end(), contents_ + buf_pre.size());

}

std::string HttpClientConcrete::getHeaderValue(const std::string& target) {
  std::string::size_type pos = target.find(":");

  if (pos != std::string::npos) {
    return trim(target.substr(pos+1));
  }  
  return std::string(target);
}

} // utility
} // openrtm_network_camera
