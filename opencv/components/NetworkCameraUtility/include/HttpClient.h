/*!
 * @file  HttpClient.h
 * @brief Http client for dll export
 * @date  2014-02-06
 */
#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <stdio.h>
#include "utility_dll_defs.h"

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

class HttpClientConcrete;

/*!
 * @class HttpClient
 * @brief HttpClient クラス
 * 
 * Httpを用いてアクセスするためのクラス。
 * 実際の処理は、HttpClientConcreteクラスへ委譲する。
 * DLL作成時にエクスポートするクラス内部でSTLを使っている場合、
 * 問題が発生したので、外部へエクスポートするクラス（本クラス）では
 * STLを取り除いた。
 *
 * @see http://support.microsoft.com/default.aspx?scid=kb;ja-jp;168958
 * @see http://stackoverflow.com/questions/8976617/when-exporting-stl-stdbasic-string-template-from-dll-i-get-a-lnk2005-error
 */
class NETWORKCAMERAUTILITY_API HttpClient
{
public:
	HttpClient(void);
	~HttpClient(void);

  /*!
   * @brief GETリクエストの実行
   *
   * 指定ホストに対してGETリクエストを実行する。処理は同期で行われる。
   * リクエスト時に、HttpClient#setBasicAuthenticationParameter メソッドにより認証情報
   * が与えられている場合は、Basic認証用の処理も実施する。
   * Httpレスポンスコードが、200、204以外は不正なレスポンスと判断し、
   * 必要なヘッダやコンテンツを設定しない。
   * また、エラー発生時には、ステータスコードに-1を設定する。
   *
   * @param p_host_name  ホスト名（IPアドレスもOK）
   * @param p_path_name  対象となる絶対パス名
   * @param p_port       ポート番号
   */
	void doGet(const char* p_host_name, const char* p_path_name, const char* p_port);

  /*!
   * @brief httpレスポンスのステータスコードを取得する。
   *
   * httpリクエストに対するステータスコードを返す。
   *
   * @return ステータスコード
   *         -1:エラー発生
   */
  int getStatusCode() const;

  /*!
   * @brief httpレスポンスのヘッダを取得する。
   *
   * httpリクエストに対するヘッダの配列を取得する。
   * 各ヘッダは　ヘッダ名：パラメータ　の形式の文字列がそのまま格納されている。
   *
   * @param p_size ヘッダ数
   * @return ヘッダの配列へのポインタ
   * @caution 戻り値は呼び出し側で開放しない
   */
  const char* const* getHeaders(int* p_size);

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
  const char* getContents() const;

  /*!
   * @brief httpレスポンスのコンテンツタイプを取得する。
   *
   * httpリクエストに対するコンテンツタイプを取得する。
   *
   * @return コンテンツタイプ文字列へのポインタ、末尾は\0
   * @caution 戻り値は呼び出し側で開放しない
   */
  const char* getContentType() const;

  /*!
   * @brief httpレスポンスのコンテンツ長を取得する。
   *
   * httpリクエストに対するコンテンツ長を取得する。
   *
   * @return コンテンツ長、データがない場合は0
   */
  size_t getContentLength() const;

  /*!
   * @brief httpリクエスト時の認証情報を設定する。
   *
   * httpリクエスト時のBasic認証用の認証情報を設定する。
   * 指定された場合は、常に認証情報を付加してリクエストを行う。
   *
   * @param p_user      ユーザー名
   * @param p_password  パスワード
   */
  void setBasicAuthenticationParameter(const char* p_user, const char* p_password);

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
   * @brief ヘッダ格納用コンテナの削除
   */
  void deleteHeaderContainer();

  HttpClientConcrete* p_client_;     //!< 実際のhttp処理を行うクラスのインスタンス, ポインタで保持するのはincludeを含めたくないため
  const char** p_header_container_;  //!< ヘッダ格納用コンテナ


  HttpClient(const HttpClient&);
  void operator=(const HttpClient&);
};

} // utility
} // openrtm_network_camera

#endif
