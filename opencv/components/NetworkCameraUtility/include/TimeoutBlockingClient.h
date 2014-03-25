/*!
 * @file  TimoutBlockingClient.h
 * @brief Network access client using Boost.Asio
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * @date  2014-02-13
 */
#ifndef TIMEOUT_BLOCKING_CLIENT_H
#define TIMEOUT_BLOCKING_CLIENT_H

#include <string>

#include <boost/asio.hpp>

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

/*!
 * @class TimeoutBlockingClient
 * @brief Boost.Asio を用いた タイムアウト付きのClient クラス
 * 
 * タイムアウトを備えた、ネットワークへアクセスするクライアントクラス。
 * 処理は、Boost.Asio を元に実装する。
 *
 * @caution
 * 各メソッド呼び出し時にタイムアウトを指定するが、本クラスで指定したタイムアウト
 * 時間より早くネットワークアクセスのメソッドがタイムアウトする場合がある
 *
 * Boost.Asio のサンプルを元に記述
 * @see http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp03/timeouts/blocking_tcp_client.cpp
 */
class TimeoutBlockingClient
{
public:
  TimeoutBlockingClient();
  ~TimeoutBlockingClient();

  /*!
   * @brief 接続
   *
   * ホストとポートを指定してTCPによる接続を行う。
   * 接続確認時、指定タイムアウト時間を経過した場合はタイムアウトと
   * 判定し、例外を投げる
   *
   * @caution
   * 名前解決は同期処理で行う（明示的なタイムアウト処理はしない）
   *
   * @param host     ホスト名またはIPアドレス
   * @param service  サービス名またはポート番号
   * @param timeout  タイムアウトまでの時間、秒単位
   */
  void connect(const std::string& host, const std::string& service, boost::posix_time::time_duration timeout);

  /*!
   * @brief 読み込み
   *
   * ソケットからの全データの読み込み
   * 読み込み結果は、getStreambufメソッドで取得した参照を利用して取得する。
   *
   * @param timeout  タイムアウトまでの時間、秒単位
   * @return 読み込みバイト数
   */
  size_t read(boost::posix_time::time_duration timeout);

  /*!
   * @brief 指定データまでの読み込み
   *
   * ソケットから指定データが出現するまでを読み込み
   * 読み込み結果は、getStreambufメソッドで取得した参照を利用して取得する。
   *
   * @param delimiter  指定データ
   * @param timeout    タイムアウトまでの時間、秒単位
   * @return 読み込みバイト数
   */
  size_t read_until(const std::string& delimiter, boost::posix_time::time_duration timeout);

  /*!
   * @brief 書き込み
   *
   * ソケットへのデータの書き込み
   *
   * @param buf      書き込みデータを保持するバッファ
   * @param timeout  タイムアウトまでの時間、秒単位
   */
  void write(boost::asio::streambuf& buf, boost::posix_time::time_duration timeout);

  /*!
   * @brief 読み込み結果を保持するstreambufへの参照を取得
   *
   * 呼び出し側では、戻り値を参照で受けて必要な処理を行う
   */
  boost::asio::streambuf& getStreambuf() { return input_buffer_; }

private:
  /*!
   * @brief タイムアウトの確認
   */
  void check_deadline();

  boost::asio::io_service io_service_;    //!< io_service
  boost::asio::ip::tcp::socket socket_;   //!< TCPソケット
  boost::asio::deadline_timer deadline_;  //!< タイマー
  boost::asio::streambuf input_buffer_;   //!< 読み込み結果を保持するバッファ


  TimeoutBlockingClient(const TimeoutBlockingClient&);
  void operator=(const TimeoutBlockingClient&);
};

} // utility
} // openrtm_network_camera

#endif