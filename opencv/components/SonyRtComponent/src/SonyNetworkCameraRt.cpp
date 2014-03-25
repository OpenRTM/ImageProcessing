// -*- C++ -*-
/*!
 * @file  SonyNetworkCameraRt.cpp
 * @brief Network camera component for SONY
 * @date $Date$
 *
 * $Id$
 */

#include "SonyNetworkCameraRt.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <rtm/SystemLogger.h>

// Module specification
// <rtc-template block="module_spec">
static const char* sonynetworkcamerart_spec[] =
  {
    "implementation_id", "SonyNetworkCameraRt",
    "type_name",         "SonyNetworkCameraRt",
    "description",       "Network camera component for SONY",
    "version",           "1.0.0",
    "vendor",            "aist",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "0",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.cameraHost", "127.0.0.1",
    "conf.default.cameraPort", "80",
    "conf.default.user", "user",
    "conf.default.password", "password",
    "conf.default.imageFormat", "JPEG",
    "conf.default.imageSize", "640x480",
    "conf.default.imageQuality", "5",
    "conf.default.whiteBalance", "auto",
    "conf.default.setupType", "desktop",
    // Widget
    "conf.__widget__.cameraHost", "text",
    "conf.__widget__.cameraPort", "text",
    "conf.__widget__.user", "text",
    "conf.__widget__.password", "text",
    "conf.__widget__.imageFormat", "radio",
    "conf.__widget__.imageSize", "radio",
    "conf.__widget__.imageQuality", "slider.5",
    "conf.__widget__.whiteBalance", "radio",
    "conf.__widget__.setupType", "radio",
    // Constraints
    "conf.__constraints__.imageFormat", "(JPEG, RAW)",
    "conf.__constraints__.imageSize", "(160x120, 320x240, 640x480)",
    "conf.__constraints__.imageQuality", "1<=x<=10",
    "conf.__constraints__.whiteBalance", "(auto, indoor, outdoor, onepushwb)",
    "conf.__constraints__.setupType", "(ceiling, desktop)",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
SonyNetworkCameraRt::SonyNetworkCameraRt(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_ptzIn("ptz", m_ptz),
    m_focusIn("focus", m_focus),
    m_brightnessIn("brightness", m_brightness),
    m_imageOut("image", m_image)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
SonyNetworkCameraRt::~SonyNetworkCameraRt()
{
}



RTC::ReturnCode_t SonyNetworkCameraRt::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("ptz", m_ptzIn);
  addInPort("focus", m_focusIn);
  addInPort("brightness", m_brightnessIn);
  
  // Set OutPort buffer
  addOutPort("image", m_imageOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("cameraHost", m_cameraHost, "127.0.0.1");
  bindParameter("cameraPort", m_cameraPort, "80");
  bindParameter("user", m_user, "user");
  bindParameter("password", m_password, "password");
  bindParameter("imageFormat", m_imageFormat, "JPEG");
  bindParameter("imageSize", m_imageSize, "640x480");
  bindParameter("imageQuality", m_imageQuality, "5");
  bindParameter("whiteBalance", m_whiteBalance, "auto");
  bindParameter("setupType", m_setupType, "desktop");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SonyNetworkCameraRt::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SonyNetworkCameraRt::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SonyNetworkCameraRt::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t SonyNetworkCameraRt::onActivated(RTC::UniqueId ec_id)
{
  RTC_DEBUG_STR("onActivated");

  // 初期設定
  setupCamera();
  setupAuthenticate();
  setupImageQuality();
  setupWhiteBalance();
  setupSetupType();

  // 以前のコンフィギュレーション値を初期化
  m_lastConfig.cameraHost = m_cameraHost;
  m_lastConfig.cameraPort = m_cameraPort;
  m_lastConfig.user       = m_user;
  m_lastConfig.password   = m_password;
  m_lastConfig.imageQuality = m_imageQuality;
  m_lastConfig.whiteBalance = m_whiteBalance;
  m_lastConfig.setupType    = m_setupType;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t SonyNetworkCameraRt::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t SonyNetworkCameraRt::onExecute(RTC::UniqueId ec_id)
{
  RTC_DEBUG_STR("onExecute");

  // コンフィギュレーション確認
  setupByConfigurations();

  // データポート入力の確認
  procPtz();
  procFocus();
  procBrightness();

  // 画像取得処理
  procImage();

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SonyNetworkCameraRt::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SonyNetworkCameraRt::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SonyNetworkCameraRt::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SonyNetworkCameraRt::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SonyNetworkCameraRt::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*!
 * @brief 画像取得処理
 *
 * 主に、３つのステップで処理を行う。
 * 1. httpを用いた画像の取得
 * 2. 取得した画像のフォーマット変換
 * 3. 出力画像フォーマットに応じた画像をOutPortへ書き込み
 *
 * @caution
 * 出力画像フォーマットにかかわらず、JPEG画像は一度
 * エンコードされる。
 */
void SonyNetworkCameraRt::procImage() {

  int length;
  const char* p_image = getCameraImage(&length);
  if ((false == m_camera.isLastApiSuccess()) || (NULL == p_image) || (0 == length)) {
    RTC_WARN_STR("failed image got.");
    return;
  }

  // フォーマット確認
  bool isJpeg;
  if ("JPEG" == m_imageFormat) {
    isJpeg = true;
  } else if ("RAW" == m_imageFormat) {
    isJpeg = false;
  } else {
    RTC_WARN_STR("invalid format string, " + m_imageFormat + ". use JPEG value");
    isJpeg = true;
  }

  // 画像サイズを取得するために、フォーマット変換を呼び出す
  // 注意
  //   出力画像フォーマットがJPEGの場合も一度は変換される
  std::vector<unsigned char> data;
  data.reserve(length);
  for (int i = 0; i < length; ++i) {
    data.push_back(p_image[i]);
  }

  cv::Mat converted = cv::imdecode(cv::Mat(data), 1);


  // 出力データの設定

  m_image.width = converted.cols;
  m_image.height = converted.rows;

  // 出力フォーマットに応じて処理を変える
  if (isJpeg) {
    m_image.format = "JPEG";

    m_image.pixels.length(length);
    memcpy((void*)&(m_image.pixels[0]), p_image, length);

  } else {
    m_image.format = "RAW";

    const int l = converted.channels() * m_image.width * m_image.height;
    m_image.pixels.length(l);
    memcpy((void*)&(m_image.pixels[0]), converted.data, l);
  }

  // OutPortへ書き込み
  m_imageOut.write();
}
const char* SonyNetworkCameraRt::getCameraImage(int* p_length) {

  // 画像の取得
  using openrtm_network_camera::sony::SonyNetworkCamera;

  SonyNetworkCamera::Resolution resolution;
  if ("160x120" == m_imageSize) {
    resolution = SonyNetworkCamera::w160x120;
  } else if ("320x240" == m_imageSize) {
    resolution = SonyNetworkCamera::w320x240;
  } else if ("640x480" == m_imageSize) {
    resolution = SonyNetworkCamera::w640x480;
  } else {
    RTC_WARN_STR("imageSize in configuration is invalid, " + m_imageSize + ". use 640x480 value.");
    resolution = SonyNetworkCamera::w640x480;
  }

  return m_camera.getImage(resolution, p_length);
}

void SonyNetworkCameraRt::procPtz() {
  if (false == m_ptzIn.isNew()) {
    return;
  }
  m_ptzIn.read();

  if (3 != m_ptz.data.length()) {
    RTC_WARN_STR("data from ptz InPort is invalid. TimedShortSeq must be length 3, but not.");
    return;
  }
  std::stringstream ss;
  for (size_t i = 0; i < m_ptz.data.length()-1; ++i) {
    ss << m_ptz.data[i] << ",";
  }
  ss << m_ptz.data[m_ptz.data.length()-1];
  RTC_INFO_STR("recieve ptz command: " + ss.str());

  // パン
  const int data_pan = m_ptz.data[0];
  if (0 > data_pan) {
    m_camera.movePanRight(-data_pan); // 正の値で与える
  } else if (0 < data_pan) {
    m_camera.movePanLeft(data_pan);
  }

  // チルト
  const int data_tilt = m_ptz.data[1];
  if (0 > data_tilt) {
    m_camera.moveTiltUp(-data_tilt); // 正の値で与える
  } else if (0 < data_tilt) {
    m_camera.moveTiltDown(data_tilt);
  }

  // ズーム
  const int data_zoom = m_ptz.data[2];
  if (0 > data_zoom) {
    m_camera.zoomTele(-data_zoom); // 正の値で与える
  } else if (0 < data_zoom) {
    m_camera.zoomWide(data_zoom);
  }

  // ホームポジション復帰
  if (0 == data_pan && 0 == data_tilt && 0 == data_zoom) {
    m_camera.moveToHomePosition();
  }
}
void SonyNetworkCameraRt::procFocus() {
  if (false == m_focusIn.isNew()) {
    return;
  }
  m_focusIn.read();

  std::stringstream ss;
  ss << m_focus.data;
  RTC_INFO_STR("recieve focus command: " + ss.str());

  if (0 > m_focus.data) {
    m_camera.adjustFocus(openrtm_network_camera::sony::SonyNetworkCamera::Near);
  } else if (0 < m_focus.data) {
    m_camera.adjustFocus(openrtm_network_camera::sony::SonyNetworkCamera::Far);
  } else {
    m_camera.adjustFocus(openrtm_network_camera::sony::SonyNetworkCamera::OnePushFocus);
  }
}
void SonyNetworkCameraRt::procBrightness() {
  if (false == m_brightnessIn.isNew()) {
    return;
  }
  m_brightnessIn.read();

  std::stringstream ss;
  ss << m_brightness.data;
  RTC_INFO_STR("recieve brightnes command: " + ss.str());

  m_camera.adjustBrightness(m_brightness.data);
}

/*!
 * @brief コンフィギュレーションを用いた設定
 *
 * 以前の設定値と異なる場合に、設定用APIを呼び出す
 */
void SonyNetworkCameraRt::setupByConfigurations() {

  // カメラ設定
  if ((m_lastConfig.cameraHost != m_cameraHost) || (m_lastConfig.cameraPort != m_cameraPort)) {
    setupCamera();
    m_lastConfig.cameraHost = m_cameraHost;
    m_lastConfig.cameraPort = m_cameraPort;
  }

  // 認証情報
  if ((m_lastConfig.user != m_user) || (m_lastConfig.password != m_password)) {
    setupAuthenticate();
    m_lastConfig.user       = m_user;
    m_lastConfig.password   = m_password;
  }

  // 画質
  if (m_lastConfig.imageQuality != m_imageQuality) {
    setupImageQuality();
    m_lastConfig.imageQuality = m_imageQuality;
  }

  // ホワイトバランス
  if (m_lastConfig.whiteBalance != m_whiteBalance) {
    setupWhiteBalance();
    m_lastConfig.whiteBalance = m_whiteBalance;
  }

  // 設置場所
  if (m_lastConfig.setupType != m_setupType) {
    setupSetupType();
    m_lastConfig.setupType    = m_setupType;
  }
}

void SonyNetworkCameraRt::setupCamera() {
  int port = m_cameraPort;
  if ((0 > m_cameraPort) || (65535 < m_cameraPort)) {
    std::stringstream ss;
    ss << m_cameraPort;
    RTC_WARN_STR("invalid configuration parameter, m_cameraPort:" + ss.str() + ". use \"80\" instead.");
    port = 80;
  }

  std::stringstream ss;
  ss << port;
  m_camera.setCamera(m_cameraHost, ss.str());
}
void SonyNetworkCameraRt::setupAuthenticate() {
  m_camera.setAuthenticateUser(m_user, m_password);
}
void SonyNetworkCameraRt::setupImageQuality() {
  m_camera.setQuality(m_imageQuality);
}
void SonyNetworkCameraRt::setupWhiteBalance() {
  using openrtm_network_camera::sony::SonyNetworkCamera;

  SonyNetworkCamera::WhiteBalance wb;
  if ("auto" == m_whiteBalance) {
    wb = SonyNetworkCamera::AutoWB;
  } else if ("indoor" == m_whiteBalance) {
    wb = SonyNetworkCamera::Indoor;
  } else if ("outdoor" == m_whiteBalance) {
    wb = SonyNetworkCamera::Outdoor;
  } else if ("onepushwb" == m_whiteBalance) {
    wb = SonyNetworkCamera::OnePushWB;
  } else {
    RTC_WARN_STR("invalid configuration parameter, whilteBalance:" + m_whiteBalance + ". use \"auto\" instead.");
    wb = SonyNetworkCamera::AutoWB;
  }
  m_camera.setWhiteBalance(wb);
}
void SonyNetworkCameraRt::setupSetupType() {
  using openrtm_network_camera::sony::SonyNetworkCamera;

  SonyNetworkCamera::SetupType st;
  if ("ceiling" == m_setupType) {
    st = SonyNetworkCamera::Ceiling;
  } else if ("desktop" == m_setupType) {
    st = SonyNetworkCamera::Desktop;
  } else {
    RTC_WARN_STR("invalid configuration parameter, setupType:" + m_setupType + ". use \"desktop\" instead.");
    st = SonyNetworkCamera::Desktop;
  }
  m_camera.setSetupType(st);
}


extern "C"
{
 
  void SonyNetworkCameraRtInit(RTC::Manager* manager)
  {
    coil::Properties profile(sonynetworkcamerart_spec);
    manager->registerFactory(profile,
                             RTC::Create<SonyNetworkCameraRt>,
                             RTC::Delete<SonyNetworkCameraRt>);
  }
  
};


