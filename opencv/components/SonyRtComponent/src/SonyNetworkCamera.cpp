/*!
 * @file  SonyNetworkCamera.cpp
 * @brief NetworkCamera access class for Sony camera.
 * @date  2014-02-09
 */

#include "SonyNetworkCamera.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#ifndef NO_RTC_LOG
#include <rtm/SystemLogger.h>
#endif

#include "IntBounds.h"
#include "HttpClient.h"

/*!
 * @namespace openrtm_network_camera
 * @brief OpenRTM NetworkCamera用コンポーネント
 *
 */
namespace openrtm_network_camera {

/*!
 * @namespace sony
 * @brief Sonyカメラ
 *
 */
namespace sony {

namespace {
const char* CONTENTS_TYPE_IMAGE = "image";
const char* CONTENTS_TYPE_TEXT  = "text";

const int STATUS_OK = 200;
const int STATUS_NO_CONTENT = 204;
const int STATUS_UNAUTHORIZED = 401;
}

SonyNetworkCamera::SonyNetworkCamera()
    : p_client_(new utility::HttpClient()),
      host_(""),
      port_(""),
      user_(""),
      password_(""),
      lastResult(false) {
#ifndef NO_RTC_LOG
  rtclog.setName("SonyNetworkCamera");
  RTC_DEBUG_STR("SonyNetworkCamera ctor");
#endif
}

SonyNetworkCamera::~SonyNetworkCamera() {
  if (NULL != p_client_) {
    delete p_client_;
    p_client_ = NULL;
  }
}

void SonyNetworkCamera::setCamera(const std::string& host, const std::string& port) {
  host_ = host;
  port_ = port;
}

void SonyNetworkCamera::setAuthenticateUser(const std::string& user, const std::string& password) {
  user_ = user;
  password_ = password;
}

/*!
 * @brief 画像の取得
 *
 * @caution 画像の取得には、/oneshotimage.jpg を使い、解像度は、camera.cgi?jpimagesize
 * で変更する方法もある。しかし、この方法の場合、解像度変更がすぐに反映されず、
 * 取得した画像のサイズが指定外のものになる場合があった。
 * このため、/jpegのAPIを使うこととした。
 *
 * @parma resolution  解像度
 * @param p_length    データ長（出力）
 * @return 画像データ配列へのポインタ
 */
const char* SonyNetworkCamera::getImage(const Resolution resolution, int* p_length) {
  const char* API_PATH = "/jpeg/";
  const char* POST_STR = ".jpg";

  std::string path(API_PATH);

  switch(resolution) {
  case w160x120:
    path += "qqvga";
    break;
  case w320x240:
    path += "qvga";
    break;
  case w640x480:
    path += "vga";
    break;
  default:
#ifndef NO_RTC_LOG
    RTC_WARN_STR("getImage: invalid resolution. try 640x480 value.");
#endif
    path += "vga";
    break;
  }
  path += POST_STR;

  const char* buf = doRequest(path, p_length);
  if (false == isValidContentsType(CONTENTS_TYPE_IMAGE)) {
    return NULL;
  }
  return buf;
}

void SonyNetworkCamera::setQuality(const int quality) {
  const char* API_PATH = "/command/camera.cgi?jpquality=";

  IntBounds<1, 10, DEFAULT_IMAGE_QUALITY> bounds;
  std::stringstream ss;
  ss << bounds(quality);
  
  std::string path(API_PATH);
  path += ss.str();

  callTextTypeAPI(path);
}

void SonyNetworkCamera::movePanLeft(const int displacement) {
  movePan("04", displacement);
}
void SonyNetworkCamera::movePanRight(const int displacement) {
  movePan("06", displacement);
}
void SonyNetworkCamera::moveTiltUp(const int displacement) {
  moveTilt("08", displacement);
}
void SonyNetworkCamera::moveTiltDown(const int displacement) {
  moveTilt("02", displacement);
}
void SonyNetworkCamera::zoomTele(const int displacement) {
  moveZoom("11", displacement);
}
void SonyNetworkCamera::zoomWide(const int displacement) {
  moveZoom("10", displacement);
}

/*!
 * @brief フォーカスの調整
 *
 * フォーカス動作を呼び出す。onepush以外の場合は、
 * 連続してフォーカス停止操作も呼び出す。
 *
 * @caution パラメータが不正の場合は、onepushでリクエストを行う。
 */
void SonyNetworkCamera::adjustFocus(const FocusType type) {
  const char* SPEED = ",1"; // [1,2,...,8] だが、１固定とする

  bool needStop = false;
  std::string type_str;
  switch(type) {
  case Near:
    type_str = "near";
    needStop = true;
    break;
  case Far:
    type_str = "far";
    needStop = true;
    break;
  case OnePushFocus:
    type_str = "onepushaf";
    break;
  default:
#ifndef NO_RTC_LOG
    RTC_WARN_STR("adjustFocus: invalid focus type. try onepush value.");
#endif
    type_str = "onepushaf";
    break;
  }

  // フォーカス実行
  doFocus(type_str + SPEED);

  // フォーカス動作停止
  if (needStop) {
    doFocus("stop,focus");
  }
}

/*!
 * @brief ホワイトバランスの設定
 *
 * @caution パラメータが不正の場合は、autoでリクエストを行う。
 */
void SonyNetworkCamera::setWhiteBalance(const WhiteBalance type) {
  const char* API_PATH = "/command/camera.cgi?wbmode=";

  std::string path(API_PATH);
  switch(type) {
  case AutoWB:
    path += "auto";
    break;
  case Indoor:
    path += "indoor";
    break;
  case Outdoor:
    path += "outdoor";
    break;
  case OnePushWB:
    path += "onepushwb";
    break;
  default:
#ifndef NO_RTC_LOG
    RTC_WARN_STR("setWhiteBalance: invalid white balance type. try auto value.");
#endif
    path += "auto";
    break;
  }

  callTextTypeAPI(path);
}

/*!
 * @brief 明るさの調整
 *
 * @caution パラメータが不正の場合は、DefaultBrightnessでリクエストを行う。
 */
void SonyNetworkCamera::adjustBrightness(const int brightness) {
  const char* API_PATH = "/command/camera.cgi?brightness=";

  IntBounds<0, 10, DEFAULT_BRIGHTNESS> bounds;
  std::stringstream ss;
  ss << bounds(brightness);

  std::string path(API_PATH);
  path += ss.str();

  callTextTypeAPI(path);
}

void SonyNetworkCamera::moveToHomePosition() {
  const char* API_PATH = "/command/presetposition.cgi?homepos=ptz-recall";
  callTextTypeAPI(API_PATH);
}

/*!
 * @brief 設置場所の設定
 *
 * @caution パラメータが不正の場合は、Ceilingでリクエストを行う。
 */
void SonyNetworkCamera::setSetupType(const SetupType type) {
  const char* API_PATH = "/command/camera.cgi?eflip=";

  std::string path(API_PATH);
  switch(type) {
  case Ceiling:
    path += "off";
    break;
  case Desktop:
    path += "on";
    break;
  default:
#ifndef NO_RTC_LOG
    RTC_WARN_STR("setSetupType: invalid setuptype value. try ceiling value.");
#endif
    path += "off";
    break;
  }

  callTextTypeAPI(path);
}


void SonyNetworkCamera::movePan(const std::string& parameter, const int displacement) {
  IntBounds<1, 10, DEFAULT_PAN> bounds;
  movePTZ(parameter + formatDigitFill0Width2(bounds(displacement)));
}
void SonyNetworkCamera::moveTilt(const std::string& parameter, const int displacement) {
  IntBounds<1, 10, DEFAULT_TILT> bounds;
  movePTZ(parameter + formatDigitFill0Width2(bounds(displacement)));
}
void SonyNetworkCamera::moveZoom(const std::string& parameter, const int displacement) {
  IntBounds<1, 10, DEFAULT_ZOOM> bounds;
  movePTZ(parameter + formatDigitFill0Width2(bounds(displacement)));
}
void SonyNetworkCamera::movePTZ(const std::string& parameter) {
  const char* API_PTZ  = "/command/ptzf.cgi?relative=";
  callTextTypeAPI(API_PTZ + parameter);
}

void SonyNetworkCamera::doFocus(const std::string& parameter) {
  const char* API_PATH = "/command/ptzf.cgi?move=";
  callTextTypeAPI(API_PATH + parameter);
}

/*!
 * @brief レスポンスのContent-TypeがテキストであるAPI呼び出し
 *
 * httpリクエストの発行、想定したコンテンツタイプの確認、コンテンツの解析
 * をまとめて実行するためのメソッド。
 *
 * @param path APIのパス
 */
void SonyNetworkCamera::callTextTypeAPI(const std::string& path) {
//  int length;
//  const char* buf = doRequest(path, &length);
  isValidContentsType(CONTENTS_TYPE_TEXT);
}

/*!
 * @brief リクエストの実行
 *
 * SonyNetworkCamera::setCamera で設定したカメラに、
 * 引数で指定されたAPIリクエストを投げる。
 * 実行時は常に、Basic認証付き。
 *
 * @caution 戻り値は呼び出し側で削除しない
 * @caution 次のAPI呼び出しによりレスポンスボディが消去される。
 *
 * @param path  APIのパス
 * @param p_length  レスポンスデータ長（出力）
 * @return コンテンツ（レスポンスボディ）へのポインタ
 */
const char* SonyNetworkCamera::doRequest(const std::string& path, int* p_length) {
  lastResult = false;

  // Basic認証付きリクエスト
  p_client_->setBasicAuthenticationParameter(user_.c_str(), password_.c_str());
  p_client_->doGet(host_.c_str(), path.c_str(), port_.c_str());

  // 結果処理

  // ステータスコード
  *p_length = 0;
  const int status = p_client_->getStatusCode();
  switch(status) {
  case STATUS_OK:
  case STATUS_NO_CONTENT:
    break;
  case STATUS_UNAUTHORIZED:
#ifndef NO_RTC_LOG
    RTC_WARN_STR("401 error. not permitted for this api. Please check your authenticate information.");
#endif
    return NULL;
    break;
  case -1:
#ifndef NO_RTC_LOG
    RTC_WARN_STR("Failed to comunicate to a network camera. Please check host name and port number for a target network camera.");
#endif
    return NULL;
    break;
  default:
    std::stringstream ss;
    ss << status;
#ifndef NO_RTC_LOG
    RTC_WARN_STR("unexpected http status code: " + ss.str());
#endif
    return NULL;
    break;
  }

  // httpステータスコードでの判定結果
  lastResult = true;

  // コンテンツ長の確認
  *p_length = p_client_->getContentLength();
  if (0 == *p_length) {
    return NULL;
  }

  return p_client_->getContents();
}

bool SonyNetworkCamera::isValidContentsType(const std::string& target_type) {
  const std::string type((NULL == p_client_->getContentType()) ? "" : p_client_->getContentType());
  if (0 == type.size()) {
    // コンテンツタイプヘッダがない場合は判定できないのでtrueとする
    return true;
  }

  if (std::string::npos == type.find(target_type)) {
    lastResult = false;  // 想定したコンテンツタイプと異なるので、falseに変更
    return false;
  }
  return true;
}

std::string formatDigitFill0Width2(const int val) {
  std::ostringstream os;
  os << std::setw(2) << std::setfill('0') << val;
  return os.str();
}


} // sony
} // openrtm_network_camera
