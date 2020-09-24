// -*- C++ -*-
/*!
 * @file  ObjectTracking.h
 * @brief Objecttrack component
 * @date  $Date$
 *
 * $Id$
 */

#ifndef OBJECTTRACKING_H
#define OBJECTTRACKING_H

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

#include <opencv2/opencv.hpp>
using namespace cv;
#if CV_MAJOR_VERSION < 3
#ifndef EVENT_LBUTTONDOWN
#define EVENT_LBUTTONDOWN CV_EVENT_LBUTTONDOWN
#endif
#ifndef EVENT_LBUTTONUP
#define EVENT_LBUTTONUP CV_EVENT_LBUTTONUP
#endif

#ifndef COLOR_HSV2BGR
#define COLOR_HSV2BGR CV_HSV2BGR
#endif
#ifndef COLOR_BGR2HSV
#define COLOR_BGR2HSV CV_BGR2HSV
#endif

#ifndef FILLED
#define FILLED CV_FILLED
#endif
#endif //CV_MAJOR_VERSION

#define		SEGMENT				150		/* cvSnakeImageで用いる制御点の数 */
#define		WINDOW_WIDTH		17		/* cvSnakeImageで最小値を探索する近傍領域の幅 */
#define		WINDOW_HEIGHT		17		/* cvSnakeImageで最小値を探索する近傍領域の高さ */
#define		HISTIMAGE_WIDTH		320		/* ヒストグラム画像の幅 */
#define		HISTIMAGE_HEIGHT	200		/* ヒストグラム画像の高さ */
#define		H_DIMENSION		16		/* ヒストグラムの次元数 */
#define		H_RANGE_MIN		0
#define		H_RANGE_MAX		180
#define		V_MIN	10		/* 明度の最小値 */
#define		V_MAX	256		/* 明度の最大値 */
#define		S_MIN	30		/* 彩度の最小値 */
#define		S_MAX	256		/* 彩度の最小値 */
#define		HIDDEN_BACKPROJECTION	0 /* バックプロジェクション画像を表示させないフラグ値 */
#define		SHOW_BACKPROJECTION		1 /* バックプロジェクション画像を表示させるフラグ値 */
#define		SELECT_OFF        0 /* 初期追跡領域が設定されていないときのフラグ値 */
#define		SELECT_ON         1 /* 初期追跡領域が設定されているときのフラグ値 */
#define		TRACKING_STOP     0 /* トラッキングを止めるフラグ値 */
#define		TRACKING_START   -1 /* トラッキングを開始するフラグ値 */
#define		TRACKING_NOW      1 /* トラッキング中を示すフラグ値 */
#define		HIDDEN_HISTOGRAM  0 /* ヒストグラムを表示させないフラグ値 */
#define		SHOW_HISTOGRAM    1 /* ヒストグラムを表示させるフラグ値 */
#define		ITERATION_SNAKE  10 /* cvSnakeImageの反復回数 */

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">

// </rtc-template>

using namespace RTC;

/*!
 * @class ObjectTracking
 * @brief Objecttrack component
 *
 */
class ObjectTracking
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  ObjectTracking(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~ObjectTracking();

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

  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  RTC::CameraImage m_orig_img;
  /*!
   */
  InPort<RTC::CameraImage> m_orig_imgIn;
  RTC::TimedLong m_event;
  /*!
   */
  InPort<RTC::TimedLong> m_eventIn;
  RTC::TimedLong m_x;
  /*!
   */
  InPort<RTC::TimedLong> m_xIn;
  RTC::TimedLong m_y;
  /*!
   */
  InPort<RTC::TimedLong> m_yIn;
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  RTC::CameraImage m_out_img;
  /*!
   */
  OutPort<RTC::CameraImage> m_out_imgOut;
  RTC::CameraImage m_hist_img;
  /*!
   */
  OutPort<RTC::CameraImage> m_hist_imgOut;
  
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
  void on_mouse(int event, int x, int y, int flags, void* param);
  cv::Scalar hsv2rgb(float hue);
  void CalculateHist(cv::MatND &hist, cv::Mat &hsvImage, cv::Mat &maskImage, cv::Rect &selection);

 private:
	 cv::Mat inputImage;			/* 入力されたIplImage */
	 cv::Mat resultImage;			/* 処理結果表示用IplImage */
	 cv::Mat hsvImage;			/* HSV表色系用IplImage */
	 cv::Mat hueImage;			/* HSV表色系のHチャンネル用IplImage */
	 cv::Mat maskImage;			/* マスク画像用IplImage */
	 cv::Mat backprojectImage;	/* バックプロジェクション画像用IplImage */
	 cv::Mat histImage;			/* ヒストグラム描画用IplImage */
	 cv::Mat grayImage;			/* グレースケール画像用IplImage */

	 cv::Mat	hist;				/* ヒストグラム処理用構造体 */

	 cv::Mat frameImage;	/* キャプチャ画像用IplImage */
	 cv::VideoCapture	capture;		/* キー入力結果を格納する変数 */
	 int count;
	 int g_temp_w;
	 int g_temp_h;

	 /* 処理モード選択用フラグ */
	 int	backprojectMode;
	 int	selectObject;
	 int	trackObject;
	 int showHist;

	 /* CamShiftトラッキング用変数 */
	 cv::Point			origin;
	 cv::Rect			selection;
	 cv::Rect			trackWindow;
	 cv::RotatedRect		trackRegion;
	 cv::TermCriteria	trackComp;

	 /* ヒストグラム用変数 */
	 int		hdims;		/* ヒストグラムの次元数 */
	 static const float	hRangesArray[2];	/* ヒストグラムのレンジ */
	 
	 int		vmin;
	 int		vmax;

  // <rtc-template block="private_attribute">
  
  // </rtc-template>

  // <rtc-template block="private_operation">
  
  // </rtc-template>

};


extern "C"
{
  DLL_EXPORT void ObjectTrackingInit(RTC::Manager* manager);
};

#endif // OBJECTTRACKING_H
