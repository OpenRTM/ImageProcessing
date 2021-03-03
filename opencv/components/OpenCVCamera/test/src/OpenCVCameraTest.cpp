// -*- C++ -*-
/*!
 * @file  OpenCVCameraTest.cpp
 * @brief USB Camera Acquire component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "OpenCVCameraTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* opencvcamera_spec[] =
  {
    "implementation_id", "OpenCVCameraTest",
    "type_name",         "OpenCVCameraTest",
    "description",       "USB Camera Acquire component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.capture_mode", "camera",
    "conf.default.device_num", "0",
    "conf.default.video_file", "video.mp4",
    "conf.default.URL", " ",
    "conf.default.frame_width", "640",
    "conf.default.frame_height", "480",
    "conf.default.frame_rate", "30",
    "conf.default.brightness", "128",
    "conf.default.contrast", "32",
    "conf.default.saturation", "32",
    "conf.default.hue", "0",
    "conf.default.gain", "64",
    "conf.default.exposure_mode", "auto",
    "conf.default.exposure_absolute", "166",

    // Widget
    "conf.__widget__.capture_mode", "radio",
    "conf.__widget__.device_num", "text",
    "conf.__widget__.video_file", "text",
    "conf.__widget__.URL", "text",
    "conf.__widget__.frame_width", "text",
    "conf.__widget__.frame_height", "text",
    "conf.__widget__.frame_rate", "text",
    "conf.__widget__.brightness", "slider.1",
    "conf.__widget__.contrast", "slider.1",
    "conf.__widget__.saturation", "slider.1",
    "conf.__widget__.hue", "slider.1",
    "conf.__widget__.gain", "slider.1",
    "conf.__widget__.exposure_mode", "radio",
    "conf.__widget__.exposure_absolute", "slider.1",
    // Constraints
    "conf.__constraints__.capture_mode", "(camera,video,URL)",
    "conf.__constraints__.brightness", "0<=x<=255",
    "conf.__constraints__.contrast", "0<=x<=255",
    "conf.__constraints__.saturation", "0<=x<=255",
    "conf.__constraints__.hue", "-2000<=x<=2000",
    "conf.__constraints__.gain", "0<=x<=255",
    "conf.__constraints__.exposure_mode", "(manual,auto)",
    "conf.__constraints__.exposure_absolute", "3<=x<=2047",

    "conf.__type__.capture_mode", "string",
    "conf.__type__.device_num", "int",
    "conf.__type__.video_file", "string",
    "conf.__type__.URL", "string",
    "conf.__type__.frame_width", "int",
    "conf.__type__.frame_height", "int",
    "conf.__type__.frame_rate", "int",
    "conf.__type__.brightness", "int",
    "conf.__type__.contrast", "int",
    "conf.__type__.saturation", "int",
    "conf.__type__.hue", "int",
    "conf.__type__.gain", "int",
    "conf.__type__.exposure_mode", "string",
    "conf.__type__.exposure_absolute", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
OpenCVCameraTest::OpenCVCameraTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_outOut("out", m_out)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
OpenCVCameraTest::~OpenCVCameraTest()
{
}



RTC::ReturnCode_t OpenCVCameraTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("out", m_outIn);

  // Set OutPort buffer

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("capture_mode", m_capture_mode, "camera");
  bindParameter("device_num", m_device_num, "0");
  bindParameter("video_file", m_video_file, "video.mp4");
  bindParameter("URL", m_URL, " ");
  bindParameter("frame_width", m_frame_width, "640");
  bindParameter("frame_height", m_frame_height, "480");
  bindParameter("frame_rate", m_frame_rate, "30");
  bindParameter("brightness", m_brightness, "128");
  bindParameter("contrast", m_contrast, "32");
  bindParameter("saturation", m_saturation, "32");
  bindParameter("hue", m_hue, "0");
  bindParameter("gain", m_gain, "64");
  bindParameter("exposure_mode", m_exposure_mode, "auto");
  bindParameter("exposure_absolute", m_exposure_absolute, "166");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t OpenCVCameraTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t OpenCVCameraTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t OpenCVCameraTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t OpenCVCameraTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t OpenCVCameraTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t OpenCVCameraTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t OpenCVCameraTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t OpenCVCameraTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t OpenCVCameraTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t OpenCVCameraTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t OpenCVCameraTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void OpenCVCameraTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(opencvcamera_spec);
    manager->registerFactory(profile,
                             RTC::Create<OpenCVCameraTest>,
                             RTC::Delete<OpenCVCameraTest>);
  }

};


