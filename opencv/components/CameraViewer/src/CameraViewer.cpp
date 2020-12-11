﻿// -*- C++ -*-
/*!
 * @file  CameraViewer.cpp
 * @brief USB Camera Monitor component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "CameraViewer.h"
#include <chrono>

// Module specification
// <rtc-template block="module_spec">
static const char* cameraviewer_spec[] =
  {
    "implementation_id", "CameraViewer",
    "type_name",         "CameraViewer",
    "description",       "USB Camera Monitor component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.image_height", "240",
    "conf.default.image_width", "320",

    // Widget
    "conf.__widget__.image_height", "text",
    "conf.__widget__.image_width", "text",
    // Constraints

    "conf.__type__.image_height", "int",
    "conf.__type__.image_width", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
CameraViewer::CameraViewer(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_inIn("in", m_in),
    m_lKeyOut("Key_out", m_lKey),
    m_lMouseEvOut("Mouse_event", m_lMouseEv),
    m_lMouseXOut("Mouse_X_pos", m_lMouseX),
    m_lMouseYOut("Mouse_Y_pos", m_lMouseY)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
CameraViewer::~CameraViewer()
{
}



RTC::ReturnCode_t CameraViewer::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("in", m_inIn);

  // Set OutPort buffer
  addOutPort("Key_out", m_lKeyOut);
  addOutPort("Mouse_event", m_lMouseEvOut);
  addOutPort("Mouse_X_pos", m_lMouseXOut);
  addOutPort("Mouse_Y_pos", m_lMouseYOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("image_height", m_img_height, "240");
  bindParameter("image_width", m_img_width, "320");
  // </rtc-template>
  
  m_nOldHeight = m_img_height;
  m_nOldWidth  = m_img_width;

  m_lKey.data = 0;

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t CameraViewer::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraViewer::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraViewer::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t CameraViewer::onActivated(RTC::UniqueId ec_id)
{ 
  /* 画像表示用ウィンドウの作成 */
  cv::namedWindow("CaptureImage", WINDOW_AUTOSIZE);
  cv::setMouseCallback("CaptureImage", onMouse, (void*)this);

#ifndef STARTWINDOWTHREAD_DISABLE
  cv::startWindowThread();
#endif
  return RTC::RTC_OK;
}


RTC::ReturnCode_t CameraViewer::onDeactivated(RTC::UniqueId ec_id)
{
  cv::destroyWindow("CaptureImage");

  return RTC::RTC_OK;
}


RTC::ReturnCode_t CameraViewer::onExecute(RTC::UniqueId ec_id)
{
  static auto tm_pre = std::chrono::system_clock::now();
  static int count = 0;

  int nLength;

  m_lKey.data = cv::waitKey(1);
  if(m_lKey.data >= 0)
  {
    printf("[onExecute] Key number %ld is down\n", m_lKey.data);
    m_lKeyOut.write();
  }

  // Check input image is new
  if (!m_inIn.isNew())
  {	
    return RTC::RTC_OK;
  }
  m_inIn.read();    

  nLength = m_in.pixels.length();

  if (!(nLength > 0))
  {
    return RTC::RTC_OK;
  }

  /* データコピー */
  cv::Mat m_orig_img(cv::Size(m_in.width, m_in.height), CV_8UC3, (void *)&(m_in.pixels[0]));
  
  /* 画像表示 */
  cv::imshow("CaptureImage", m_orig_img);

  if (count > 100)
  {
    count = 0;
    auto tm = std::chrono::system_clock::now();
    double sec(std::chrono::duration<double>(tm - tm_pre).count());

    if (sec > 1.0 && sec < 1000.0)
    {
      std::cout << 100.0/sec << " [FPS]" << std::endl;
    }
    tm_pre = tm;
  }

  ++count;

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t CameraViewer::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraViewer::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraViewer::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraViewer::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraViewer::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

bool CameraViewer::isCFGChanged()
{
  if(m_img_height != m_nOldHeight || m_img_width != m_nOldWidth)
    return true;

  return false;
}

void onMouse(int nEvent, int x, int y, int nFlags, void *param)
{
  //printf("[onExecute] Mouse event [%d] is occured on (%d, %d)\n", 
  //                    nEvent, x, y);

  CameraViewer *rtmParam = (CameraViewer *)param;

  rtmParam->m_lMouseEv.data = nEvent;
  rtmParam->m_lMouseX.data  = x;
  rtmParam->m_lMouseY.data  = y;

  rtmParam->m_lMouseEvOut.write();
  rtmParam->m_lMouseXOut.write();
  rtmParam->m_lMouseYOut.write();
}

extern "C"
{

  void CameraViewerInit(RTC::Manager* manager)
  {
    coil::Properties profile(cameraviewer_spec);
    manager->registerFactory(profile,
                             RTC::Create<CameraViewer>,
                             RTC::Delete<CameraViewer>);
  }

};


