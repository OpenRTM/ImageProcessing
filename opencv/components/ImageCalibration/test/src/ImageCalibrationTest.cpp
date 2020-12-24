// -*- C++ -*-
/*!
 * @file  ImageCalibrationTest.cpp
 * @brief Image Calibration
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "ImageCalibrationTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* imagecalibration_spec[] =
  {
    "implementation_id", "ImageCalibrationTest",
    "type_name",         "ImageCalibrationTest",
    "description",       "Image Calibration",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.checker_w", "13",
    "conf.default.checker_h", "9",
    "conf.default.image_num", "5",

    // Widget
    "conf.__widget__.checker_w", "text",
    "conf.__widget__.checker_h", "text",
    "conf.__widget__.image_num", "text",
    // Constraints

    "conf.__type__.checker_w", "int",
    "conf.__type__.checker_h", "int",
    "conf.__type__.image_num", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
ImageCalibrationTest::ImageCalibrationTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_img_origIn("original_image", m_img_orig),
    m_img_checkOut("checker_image", m_img_check),
    m_CameraCalibrationServicePort("CameraCalibrationService")

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
ImageCalibrationTest::~ImageCalibrationTest()
{
}



RTC::ReturnCode_t ImageCalibrationTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("checker_image", m_img_checkIn);

  // Set OutPort buffer
  addOutPort("original_image", m_img_origOut);

  // Set service provider to Ports

  // Set service consumers to Ports
  m_CameraCalibrationServicePort.registerConsumer("CalibrationService", "ImageCalibService::CalibrationService", m_CalibrationService);

  // Set CORBA Service Ports
  addPort(m_CameraCalibrationServicePort);

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("checker_w", m_checker_w, "13");
  bindParameter("checker_h", m_checker_h, "9");
  bindParameter("image_num", m_image_num, "5");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ImageCalibrationTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageCalibrationTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageCalibrationTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t ImageCalibrationTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageCalibrationTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageCalibrationTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ImageCalibrationTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageCalibrationTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageCalibrationTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageCalibrationTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageCalibrationTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void ImageCalibrationTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(imagecalibration_spec);
    manager->registerFactory(profile,
                             RTC::Create<ImageCalibrationTest>,
                             RTC::Delete<ImageCalibrationTest>);
  }

};


