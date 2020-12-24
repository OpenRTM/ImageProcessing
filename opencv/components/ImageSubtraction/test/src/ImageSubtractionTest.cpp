// -*- C++ -*-
/*!
 * @file  ImageSubtractionTest.cpp
 * @brief Image subtraction conponent
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "ImageSubtractionTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* imagesubtraction_spec[] =
  {
    "implementation_id", "ImageSubtractionTest",
    "type_name",         "ImageSubtractionTest",
    "description",       "Image subtraction conponent",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.control_mode", "b",
    "conf.default.image_height", "240",
    "conf.default.image_width", "320",
    "conf.default.threshold_coefficient", "5.0",
    "conf.default.constant_threshold", "20",

    // Widget
    "conf.__widget__.control_mode", "radio",
    "conf.__widget__.image_height", "text",
    "conf.__widget__.image_width", "text",
    "conf.__widget__.threshold_coefficient", "text",
    "conf.__widget__.constant_threshold", "slider.1",
    // Constraints
    "conf.__constraints__.control_mode", "(b,m)",
    "conf.__constraints__.constant_threshold", "0<=x<=255",

    "conf.__type__.control_mode", "char",
    "conf.__type__.image_height", "int",
    "conf.__type__.image_width", "int",
    "conf.__type__.threshold_coefficient", "double",
    "conf.__type__.constant_threshold", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
ImageSubtractionTest::ImageSubtractionTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_img_origIn("original_image", m_img_orig),
    m_KeyIn("Key", m_Key),
    m_img_captureOut("capture_image", m_img_capture),
    m_img_resultOut("result_image", m_img_result),
    m_img_backOut("back_image", m_img_back),
    m_img_thresholdOut("threshold_image", m_img_threshold)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
ImageSubtractionTest::~ImageSubtractionTest()
{
}



RTC::ReturnCode_t ImageSubtractionTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("capture_image", m_img_captureIn);
  addInPort("result_image", m_img_resultIn);
  addInPort("back_image", m_img_backIn);
  addInPort("threshold_image", m_img_thresholdIn);

  // Set OutPort buffer
  addOutPort("original_image", m_img_origOut);
  addOutPort("Key", m_KeyOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("control_mode", m_cont_mode, "b");
  bindParameter("image_height", m_img_height, "240");
  bindParameter("image_width", m_img_width, "320");
  bindParameter("threshold_coefficient", m_thre_coefficient, "5.0");
  bindParameter("constant_threshold", m_constant_thre, "20");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ImageSubtractionTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageSubtractionTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageSubtractionTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t ImageSubtractionTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageSubtractionTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageSubtractionTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ImageSubtractionTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageSubtractionTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageSubtractionTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageSubtractionTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageSubtractionTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void ImageSubtractionTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(imagesubtraction_spec);
    manager->registerFactory(profile,
                             RTC::Create<ImageSubtractionTest>,
                             RTC::Delete<ImageSubtractionTest>);
  }

};


