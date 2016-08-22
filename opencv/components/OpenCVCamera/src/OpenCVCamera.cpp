// -*- C++ -*-
/*!
 * @file  OpenCVCamera.cpp
 * @brief USB Camera Acquire component
 * @date $Date$
 *
 * $Id$
 */

#include "OpenCVCamera.h"
#include <iostream>

using namespace std;

// Module specification
// <rtc-template block="module_spec">
static const char* opencvcamera_spec[] =
  {
    "implementation_id", "OpenCVCamera",
    "type_name",         "OpenCVCamera",
    "description",       "USB Camera Acquire component",
    "version",           "1.1.0",
    "vendor",            "AIST",
    "category",          "example",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.device_num", "0",
	"conf.default.frame_width", "640",
	"conf.default.frame_height", "480",
	"conf.default.frame_rate", "10",
	// Widget
    "conf.__widget__.device_num", "text",
	"conf.__widget__.frame_width", "text",
	"conf.__widget__.frame_height", "text",
	"conf.__widget__.frame_rate", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
OpenCVCamera::OpenCVCamera(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_outOut("out", m_out),

    // </rtc-template>
    dummy(0)
{
}

/*!
 * @brief destructor
 */
OpenCVCamera::~OpenCVCamera()
{
}



RTC::ReturnCode_t OpenCVCamera::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("out", m_outOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("device_num", m_device_num, "0");
  bindParameter("frame_width", m_frame_width, "640");
  bindParameter("frame_height", m_frame_height, "480");
  bindParameter("frame_rate", m_frame_rate, "10");
  // </rtc-template>
 
  m_device_id = -1;
  m_capture = NULL;

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t OpenCVCamera::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t OpenCVCamera::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t OpenCVCamera::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t OpenCVCamera::onActivated(RTC::UniqueId ec_id)
{
  m_device_id = m_device_num;

  /* カメラデバイスの探索 */
  if (NULL == (m_capture = cvCaptureFromCAM(m_device_id)))
  {
    cout << "No Camera Device" << endl;
    return RTC::RTC_ERROR;
  }

  return RTC::RTC_OK;
}


RTC::ReturnCode_t OpenCVCamera::onDeactivated(RTC::UniqueId ec_id)
{
  /* カメラ用メモリの解放 */
  if (m_capture != NULL)
  {
  	cvReleaseCapture(&m_capture);
  }

  return RTC::RTC_OK;
}

RTC::ReturnCode_t OpenCVCamera::onExecute(RTC::UniqueId ec_id)
{
  static coil::TimeValue tmOld;
  static int count = 0;
  const int DISPLAY_PERIOD_FRAME_NUM = 100;
  IplImage *cam_frame = NULL;

  /* 実行中にコンフィグレーションによりデバイスIDが更新された場合 */
  if (m_device_num != m_device_id)
  {
    cvReleaseCapture(&m_capture);
    m_device_id = m_device_num;

    /* カメラデバイスの再探索 */
    if (NULL == (m_capture = cvCaptureFromCAM(m_device_id)))
    {
      cout << "No Camera Device" << endl;
      return RTC::RTC_ERROR;
    }
  }

  cvSetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_WIDTH, m_frame_width);
  cvSetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_HEIGHT, m_frame_height);
  cvSetCaptureProperty(m_capture, CV_CAP_PROP_FPS, m_frame_rate);

  cam_frame = cvQueryFrame(m_capture);
  if (NULL == cam_frame)
  {
    cout << "Bad frame or no frame!!" << endl;
    return RTC::RTC_ERROR;
  }

  IplImage* frame = cvCreateImage(cvGetSize(cam_frame), 8, 3);

  if (cam_frame->origin == IPL_ORIGIN_TL)
  {
    cvCopy(cam_frame, frame);
  } else {
	cvFlip(cam_frame, frame);
  }

  int len = frame->nChannels * frame->width * frame->height;

  /* 画面のサイズ情報を入れる */
  m_out.pixels.length(len);
  m_out.width  = frame->width;
  m_out.height = frame->height;

  memcpy((void *)&(m_out.pixels[0]), frame->imageData, len);
  cvReleaseImage(&frame);

  /* 繋がってるコンポーネントがしんでしまうと問題発生 */
  m_outOut.write();

  /* フレームレートの表示 */
  if (count++ > DISPLAY_PERIOD_FRAME_NUM)
  {
    coil::TimeValue tmNow = coil::gettimeofday();
    double sec(tmNow - tmOld);

    if (sec > 1.0)
	{
	 cout << (DISPLAY_PERIOD_FRAME_NUM / sec) << " [FPS]" << endl;
     tmOld = tmNow;
	}
	count = 0;
  }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t OpenCVCamera::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t OpenCVCamera::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t OpenCVCamera::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t OpenCVCamera::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t OpenCVCamera::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void OpenCVCameraInit(RTC::Manager* manager)
  {
    coil::Properties profile(opencvcamera_spec);
    manager->registerFactory(profile,
                             RTC::Create<OpenCVCamera>,
                             RTC::Delete<OpenCVCamera>);
  }
  
};


