// -*- C++ -*-
/*!
 * @file  CameraViewerTest.cpp
 * @brief USB Camera Monitor component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "CameraViewerTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* cameraviewer_spec[] =
  {
    "implementation_id", "CameraViewerTest",
    "type_name",         "CameraViewerTest",
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
CameraViewerTest::CameraViewerTest(RTC::Manager* manager)
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
CameraViewerTest::~CameraViewerTest()
{
}



RTC::ReturnCode_t CameraViewerTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("Key_out", m_lKeyIn);
  addInPort("Mouse_event", m_lMouseEvIn);
  addInPort("Mouse_X_pos", m_lMouseXIn);
  addInPort("Mouse_Y_pos", m_lMouseYIn);

  // Set OutPort buffer
  addOutPort("in", m_inOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("image_height", m_img_height, "240");
  bindParameter("image_width", m_img_width, "320");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t CameraViewerTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraViewerTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraViewerTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t CameraViewerTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t CameraViewerTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t CameraViewerTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t CameraViewerTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraViewerTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraViewerTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraViewerTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraViewerTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void CameraViewerTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(cameraviewer_spec);
    manager->registerFactory(profile,
                             RTC::Create<CameraViewerTest>,
                             RTC::Delete<CameraViewerTest>);
  }

};


