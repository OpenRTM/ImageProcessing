/*!
 * @file  SonyNetworkCamera.h
 * @brief NetworkCamera access class for Sony camera.
 * @date  2014-02-09
 */
#ifndef SONY_NETWORK_CAMERA_H
#define SONY_NETWORK_CAMERA_H

#include <string>

#ifndef NO_RTC_LOG
#include <rtm/SystemLogger.h>
#endif

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
  class HttpClient; // 前方宣言
}

/*!
 * @namespace sony
 * @brief Sonyカメラ
 *
 */
namespace sony {

/*!
 * @class SonyNetworkCamera
 * @brief SonyNetworkCamera クラス
 * 
 * Sony製ネットワークカメラへアクセスするためのクラス。
 * クラスのインスタンス化後も、接続先を切り替え可能とする。
 *
 * カメラ呼び出しの結果が成功したか否かをメソッドにより取得可能とする。
 * 詳細なエラー情報はログへ出力する。
 */
class SonyNetworkCamera
{
public:

  /*!
   * @brief 画像サイズ
   */
  enum Resolution {
    w160x120,    //!< 160x120
    w320x240,    //!< 320x240
    w640x480,    //!< 640x480
  };

  /*!
   * @brief フォーカス
   */
  enum FocusType {
    Near,          //!< 近く
    Far,           //!< 遠く
    OnePushFocus,  //!< ワンプッシュ
  };

  /*!
   * @brief ホワイトバランス
   */
  enum WhiteBalance {
    AutoWB,            //!< 自動
    Indoor,            //!< 屋内
    Outdoor,           //!< 屋外
    OnePushWB,         //!< ワンプッシュ
  };

  /*!
   * @brief 設置タイプ
   */
  enum SetupType {
    Ceiling,  //!< 天井
    Desktop,  //!< 卓上
  };

	SonyNetworkCamera(void);
	~SonyNetworkCamera(void);

  /*!
   * @brief カメラの設定
   *
   * @param host  ホスト名またはIPアドレス
   * @param port  ポート番号
   */
  void setCamera(const std::string& host, const std::string& port);

  /*!
   * @brief 認証ユーザーの設定
   *
   * Basic認証用のユーザー名とパスワードを設定する。
   * 設定時は、常に認証情報を送信する。
   * 空文字列を設定すれば、指定が解除される。
   *
   * @param user      ユーザー名
   * @param password  パスワード
   */
  void setAuthenticateUser(const std::string& user, const std::string& password);

  /*!
   * @brief 画像の取得
   *
   * カメラからの画像を取得する。フォーマットはJPEG。
   * 呼び出し時に解像度をパラメータとして与える。
   *
   * @caution 戻り値は呼び出し側で削除しない
   * @caution 次のAPI呼び出し（画像以外も含むすべて）によりデータが消去される。
   * したがって、次の呼び出し前に（必要があれば）戻り値をコピーすること
   *
   * @parma resolution  解像度
   * @param p_length    データ長（出力）
   * @return 画像データ配列へのポインタ
   */
  const char* getImage(const Resolution resolution, int* p_length);

  /*!
   * @brief 画質の設定
   *
   * カメラ画像の画質を設定する。
   *
   * @param quality  画質
   */
  void setQuality(const int quality = DEFAULT_IMAGE_QUALITY);

  /*!
   * @brief 左方向へパン
   */
	void movePanLeft(const int displacement = DEFAULT_PAN);

  /*!
   * @brief 右方向へパン
   */
  void movePanRight(const int displacement = DEFAULT_PAN);

  /*!
   * @brief チルトアップ
   */
  void moveTiltUp(const int displacement = DEFAULT_TILT);

  /*!
   * @brief チルトダウン
   */
  void moveTiltDown(const int displacement = DEFAULT_TILT);

  /*!
   * @brief ズームイン
   */
  void zoomTele(const int displacement = DEFAULT_ZOOM);

  /*!
   * @brief ズームアウト
   */
  void zoomWide(const int displacement = DEFAULT_ZOOM);

  /*!
   * @brief フォーカス調整
   *
   * @param type  フォーカス調整タイプ
   */
  void adjustFocus(const FocusType type);

  /*!
   * @brief ホワイトバランス設定
   *
   * @param type  ホワイトバランスタイプ
   */
  void setWhiteBalance(const WhiteBalance type);

