/*!
 * @file  HttpClientConcrete.h
 * @brief Http client using Boost.Asio
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * @date  2014-02-06
 */
#ifndef HTTP_CLIENT_CONCRETE_H
#define HTTP_CLIENT_CONCRETE_H

#include <string>
#include <vector>

/*!
 * @namespace openrtm_network_camera
 * @brief OpenRTM NetworkCamera用コンポーネント
 *
 */
namespace openrtm_network_camera {

/*!
 * @namespace utility
 * @brief 共通処理
 *
 */
namespace utility {

class TimeoutBlockingClient;

/*!
 * @class HttpClientConcrete
 * @brief Boost.Asio を用いた HttpClient クラス
 * 
 * Httpを用いてアクセスするためのクラス。
 * 処理は、Boost.Asio を元に実装する。
 *
 * Boost.Asio のサンプルを元に記述
 * @see http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp03/http/client/sync_client.cpp
 */
class HttpClientConcrete
{
public:
	HttpClientConcrete(void);
	~HttpClientConcrete(void);

  /*!
   * @brief GETリクエストの実行
   *
   * 指定ホストに対してGETリクエストを実行する。処理は同期で行われる。
   * リクエスト時に、HttpClientConcrete#setBasicAuthenticationParameter メソッドにより認証情報
   * が与えられている場合は、Basic認証用の処理も実施する。
   * Httpレスポンスコードが、200、204以外は不正なレスポンスと判断し、
   * 必要なヘッダやコンテンツを設定しない。
   * また、エラー発生時には、ステータスコードに-1を設定する。
   *
   * @param host_name  ホスト名（IPアドレスもOK）
   * @param path_name  対象となる絶対パス名
   * @param port       ポート番号
   */
	void doGet(const std::string& host_name, const std::string& path_name, const std::string& port);

  /*!
   * @brief httpレスポンスのステータスコードを取得する。
   *
   * httpリクエストに対するステータスコードを返す。
   *
   * @return ステータスコード
   *         -1:エラー発生
   */
  int getStatusCode() const { return status_code_; }

  /*!
   * @brief httpレスポンスのヘッダを取得する。
   *
   * httpリクエストに対するヘッダの配列を取得する。
   * 各ヘッダは　ヘッダ名：パラメータ　の形式の文字列がそのまま格納されている。
   *
   * @return ヘッダ
   */
  const std::vector<std::string>& getHeaders() const { return headers_; }

  /*!
   * @brief httpレスポンスのコンテンツを取得する。
   *
   * httpリクエストに対するコンテンツを取得する。
   * ステータスコードが204（No Contents）の場合や、200（OK）の場合でも、
   * コンテンツが含まれていない場合がある。
   * コンテンツはテキストの場合もバイナリデータの場合もあるので、
   * 取得したデータは、getContentType と getContentLength に応じて適切に
   * 処理を行うこと。
   *
   * @return コンテンツデータへのポインタ
   * @caution 戻り値は呼び出し側で開放しない
   */
  const char* getContents() const { return contents_; }

  /*!
   * @brief httpレスポンスのコンテンツタイプを取得する。
   *
   * httpリクエストに対するコンテンツタイプを取得する。
   *
   * @return コンテンツタイプ
   */
  const std::string& getContentType() const { return content_type_; }

  /*!
   * @brief httpレスポンスのコンテンツ長を取得する。
   *
   * httpリクエストに対するコンテンツ長を取得する。
   *
   * @return コンテンツ長、データがない場合は0
   */
  size_t getContentLength() const { return content_length_; }

  /*!
   * @brief httpリクエスト時の認証情報を設定する。
   *
   * httpリクエスト時のBasic認証用の認証情報を設定する。
   * 指定された場合は、常に認証情報を付加してリクエストを行う。
   *
   * @param p_user      ユーザー名
   * @param p_password  パスワード
   */
  void setBasicAuthenticationParameter(const std::string& user, const std::string& password);

  /*!
   * @brief タイムアウト時間の設定
   *
   * ネットワークアクセス時のタイムアウトを指定時間とする。
   * 本メソッドが呼ばれない場合は、デフォルト値が使われる。
   * （デフォルト値は60秒）
   *
   * @caution
   * パラメータが０以下の場合、デフォルト値が使われる。
   *
   * @param timeout  タイムアウト時間, 秒単位
   */
  void setTimeout(const long timeout);

protected:
private:
  /*!
   * @brief httpレスポンスに関連するメンバー変数の初期化
   */
  void response_member_init();

  /*!
   * @brief ヘッダの処理
   *
   * レスポンスデータを処理して、ヘッダを取り出す。
   *
   * @param client  タイムアウト付き同期処理TCPクライアントの参照
   */
  void processHeaders(TimeoutBlockingClient& client);

  /*!
   * @breif コンテンツタイプを設定する。
   */
  void setContentType();

  /*!
   * @breif コンテンツ長を設定する。
   */
  void setContentLength();

  /*!
   * @breif コンテンツの処理。
   *
   * レスポンスデータを処理して、コンテンツを取り出す。
   *
   * @param client  タイムアウト付き同期処理TCPクライアントの参照
   */
  void processContents(TimeoutBlockingClient& client);

  /*!
   * @breif ヘッダ値の取得。
   *
   * ヘッダ文字列を解析し、その値を取り出す。
   *
   * @param target ヘッダ文字列
   * @return ヘッダ値（コロン以降の値）
   */
  std::string getHeaderValue(const std::string& target);


  const static int ERROR_CODE = -1;  //!< エラー発生時のステータスコード
  const static int DEFAULT_TIMEOUT_SEC = 60;  //!< デフォルトのタイムアウト時間, 秒単位

  long timeout_;  //!< タイムアウト時間, 秒単位

  // リクエスト用変数
  std::string user_;      //!< ユーザー名
  std::string password_;  //!< パスワード

  // レスポンス用変数
	int status_code_;                   //!< HTTP問い合わせの戻り値、エラー発生時は-1とする
	std::vector<std::string> headers_;  //!< ヘッダ
  std::string content_type_;          //!< コンテンツタイプ
  size_t content_length_;             //!< コンテンツ長
  char* contents_;                    //!< コンテンツ


	HttpClientConcrete(const HttpClientConcrete&);
	void operator=(const HttpClientConcrete&);
};

} // utility
} // openrtm_network_camera

#endif