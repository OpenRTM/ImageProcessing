// -*- C++ -*-
/*!
 * @file  SubtractCaptureImage.h
 * @brief SubtractCaptureImage component
 * @date  $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#ifndef SUBTRACTCAPTUREIMAGE_H
#define SUBTRACTCAPTUREIMAGE_H

#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

#include <opencv2/opencv.hpp>

#define MASK_THRESHOLD		1 /* backgroundMaskImageやstillObjectMaskImageを生成するための閾値 */
#define THRESHOLD_MAX_VALUE	255 /* 2値化の際に使用する最大値 */

#define BACKGROUND_ALPHA		0.01 /* 背景更新の際の混合比率 */
#define BACKGROUND_INITIAL_THRESHOLD	20  /* 背景の初期閾値 */

#define STILL_OBJECT_ALPHA			0.1 /* 静止物体更新の際の混合比率 */
#define STILL_OBJECT_INITIAL_THRESHOLD	255 /* 静止物体の初期閾値 */

#define THRESHOLD_COEFFICIENT	5.0 /* 閾値の値を引く際の使用する閾値にかける数 */

#define NOT_STILL_DEC_STEP		10 /* 動いた場合のカウンタ減算量 */
#define STILL_OBJECT_TO_BACKGROUND	100 /* 背景として採用する基準 */


// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">
#include "InterfaceDataTypesStub.h"

// </rtc-template>

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>

/*!
 * @class SubtractCaptureImage
 * @brief SubtractCaptureImage component
 *
 */
class SubtractCaptureImage
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  SubtractCaptureImage(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~SubtractCaptureImage();

  // <rtc-template block="public_attribute">
  
  // </rtc-template>

  // <rtc-template block="public_operation">
  
  // </rtc-template>

  /***
   *
   * The initialize action (on CREATED->ALIVE transition)
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onInitialize();

  /***
   *
   * The finalize action (on ALIVE->END transition)
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onFinalize();

  /***
   *
   * The startup action when ExecutionContext startup
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

  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  RTC::CameraImage m_image_orig;
  /*!
   */
  RTC::InPort<RTC::CameraImage> m_image_origIn;
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  RTC::CameraImage m_image_out;
  /*!
   */
  RTC::OutPort<RTC::CameraImage> m_image_outOut;
  RTC::CameraImage m_foreMaskImg;
  /*!
   */
  RTC::OutPort<RTC::CameraImage> m_foreMaskImgOut;
  RTC::CameraImage m_stillMaskImg;
  /*!
   */
  RTC::OutPort<RTC::CameraImage> m_stillMaskImgOut;
  RTC::CameraImage m_backGroundImg;
  /*!
   */
  RTC::OutPort<RTC::CameraImage> m_backGroundImgOut;
  RTC::CameraImage m_stillImg;
  /*!
   */
  RTC::OutPort<RTC::CameraImage> m_stillImgOut;
  
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

  cv::Mat backgroundAverageImage;     /* 背景の平均値保存用 */
  cv::Mat backgroundThresholdImage;   /* 背景の閾値保存用 */
  cv::Mat stillObjectAverageImage;    /* 静止物体の平均値保存用 */
  cv::Mat stillObjectThresholdImage;  /* 静止物体の閾値保存用 */
  cv::Mat stillObjectCounterImage;    /* 静止物体のカウンタ用 */
  cv::Mat backgroundDifferenceImage;  /* 背景差分画像用 */
  cv::Mat stillObjectDifferenceImage; /* 静止物体差分画像用 */
  cv::Mat thresholdImage32;           /* 32bitの閾値画像用 */
  cv::Mat thresholdImage;             /* 閾値画像用 */
  cv::Mat resultImage;                /* 結果画像用 */
  cv::Mat backgroundMaskImage;        /* 背景マスク画像用 */
  cv::Mat foregroundMaskImage;        /* 前景マスク用 */
  cv::Mat stillObjectMaskImage;       /* 静止物体マスク用 */
  cv::Mat movingObjectMask;           /* 動物体マスク用 */
  cv::Mat backgroundCopyMaskImage;    /* 背景にコピーする際に使用するマスク用 */
  cv::Mat tmpMaskImage;               /* テンポラリ用 */
  cv::Mat tmp2MaskImage;              /* テンポラリ用(その2) */
  cv::Mat frameImage32;               /* 32bitのキャプチャした画像用 */
  cv::Mat backgroundImage;            /* 背景画像用 */
  cv::Mat stillObjectImage;           /* 静止物体画像用 */
  cv::Mat outputImage;

  cv::Mat foreGroundMaskBuff;
  cv::Mat stillObjectMaskBuff;
  cv::Mat backGroundBuff;
  cv::Mat stillObjectImageBuff;
  cv::Mat stillObjectCounterBuff;

  int key;    /* キー入力用の変数 */
};


extern "C"
{
  DLL_EXPORT void SubtractCaptureImageInit(RTC::Manager* manager);
};

#endif // SUBTRACTCAPTUREIMAGE_H
