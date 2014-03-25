// -*- C++ -*-
/*!
 * @file  PanasonicNetworkCameraRt.h
 * @brief Network camera component for Panasonic
 * @date  $Date$
 *
 * $Id$
 */

#ifndef PANASONICNETWORKCAMERART_H
#define PANASONICNETWORKCAMERART_H

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">

// </rtc-template>

#include "PanasonicNetworkCamera.h"

using namespace RTC;

/*!
 * @class PanasonicNetworkCameraRt
 * @brief Network camera component for Panasonic
 *
 */
class PanasonicNetworkCameraRt
  : public RTC::DataFlowComponentBase
{
 public:

  /*!
   * @brief configurations for setup using camera api
   *
   * This struct is assumed to remember for last configurations value.
   */
  struct ApiSetupConfigurations {
    std::string cameraHost;
    int         cameraPort;
    std::string user;
    std::string password;
    std::string whiteBalance;
    std::string setupType;
  };

  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  PanasonicNetworkCameraRt(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~PanasonicNetworkCameraRt();

  // <rtc-template block="public_attribute">
  
  // </rtc-template>

  // <rtc-template block="public_operation">
  
  // </rtc-template>

  /***
   *
   * The initialize action (on CREATED->ALIVE transition)
   * formaer rtc_init_entry() 
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onInitialize();

  /***
   *
   * The finalize action (on ALIVE->END transition)
   * formaer rtc_exiting_entry()
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onFinalize();

  /***
   *
   * The startup action when ExecutionContext startup
   * former rtc_starting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStartup(RTC::UniqueId ec_id);

  /***
   *
   * The shutdown action when ExecutionContext stop
   * former rtc_stopping_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onShutdown(RTC::UniqueId ec_id);

  /***
   *
   * The activated action (Active state entry action)
   * former rtc_active_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  /***
   *
   * The deactivated action (Active state exit action)
   * former rtc_active_exit()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  /***
   *
   * The execution action that is invoked periodically
   * former rtc_active_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

  /***
   *
   * The aborting action when main logic error occurred.
   * former rtc_aborting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onAborting(RTC::UniqueId ec_id);

  /***
   *
   * The error action in ERROR state
   * former rtc_error_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onError(RTC::UniqueId ec_id);

  /***
   *
   * The reset action that is invoked resetting
   * This is same but different the former rtc_init_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onReset(RTC::UniqueId ec_id);
  
  /***
   *
   * The state update action that is invoked after onExecute() action
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStateUpdate(RTC::UniqueId ec_id);

  /***
   *
   * The action that is invoked when execution context's rate is changed
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onRateChanged(RTC::UniqueId ec_id);


 protected:
  // <rtc-template block="protected_attribute">
  
  // </rtc-template>

  // <rtc-template block="protected_operation">
  
  // </rtc-template>

  // Configuration variable declaration
  // <rtc-template block="config_declare">
  /*!
   * 
   * - Name:  cameraHost
   * - DefaultValue: 127.0.0.1
   */
  std::string m_cameraHost;
  /*!
   * 
   * - Name:  cameraPort
   * - DefaultValue: 80
   */
  int m_cameraPort;
  /*!
   * 
   * - Name:  user
   * - DefaultValue: user
   */
  std::string m_user;
  /*!
   * 
   * - Name:  password
   * - DefaultValue: password
   */
  std::string m_password;
  /*!
   * 
   * - Name:  imageFormat
   * - DefaultValue: JPEG
   */
  std::string m_imageFormat;
  /*!
   * 
   * - Name:  imageQuality
   * - DefaultValue: 5
   */
  int m_imageQuality;
  /*!
   * 
   * - Name:  imageSize
   * - DefaultValue: 640x480
   */
  std::string m_imageSize;
  /*!
   * 
   * - Name:  whiteBalance
   * - DefaultValue: auto
   */
  std::string m_whiteBalance;
  /*!
   * 
   * - Name:  setupType
   * - DefaultValue: desktop
   */
  std::string m_setupType;

  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  RTC::TimedShortSeq m_ptz;
  /*!
   */
  InPort<RTC::TimedShortSeq> m_ptzIn;
  RTC::TimedShort m_focus;
  /*!
   */
  InPort<RTC::TimedShort> m_focusIn;
  RTC::TimedShort m_brightness;
  /*!
   */
  InPort<RTC::TimedShort> m_brightnessIn;
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  RTC::CameraImage m_image;
  /*!
   */
  OutPort<RTC::CameraImage> m_imageOut;
  
  // </rtc-template>

  // CORBA Port declaration
  // <rtc-template block="corbaport_declare">
  
  // </rtc-template>

  // Service declaration
  // <rtc-template block="service_declare">
  
  // </rtc-template>

  // Consumer declaration
  // <rtc-template block="consumer_declare">
  
  // </rtc-template>

 private:
  // <rtc-template block="private_attribute">
  
  // </rtc-template>

  // <rtc-template block="private_operation">
  
  // </rtc-template>

  /*!
   * @brief 画像取得処理
   */
  void procImage();

  /*!
   * @brief 画像取得
   */
  const char* getCameraImage(int* p_length);

  // データポートによる設定

  /*!
   * @brief パン・チルト・ズーム
   */
  void procPtz();

  /*!
   * @brief フォーカス設定
   */
  void procFocus();

  /*!
   * @brief 明るさ設定
   */
  void procBrightness();

  /*!
   * @brief コンフィギュレーションを用いた設定
   */
  void setupByConfigurations();

  /*!
   * @brief カメラ設定
   *
   * ホスト名とポート番号を設定する
   */
  void setupCamera();

  /*!
   * @brief 認証設定
   *
   * 認証で使うユーザー名とパスワードを設定する
   */
  void setupAuthenticate();

  /*!
   * @brief ホワイトバランス設定
   */
  void setupWhiteBalance();

  /*!
   * @brief 設置タイプ設定
   *
   * 設置タイプ（天井、卓上）を設定する
   */
  void setupSetupType();


  ApiSetupConfigurations m_lastConfig;                                 //!< 以前のコンフィギュレーション値
  openrtm_network_camera::panasonic::PanasonicNetworkCamera m_camera;  //!< Panasonicネットワークカメラクラス
};


extern "C"
{
  DLL_EXPORT void PanasonicNetworkCameraRtInit(RTC::Manager* manager);
};

#endif // PANASONICNETWORKCAMERART_H
