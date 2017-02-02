// -*- C++ -*-
/*!
 * @file  Hough.h
 * @brief Hough line component
 * @date  $Date$
 *
 * $Id$
 */

#ifndef HOUGH_H
#define HOUGH_H

#include <fstream>
#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

//OpenCV header file include
#include <opencv2/imgproc/imgproc.hpp>

/* cvCanny用定数 */
#define	APERTURE_SIZE   3   /* Sobelオペレータのサイズ (リファレンス参照) */

/* cvHoughLines2用定数 */
#define RHO             1         /* パラメータ空間におけるρ軸の分解能(ピクセル単位) */
#define THETA   ( CV_PI / 180 )   /* パラメータ空間におけるθ軸の分解能(ラジアン単位) */

/* cvLine用定数 */
#define SHIFT     0   /* 座標の小数点以下の桁を表すビット数 */

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">

// </rtc-template>

using namespace RTC;

/*!
 * @class Hough
 * @brief Hough line component
 *
 */
class Hough
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  Hough(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~Hough();

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
   * - Name:  canny_threshold1
   * - DefaultValue: 100
   */
  int m_canny_threshold1;
  /*!
   * 
   * - Name:  canny_threshold2
   * - DefaultValue: 100
   */
  int m_canny_threshold2;
  /*!
   * 
   * - Name:  hough_method
   * - DefaultValue: PROBABILISTIC
   */
  std::string m_hough_method;
  /*!
   * 
   * - Name:  hough_threshold
   * - DefaultValue: 50
   */
  int m_hough_threshold;
  /*!
   * 
   * - Name:  hough_param1
   * - DefaultValue: 50
   */
  double m_hough_param1;
  /*!
   * 
   * - Name:  hough_param2
   * - DefaultValue: 10
   */
  double m_hough_param2;
  /*!
   * 
   * - Name:  line_color_R
   * - DefaultValue: 255
   */
  int m_line_color_R;
  /*!
   * 
   * - Name:  line_color_G
   * - DefaultValue: 0
   */
  int m_line_color_G;
  /*!
   * 
   * - Name:  line_color_B
   * - DefaultValue: 0
   */
  int m_line_color_B;
  /*!
   * 
   * - Name:  line_thickness
   * - DefaultValue: 2
   */
  int m_line_thickness;
  /*!
   * 
   * - Name:  line_type
   * - DefaultValue: 8
   */
  std::string m_line_type;

  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  RTC::CameraImage m_image_orig;
  /*!
   */
  InPort<RTC::CameraImage> m_image_origIn;
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  RTC::CameraImage m_image_hough;
  /*!
   */
  OutPort<RTC::CameraImage> m_image_houghOut;
  
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
	 cv::Mat imageBuff;
	 cv::Mat grayImage;
	 cv::Mat edgeImage;
	 cv::Mat hough;
	 cv::Mat houghImage;
  int len;
  
  
  int debug_method;   /* configuration切り替え時の確認用 */
  int debug_type;     /* configuration切り替え時の確認用 */
  
  int m_in_height;    /* 入力イメージのHeight */
  int m_in_width;     /* 入力イメージのWidth */
  
};


extern "C"
{
  DLL_EXPORT void HoughInit(RTC::Manager* manager);
};

#endif // HOUGH_H
