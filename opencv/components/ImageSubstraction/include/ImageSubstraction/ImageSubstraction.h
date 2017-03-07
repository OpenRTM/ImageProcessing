// -*- C++ -*-
/*!
 * @file  ImageSubstraction.h
 * @brief Image substraction conponent
 * @date  $Date$
 *
 * $Id$
 */

#ifndef IMAGESUBSTRACTION_H
#define IMAGESUBSTRACTION_H

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

#include <opencv2/opencv.hpp>

#define	NUM_OF_BACKGROUND_FRAMES	50	/* 背景モデルを生成するのに使用する画像の枚数 */

#define CONSTANT_MODE		0	/* 画像全体で1つの閾値を用いるフラグ値 */
#define DYNAMIC_MODE		1	/* 画素ごとに閾値を設けるフラグ値 */

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">

// </rtc-template>

using namespace RTC;

/*!
 * @class ImageSubstraction
 * @brief Image substraction conponent
 *
 */
class ImageSubstraction
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  ImageSubstraction(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~ImageSubstraction();

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
   * - Name:  cont_mode
   * - DefaultValue: b
   */
  char m_cont_mode;
  /*!
   * 
   * - Name:  img_height
   * - DefaultValue: 240
   */
  int m_img_height;
  /*!
   * 
   * - Name:  img_width
   * - DefaultValue: 320
   */
  int m_img_width;
  /*!
   * 
   * - Name:  thre_coefficient
   * - DefaultValue: 5.0
   */
  double m_thre_coefficient;
  /*!
   * 
   * - Name:  constant_thre
   * - DefaultValue: 20
   */
  int m_constant_thre;

  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  RTC::CameraImage m_img_orig;
  /*!
   */
  InPort<RTC::CameraImage> m_img_origIn;
  RTC::TimedLong m_Key;
  /*!
   */
  InPort<RTC::TimedLong> m_KeyIn;
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  RTC::CameraImage m_img_capture;
  /*!
   */
  OutPort<RTC::CameraImage> m_img_captureOut;
  RTC::CameraImage m_img_result;
  /*!
   */
  OutPort<RTC::CameraImage> m_img_resultOut;
  RTC::CameraImage m_img_back;
  /*!
   */
  OutPort<RTC::CameraImage> m_img_backOut;
  RTC::CameraImage m_img_threshold;
  /*!
   */
  OutPort<RTC::CameraImage> m_img_thresholdOut;
  
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
  void initializeBackgroundModel(int num, cv::Size size, double thre_coefficient);

 private:
	 cv::Mat backgroundAverageImage;	/* 背景の平均値保存用IplImage */
	 cv::Mat backgroundThresholdImage;	/* 背景の閾値保存用IplImage */

	 cv::Mat originalImage;		/* キャプチャ画像用IplImage */
	 cv::Mat differenceImage;	/* 差分画像用IplImage */
	 cv::Mat resultImage;
	 cv::Mat outputImage;

	 int ImageSubstraction_count;
	 int	mode;
	 int g_temp_w;
	 int g_temp_h;

	 static const char* mode_str[2];
  // <rtc-template block="private_attribute">
  
  // </rtc-template>

  // <rtc-template block="private_operation">
  
  // </rtc-template>

};



extern "C"
{
  DLL_EXPORT void ImageSubstractionInit(RTC::Manager* manager);
};

#endif // IMAGESUBSTRACTION_H