  /*!
   * @brief 明るさ調整
   *
   * @param type  明るさ調整タイプ
   */
  void adjustBrightness(const int brightness = DEFAULT_BRIGHTNESS);

  /*!
   * @brief ホームポジション復帰
   */
  void moveToHomePosition();

  /*!
   * @brief 設置場所の設定
   *
   * @param type  設置場所タイプ
   */
  void setSetupType(const SetupType type);

  /*!
   * @brief 実行APIの成否
   *
   * 前回呼び出したAPIが成功したか否かを調べる。
   *
   * @return true : 前回呼び出しが成功
   *         false: 前回呼び出しが失敗
   */
  bool isLastApiSuccess() const { return lastResult; }

protected:
private:
  /*!
   * @brief パン動作
   *
   * @param parameter 左右
   */
  void movePan(const std::string& parameter, const int displacement);

  /*!
   * @brief チルト動作
   *
   * @param parameter 上下
   */
  void moveTilt(const std::string& parameter, const int displacement);

  /*!
   * @brief ズーム動作
   *
   * @param parameter インアウト
   */
  void moveZoom(const std::string& parameter, const int displacement);

  /*!
   * @brief パン・チルト・ズーム動作
   *
   * @param parameter 動作パラメータ
   */
  void movePTZ(const std::string& parameter);

  /*!
   * @brief フォーカス実行
   *
   * @param parameter 動作パラメータ
   */
  void doFocus(const std::string& parameter);

  /*!
   * @brief レスポンスのContent-TypeがテキストであるAPI呼び出し
   *
   * @param path APIのパス
   */
  void callTextTypeAPI(const std::string& path);

  /*!
   * @brief リクエストの実行
   *
   * ネットワークカメラへのAPIリクエストを行う。
   * 呼び出し結果に応じて、前回の実行結果を設定する。
   *
   * @caution 戻り値は呼び出し側で削除しない
   * @caution 次のAPI呼び出しによりレスポンスボディが消去される。
   *
   * @param path  APIのパス
   * @param p_length  レスポンスデータ長（出力）
   * @return コンテンツ（レスポンスボディ）へのポインタ
   */
  const char* doRequest(const std::string& path, int* p_length);

  /*!
   * @brief 想定したコンテンツか否か
   *
   * リクエストで想定したコンテンツか否かを判定する。
   * 判定は、Contents-Typeヘッダの情報に基づく。
   *
   * @param target_type 想定しているコンテンツタイプ（MIME表記で与える）
   * @return true : レスポンスは想定したコンテンツ またはContents-Typeヘッダがない
   *         false: レスポンスは想定したコンテンツと異なる
   */
  bool isValidContentsType(const std::string& target_type);


#ifndef NO_RTC_LOG
  RTC::Logger rtclog;              //!< ロガー
#endif

  utility::HttpClient* p_client_;  //!< HttpClientインスタンス

  std::string host_;      //!< ホスト名またはIPアドレス
  std::string port_;      //!< ポート番号

  std::string user_;      //!< Basic認証用ユーザー
  std::string password_;  //!< Basic認証用パスワード

  bool lastResult;        //!< 直前のAPIの実行結果, false:なんらかのエラー, true:成功

  static const int DEFAULT_IMAGE_QUALITY = 5;  //!< 画質のデフォルト値
  static const int DEFAULT_PAN           = 5;  //!< パン操作のデフォルト移動量
  static const int DEFAULT_TILT          = 5;  //!< チルト操作のデフォルト移動量
  static const int DEFAULT_ZOOM          = 5;  //!< ズーム操作のデフォルト移動量
  static const int DEFAULT_BRIGHTNESS    = 5;  //!< 明るさのデフォルト値


  SonyNetworkCamera(const SonyNetworkCamera&);
  void operator=(const SonyNetworkCamera&);
};

/*!
 * @brief 整数から整形済み文字列への変換
 *
 * 引数で与えられた整数を、2桁表示し、
 * 不足分は0で埋めた形式の文字列に変換する。
 *
 * @parma val  整形対象の数値
 * @return 整形後の文字列
 */
std::string formatDigitFill0Width2(const int val);

} // sony
} // openrtm_network_camera

#endif