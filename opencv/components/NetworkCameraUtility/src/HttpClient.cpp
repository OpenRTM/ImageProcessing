/*!
 * @file  HttpClient.cpp
 * @brief Http client for dll export
 * @date  2014-02-06
 */
#include "HttpClient.h"

#include <string>

#include "HttpClientConcrete.h"


namespace openrtm_network_camera {
namespace utility {

HttpClient::HttpClient(void)
    : p_client_(new HttpClientConcrete()),
      p_header_container_(NULL) {
}

HttpClient::~HttpClient(void) {
  if (p_client_ != NULL) {
    delete p_client_;
    p_client_ = NULL;
  }
  deleteHeaderContainer();
}

void HttpClient::doGet(const char* p_host_name, const char* p_path_name, const char* p_port) {
  p_client_->doGet(p_host_name, p_path_name, p_port);
}

int HttpClient::getStatusCode() const {
  return p_client_->getStatusCode();
}

/*!
 * @brief ヘッダの取得
 *
 * ヘッダ情報は、HttpClientConcreteインスタンスが保持しており、
 * vector<string> の形式となっている。
 * 外部に公開する場合、STLクラスを使わないので、const char * へのポインタ
 * として戻り値を生成する。
 *
 * @param p_size ヘッダ数
 * @return ヘッダの配列へのポインタ
 */
const char* const* HttpClient::getHeaders(int* p_size) {
  deleteHeaderContainer();

  const std::vector<std::string>& headers = p_client_->getHeaders();
  *p_size = headers.size();
  p_header_container_ = new const char* [*p_size];

  for (int i = 0; i < *p_size; ++i) {
    p_header_container_[i] = headers[i].c_str();
  }
  return p_header_container_;
}

const char* HttpClient::getContents() const {
  return p_client_->getContents();
}

const char* HttpClient::getContentType() const {
  // 空文字列の場合も空文字列へのポインタを返す
  return p_client_->getContentType().c_str();
}

size_t HttpClient::getContentLength() const {
  return p_client_->getContentLength();
}

void HttpClient::setBasicAuthenticationParameter(const char* p_user, const char * p_password) {
  p_client_->setBasicAuthenticationParameter(p_user, p_password);
}

void HttpClient::setTimeout(const long timeout) {
  p_client_->setTimeout(timeout);
}

void HttpClient::deleteHeaderContainer() {
    if (p_header_container_ != NULL) {
    delete [] p_header_container_;
    p_header_container_ = NULL;
  }
}

} // utility
} // openrtm_network_camera
