// -*- C++ -*-
/*!
 * @file  RockPaperScissors.h
 * @brief RockpaperScissors check compoenet
 * @date  $Date$
 *
 * $Id$
 */

#ifndef ROCKPAPERSCISSORS_H
#define ROCKPAPERSCISSORS_H

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

/* OpenCVHeadファイルのIncluding */
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>
#include "Labeling.h"

/* 肌色抽出用閾値：上限、下限の閾値設定 */
#define HMAX 20
#define HMIN 0
#define SMAX 255*1
#define SMIN 255*0.2
#define VMAX 255*1.0
#define VMIN 255*0

/* 無視する領域サイズ(ラベリング用) */
#define IGNORE_SIZE 1000	

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">

// </rtc-template>

using namespace RTC;

/*!
 * @class RockPaperScissors
 * @brief RockpaperScissors check compoenet
 *
 */
class RockPaperScissors
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  RockPaperScissors(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~RockPaperScissors();

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
   * - Name:  rock_max
   * - DefaultValue: 1.0
   */
  double m_rock_max;
  /*!
   * 
   * - Name:  rock_min
   * - DefaultValue: 0.85
   */
  double m_rock_min;
  /*!
   * 
   * - Name:  scissor_max
   * - DefaultValue: 0.85
   */
  double m_scissor_max;
  /*!
   * 
   * - Name:  scissor_min
   * - DefaultValue: 0.7
   */
  double m_scissor_min;
  /*!
   * 
   * - Name:  paper_max
   * - DefaultValue: 0.7
   */
  double m_paper_max;
  /*!
   * 
   * - Name:  paper_min
   * - DefaultValue: 0.5
   */
  double m_paper_min;
  /*!
   * 
   * - Name:  iterations
   * - DefaultValue: 4
   */
  int m_iterations;
  /*!
   * 
   * - Name:  out_mode
   * - DefaultValue: 1
   */
  int m_out_mode;

  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  RTC::CameraImage m_img_input;
  /*!
   */
  InPort<RTC::CameraImage> m_img_inputIn;
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  RTC::CameraImage m_img_output;
  /*!
   */
  OutPort<RTC::CameraImage> m_img_outputOut;
  RTC::TimedString m_result;
  /*!
   */
  OutPort<RTC::TimedString> m_resultOut;
  
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
  
  void extractSkinColor( void );
  void interpolate( void );
  int  pickupMaxArea( void );
  void createConvexHull(int handarea, cv::Mat &pointMatrix, cv::Mat &hullMatrix );
  void drawConvexHull(cv::Mat &handpoint, cv::Mat &hull, int hullcount);
  int  calcConvexHullArea(cv::Mat &handpoint, cv::Mat &hull, int hullcount);
  void decide( int handarea, int hullarea );
  
  int dummy;
  std::string m_prev_judge;   /* 前回の判定　グー / チョキ / パー */

  cv::Mat m_frame_image;
  cv::Mat m_image_buff;
  cv::Mat m_output_buff;
  cv::Mat m_hsv_buff;
  cv::Mat m_convexHull_buff;
  cv::Mat m_skin_buff;
  cv::Mat m_temp_buff;
  cv::Mat m_label_buff;

  cv::VideoCapture capture;

};


extern "C"
{
  DLL_EXPORT void RockPaperScissorsInit(RTC::Manager* manager);
};

#endif // ROCKPAPERSCISSORS_H
