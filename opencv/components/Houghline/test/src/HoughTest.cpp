// -*- C++ -*-
/*!
 * @file  HoughTest.cpp
 * @brief Hough line component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "HoughTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* hough_spec[] =
  {
    "implementation_id", "HoughTest",
    "type_name",         "HoughTest",
    "description",       "Hough line component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.canny_thresld1", "100",
    "conf.default.canny_thresld2", "100",
    "conf.default.hough_method", "PROBABILISTIC",
    "conf.default.hough_thresld", "50",
    "conf.default.hough_param1", "50",
    "conf.default.hough_param2", "10",
    "conf.default.line_color_R", "255",
    "conf.default.line_color_G", "0",
    "conf.default.line_color_B", "0",
    "conf.default.line_thickness", "2",
    "conf.default.line_type", "8",

    // Widget
    "conf.__widget__.canny_thresld1", "slider.1",
    "conf.__widget__.canny_thresld2", "slider.1",
    "conf.__widget__.hough_method", "radio",
    "conf.__widget__.hough_thresld", "slider.1",
    "conf.__widget__.hough_param1", "text",
    "conf.__widget__.hough_param2", "text",
    "conf.__widget__.line_color_R", "text",
    "conf.__widget__.line_color_G", "text",
    "conf.__widget__.line_color_B", "text",
    "conf.__widget__.line_thickness", "text",
    "conf.__widget__.line_type", "radio",
    // Constraints
    "conf.__constraints__.canny_thresld1", "0<=x<=255",
    "conf.__constraints__.canny_thresld2", "0<=x<=255",
    "conf.__constraints__.hough_method", "(PROBABILISTIC,STANDARD,MULTI_SCALE)",
    "conf.__constraints__.hough_thresld", "0<=x<=255",
    "conf.__constraints__.line_type", "(8,4,CV_AA)",

    "conf.__type__.canny_thresld1", "int",
    "conf.__type__.canny_thresld2", "int",
    "conf.__type__.hough_method", "string",
    "conf.__type__.hough_thresld", "int",
    "conf.__type__.hough_param1", "double",
    "conf.__type__.hough_param2", "double",
    "conf.__type__.line_color_R", "int",
    "conf.__type__.line_color_G", "int",
    "conf.__type__.line_color_B", "int",
    "conf.__type__.line_thickness", "int",
    "conf.__type__.line_type", "string",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
HoughTest::HoughTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("origin_image", m_image_orig),
    m_image_houghOut("houghline_image", m_image_hough)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
HoughTest::~HoughTest()
{
}



RTC::ReturnCode_t HoughTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("houghline_image", m_image_houghIn);

  // Set OutPort buffer
  addOutPort("origin_image", m_image_origOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("canny_thresld1", m_canny_threshold1, "100");
  bindParameter("canny_thresld2", m_canny_threshold2, "100");
  bindParameter("hough_method", m_hough_method, "PROBABILISTIC");
  bindParameter("hough_thresld", m_hough_threshold, "50");
  bindParameter("hough_param1", m_hough_param1, "50");
  bindParameter("hough_param2", m_hough_param2, "10");
  bindParameter("line_color_R", m_line_color_R, "255");
  bindParameter("line_color_G", m_line_color_G, "0");
  bindParameter("line_color_B", m_line_color_B, "0");
  bindParameter("line_thickness", m_line_thickness, "2");
  bindParameter("line_type", m_line_type, "8");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t HoughTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HoughTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HoughTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t HoughTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t HoughTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t HoughTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t HoughTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HoughTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HoughTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HoughTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HoughTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void HoughTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(hough_spec);
    manager->registerFactory(profile,
                             RTC::Create<HoughTest>,
                             RTC::Delete<HoughTest>);
  }

};


