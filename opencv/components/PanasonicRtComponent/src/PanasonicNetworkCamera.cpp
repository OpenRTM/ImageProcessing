/*!
 * @file  PanasonicNetworkCamera.cpp
 * @brief NetworkCamera access class for Panasonic camera.
 * @date  2014-02-07
 */

#include "PanasonicNetworkCamera.h"

#include <iostream>
#include <sstream>
#include <string>

#include <boost/regex.hpp>

#ifndef NO_RTC_LOG
#include <rtm/SystemLogger.h>
#endif

#include "HttpClient.h"

/*!
 * @namespace openrtm_network_camera
 * @brief OpenRTM NetworkCamera用コンポーネント
 *
 */
namespace openrtm_network_camera {

/*!
 * @namespace panasonic
 * @brief Panasonicカメラ
 *
 */
namespace panasonic {

namespace {
const char* CONTENTS_TYPE_IMAGE       = "image";
const char* CONTENTS_TYPE_TEXT        = "text";
const char* CONTENTS_TYPE_TEXT_PLAIN  = "text/plain";

const int STATUS_OK = 200;
const int STATUS_NO_CONTENT = 204;
const int STATUS_UNAUTHORIZED = 401;
}

PanasonicNetworkCamera::PanasonicNetworkCamera()
    : p_client_(new utility::HttpClient()),
      host_(""),
      port_(""),
      user_(""),
      password_(""),
      lastResult(false) {
#ifndef NO_RTC_LOG
  rtclog.setName("PanasonicNetworkCamera");
  RTC_DEBUG_STR("PanasonicNetworkCamera ctor");
#endif
}

PanasonicNetworkCamera::~PanasonicNetworkCamera() {
  if (NULL != p_client_) {
    delete p_client_;
    p_client_ = NULL;
  }
}

void PanasonicNetworkCamera::setCamera(const std::string& host, const std::string& port) {
  host_ = host;
  port_ = port;
}

void PanasonicNetworkCamera::setAuthenticateUser(const std::string& user, const std::string& password) {
  user_ = user;
  password_ = password;
}

/*!
 * @brief 画像の取得
 *
 * @caution パラメータが不正の場合は、640x480, Standard でリクエストを行う。
 */
const char* PanasonicNetworkCamera::getImage(const Resolution resolution, const Quality quality, int* p_length) {
  const char* API_PATH = "/SnapshotJPEG?Resolution=";
  const char* QUALITY = "&Quality=";

  std::string path(API_PATH);

  switch(resolution) {
  case w160x120:
    path += "160x120";
    break;
  case w192x144:
    path += "192x144";
    break;
  case w320x240:
    path += "320x240";
    break;
  case w640x480:
    path += "640x480";
    break;
  case w1280x960:
    path += "1280x960";
    break;
  case w1280x1024:
    path += "1280x1024";
    break;
  default:
#ifndef NO_RTC_LOG
    RTC_WARN_STR("getImage: invalid resolution. try 640x480 value.");
#endif
    path += "640x480";
    break;
  }

  path += QUALITY;

  switch(quality) {
  case Motion:
    path += "Motion";
    break;
  case Standard:
    path += "Standard";
    break;
  case Clarity:
    path += "Clarity";
    break;
  default:
#ifndef NO_RTC_LOG
    RTC_WARN_STR("getImage: invalid quality. try Standard value.");
#endif
    path += "Standard";
    break;
  }

  const char* buf = doRequest(path, p_length);
  if (false == isValidContentsType(CONTENTS_TYPE_IMAGE)) {
    return NULL;
  }
  analyzeContents(buf, *p_length);
  return buf;
}

void PanasonicNetworkCamera::movePanLeft() {
  movePan("4");
}
void PanasonicNetworkCamera::movePanRight() {
  movePan("6");
}

void PanasonicNetworkCamera::moveTiltUp() {
  moveTilt("8");
}

void PanasonicNetworkCamera::moveTiltDown() {
  moveTilt("2");
}

void PanasonicNetworkCamera::zoomTele() {
  moveZoom("4");
}
void PanasonicNetworkCamera::zoomWide() {
  moveZoom("6");
}

/*!
 * @brief フォーカスの調整
 *
 * @caution パラメータが不正の場合は、autoでリクエストを行う。
 */
void PanasonicNetworkCamera::adjustFocus(const FocusType type) {
  const char* API_PATH = "/Set?Func=Focus&Kind=0&FocusMode=";

  std::string path(API_PATH);
  switch(type) {
  case Near:
    path += "4";
    break;
  case AutoFocus:
    path += "5";
    break;
  case Far:
    path += "6";
    break;
  default:
#ifndef NO_RTC_LOG
    RTC_WARN_STR("adjustFocus: invalid focus type. try auto value.");
#endif
    path += "5";
    break;
  }

  callTextTypeAPI(path);
}

/*!
 * @brief ホワイトバランスの設定
 *
 * @caution パラメータが不正の場合は、holdでリクエストを行う。
 */
void PanasonicNetworkCamera::setWhiteBalance(const WhiteBalance type) {
  const char* API_PATH = "/Set?Func=CameraWB&Kind=1&Data=";

  std::string path(API_PATH);
  switch(type) {
  case AutoWB:
    path += "0";
    break;
  case Indoor:
    path += "32";
    break;
  case Fluorescent_White:
    path += "48";
    break;
  case Fluorescent_Day:
    path += "64";
    break;
  case Outdoor:
    path += "80";
    break;
  case Hold:
    path += "16";
    break;
  default:
#ifndef NO_RTC_LOG
    RTC_WARN_STR("setWhiteBalance: invalid white balance type. try hold value. (hold means keep current while balance.)");
#endif
    path += "16";
    break;
  }

  callTextTypeAPI(path);
}

/*!
 * @brief 明るさの調整
 *
 * @caution パラメータが不正の場合は、DefaultBrightnessでリクエストを行う。
 */
void PanasonicNetworkCamera::adjustBrightness(const BrightnessType type) {
  const char* API_PATH = "/nphControlCamera?Direction=";

  std::string path(API_PATH);
  switch(type) {
  case Darker:
    path += "Darker";
    break;
  case DefaultBrightness:
    path += "DefaultBrightness";
    break;
  case Brighter:
    path += "Brighter";
    break;
  default:
#ifndef NO_RTC_LOG
    RTC_WARN_STR("adjustBrightness: invalid brightness type. try default brightness value.");
#endif
    path += "DefaultBrightness";
    break;
  }

  callTextTypeAPI(path);
}

void PanasonicNetworkCamera::moveToHomePosition() {
  const char* API_PATH = "/Set?Func=PresetCnt&Kind=0";

  callTextTypeAPI(API_PATH);
}

/*!
 * @brief 設置場所の設定
 *
 * @caution パラメータが不正の場合は、Ceilingでリクエストを行う。
 */
void PanasonicNetworkCamera::setSetupType(const SetupType type) {
  const char* API_PATH = "/Set?Func=Tilt&Kind=1&SetupType=";

  std::string path(API_PATH);
  switch(type) {
  case Ceiling:
    path += "0";
    break;
  case Desktop:
    path += "1";
    break;
  default:
#ifndef NO_RTC_LOG
    RTC_WARN_STR("setSetupType: invalid setuptype value. try ceiling value.");
#endif
    path += "0";
    break;
  }

  callTextTypeAPI(path);
}

void PanasonicNetworkCamera::movePan(const std::string& parameter) {
  const char* API_PAN  = "/Set?Func=Pan&Kind=0&DegMode=";
  movePTZ(API_PAN, parameter);
}
void PanasonicNetworkCamera::moveTilt(const std::string& parameter) {
  const char* API_TILT = "/Set?Func=Tilt&Kind=0&DegMode=";
  movePTZ(API_TILT, parameter);
}
void PanasonicNetworkCamera::moveZoom(const std::string& parameter) {
  const char* API_ZOOM = "/Set?Func=Zoom&Kind=0&ZoomMode=";
  movePTZ(API_ZOOM, parameter);
}
void PanasonicNetworkCamera::movePTZ(const std::string& path, const std::string& parameter) {
  callTextTypeAPI(path + parameter);
}

/*!
 * @brief レスポンスのContent-TypeがテキストであるAPI呼び出し
 *
 * httpリクエストの発行、想定したコンテンツタイプの確認、コンテンツの解析
 * をまとめて実行するためのメソッド。
 *
 * @param path APIのパス
 */
void PanasonicNetworkCamera::callTextTypeAPI(const std::string& path) {
  int length;
  const char* buf = doRequest(path, &length);
  if (false == isValidContentsType(CONTENTS_TYPE_TEXT)) {
    return;
  }
  analyzeContents(buf, length);
}

/*!
 * @brief リクエストの実行
 *
 * PanasonicNetworkCamera::setCamera で設定したカメラに、
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
const char* PanasonicNetworkCamera::doRequest(const std::string& path, int* p_length) {
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

bool PanasonicNetworkCamera::isValidContentsType(const std::string& target_type) {
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

/*!
 * @brief コンテンツの解析
 *
 * doRequest 呼び出しのあと呼ばれることを想定。
 * Panasonicのネットワークカメラの多くのAPIはコンテンツに実行結果
 * を含めて返す。このため、コンテンツを解析してエラーが含まれて
 * いればその情報をログへ出力する。
 * 本メソッドによりコンテンツを解析して処理する。
 *
 * 実際の処理はContent-Typeがtext/plainの場合のみ行われる。
 * エラーコードが検出された場合は、lastResult をfalseに設定する。
 *
 * @param p_contents  コンテンツ（doRequestの戻り値）
 * @param length      コンテンツ長
 */
void PanasonicNetworkCamera::analyzeContents(const char* p_contents, int length) {
  if (NULL == p_contents) {
    return;
  }

  // text/plain であることの確認
  const std::string type((NULL == p_client_->getContentType()) ? "" : p_client_->getContentType());
  if (std::string::npos == type.find(CONTENTS_TYPE_TEXT_PLAIN)) {
    return;
  }

  // Return結果があり、エラー（負の値）ならログへ出力
  const std::string target(p_contents, p_contents+length);

  static const boost::regex reg("^Return:\\s*(-?\\d+)"); // Return:0, Return:-10, Return: 10 etc
  boost::smatch result;
  if (boost::regex_search(target, result, reg)) {
    const int return_code = ::atoi(result.str(1).c_str());
    if (0 > return_code) {
      std::stringstream ss;
      ss << return_code;
#ifndef NO_RTC_LOG
      RTC_WARN_STR("error: return code is " + ss.str());
      RTC_WARN_STR("Check your setup parameters and your camera features.");
#endif
      lastResult = false;  // エラーが見つかったので、falseに変更
    }
  }
}


} // panasonic
} // openrtm_network_camera