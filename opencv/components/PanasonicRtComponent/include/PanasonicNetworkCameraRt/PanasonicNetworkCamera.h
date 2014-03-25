/*!
 * @file  PanasonicNetworkCamera.h
 * @brief NetworkCamera access class for Panasonic camera.
 * @date  2014-02-07
 */
#ifndef PANASONIC_NETWORK_CAMERA_H
#define PANASONIC_NETWORK_CAMERA_H

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
 * @namespace panasonic
 * @brief Panasonicカメラ
 *
 */
namespace panasonic {

/*!
 * @class PanasonicNetworkCamera
 * @brief PanasonicNetworkCamera クラス
 * 
 * Panasonic製ネットワークカメラへアクセスするためのクラス。
 * クラスのインスタンス化後も、接続先を切り替え可能とする。
 *
 * カメラ呼び出しの結果が成功したか否かをメソッドにより取得可能とする。
 * 詳細なエラー情報はログへ出力する。
 */
class PanasonicNetworkCamera
{
public:

  /*!
   * @brief 画像サイズ
   */
  enum Resolution {
    w160x120,    //!< 160x120
    w192x144,    //!< 192x144
    w320x240,    //!< 320x240
    w640x480,    //!< 640x480
    w1280x960,   //!< 1280x960
    w1280x1024,  //!< 1280x1024
  };

  /*!
   * @brief 画質
   */
  enum Quality {
    Motion ,  //!< 動き優先
    Standard, //!< 通常
    Clarity,  //!< 画質優先
  };

  /*!
   * @brief フォーカス
   */
  enum FocusType {
    Near,       //!< 近く
    AutoFocus,  //!< 自動
    Far,        //!< 遠く
  };

  /*!
   * @brief ホワイトバランス
   */
  enum WhiteBalance {
    AutoWB,            //!< 自動
    Indoor,            //!< 屋内
    Fluorescent_White, //!< 蛍光灯（白色）
    Fluorescent_Day,   //!< 蛍光灯（昼白色）
    Outdoor,           //!< 屋外
    Hold,              //!< ホールド
  };

  /*!
   * @brief 明るさ
   */
  enum BrightnessType {
    Darker,             //!< 暗く
    DefaultBrightness,  //!< デフォルトの明るさ
    Brighter,           //!< 明るく
  };

  /*!
   * @brief 設置タイプ
   */
  enum SetupType {
    Ceiling,  //!< 天井
    Desktop,  //!< 卓上
  };

	PanasonicNetworkCamera(void);
	~PanasonicNetworkCamera(void);

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
   * 呼び出し時に解像度と画質をパラメータとして与える。
   * 現在設定中の解像度と画質以外を指定する場合、変更権限が必要。
   * 解像度および画質の指定値が、カメラ側が対応していない場合の挙動はカメラに依存する。
   *
   * @caution 戻り値は呼び出し側で削除しない
   * @caution 次のAPI呼び出し（画像以外も含むすべて）によりデータが消去される。
   * したがって、次の呼び出し前に（必要があれば）戻り値をコピーすること
   *
   * @parma resolution  解像度
   * @param quality     画質
   * @param p_length    データ長（出力）
   * @return 画像データ配列へのポインタ
   */
  const char* getImage(const Resolution resolution, const Quality quality, int* p_length);

  /*!
   * @brief 左方向へパン
   */
	void movePanLeft();

  /*!
   * @brief 右方向へパン
   */
  void movePanRight();

  /*!
   * @brief チルトアップ
   */
  void moveTiltUp();

  /*!
   * @brief チルトダウン
   */
  void moveTiltDown();

  /*!
   * @brief ズームイン
   */
  void zoomTele();

  /*!
   * @brief ズームアウト
   */
  void zoomWide();

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
  void adjustBrightness(const BrightnessType type);

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
  void movePan(const std::string& parameter);

  /*!
   * @brief チルト動作
   *
   * @param parameter 上下
   */
  void moveTilt(const std::string& parameter);

  /*!
   * @brief ズーム動作
   *
   * @param parameter インアウト
   */
  void moveZoom(const std::string& parameter);

  /*!
   * @brief パン・チルト・ズーム動作
   *
   * @param path      APIのパス
   * @param parameter 動作パラメータ
   */
  void movePTZ(const std::string& path, const std::string& parameter);

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

  /*!
   * @brief コンテンツの解析
   *
   * Panasonicのネットワークカメラの多くのAPIはコンテンツに実行結果
   * を含めて返す。このため、コンテンツを解析してエラーが含まれて
   * いればその情報をログへ出力する。
   *
   * エラーコードが検出された場合は前回の実行結果を失敗に設定する。
   *
   * @param p_contents  コンテンツ（doRequestの戻り値）
   * @param length      コンテンツ長
   */
  void analyzeContents(const char* p_contents, int length);

#ifndef NO_RTC_LOG
  RTC::Logger rtclog;              //!< ロガー
#endif

  utility::HttpClient* p_client_;  //!< HttpClientインスタンス

  std::string host_;      //!< ホスト名またはIPアドレス
  std::string port_;      //!< ポート番号

  std::string user_;      //!< Basic認証用ユーザー
  std::string password_;  //!< Basic認証用パスワード

  bool lastResult;        //!< 直前のAPIの実行結果, false:なんらかのエラー, true:成功


  PanasonicNetworkCamera(const PanasonicNetworkCamera&);
  void operator=(const PanasonicNetworkCamera&);
};

} // panasonic
} // openrtm_network_camera

#endif