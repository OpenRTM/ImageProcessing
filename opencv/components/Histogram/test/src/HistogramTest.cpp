// -*- C++ -*-
/*!
 * @file  HistogramTest.cpp
 * @brief Histogram image component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "HistogramTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* histogram_spec[] =
  {
    "implementation_id", "HistogramTest",
    "type_name",         "HistogramTest",
    "description",       "Histogram image component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.brightness", "100",
    "conf.default.contrast", "100",

    // Widget
    "conf.__widget__.brightness", "slider.1",
    "conf.__widget__.contrast", "slider.1",
    // Constraints
    "conf.__constraints__.brightness", "0<=x<=200",
    "conf.__constraints__.contrast", "0<=x<=200",

    "conf.__type__.brightness", "int",
    "conf.__type__.contrast", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
HistogramTest::HistogramTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("original_image", m_image_orig),
    m_image_histogramImageOut("histogram_image", m_image_histogramImage),
    m_image_histogramOut("histogram", m_image_histogram)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
HistogramTest::~HistogramTest()
{
}



RTC::ReturnCode_t HistogramTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("histogram_image", m_image_histogramImageIn);
  addInPort("histogram", m_image_histogramIn);

  // Set OutPort buffer
  addOutPort("original_image", m_image_origOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("brightness", m_brightness, "100");
  bindParameter("contrast", m_contrast, "100");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t HistogramTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HistogramTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HistogramTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t HistogramTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t HistogramTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t HistogramTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t HistogramTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HistogramTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HistogramTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HistogramTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HistogramTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void HistogramTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(histogram_spec);
    manager->registerFactory(profile,
                             RTC::Create<HistogramTest>,
                             RTC::Delete<HistogramTest>);
  }

};


