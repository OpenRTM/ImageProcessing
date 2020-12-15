// -*- C++ -*-
/*!
 * @file  ChromakeyTest.cpp
 * @brief Chromakey image component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "ChromakeyTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* chromakey_spec[] =
  {
    "implementation_id", "ChromakeyTest",
    "type_name",         "ChromakeyTest",
    "description",       "Chromakey image component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.lower_blue", "0",
    "conf.default.upper_blue", "0",
    "conf.default.lower_green", "0",
    "conf.default.upper_green", "0",
    "conf.default.lower_red", "0",
    "conf.default.upper_red", "0",

    // Widget
    "conf.__widget__.lower_blue", "slider.1",
    "conf.__widget__.upper_blue", "slider.1",
    "conf.__widget__.lower_green", "slider.1",
    "conf.__widget__.upper_green", "slider.1",
    "conf.__widget__.lower_red", "slider.1",
    "conf.__widget__.upper_red", "slider.1",
    // Constraints
    "conf.__constraints__.lower_blue", "0<=x<=255",
    "conf.__constraints__.upper_blue", "0<=x<=255",
    "conf.__constraints__.lower_green", "0<=x<=255",
    "conf.__constraints__.upper_green", "0<=x<=255",
    "conf.__constraints__.lower_red", "0<=x<=255",
    "conf.__constraints__.upper_red", "0<=x<=255",

    "conf.__type__.lower_blue", "int",
    "conf.__type__.upper_blue", "int",
    "conf.__type__.lower_green", "int",
    "conf.__type__.upper_green", "int",
    "conf.__type__.lower_red", "int",
    "conf.__type__.upper_red", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
ChromakeyTest::ChromakeyTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_originalIn("original_image", m_image_original),
    m_image_backIn("background_image", m_image_back),
    m_image_outputOut("chromakey_image", m_image_output)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
ChromakeyTest::~ChromakeyTest()
{
}



RTC::ReturnCode_t ChromakeyTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("chromakey_image", m_image_outputIn);

  // Set OutPort buffer
  addOutPort("original_image", m_image_originalOut);
  addOutPort("background_image", m_image_backOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("lower_blue", m_nLowerBlue, "0");
  bindParameter("upper_blue", m_nUpperBlue, "0");
  bindParameter("lower_green", m_nLowerGreen, "0");
  bindParameter("upper_green", m_nUpperGreen, "0");
  bindParameter("lower_red", m_nLowerRed, "0");
  bindParameter("upper_red", m_nUpperRed, "0");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ChromakeyTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ChromakeyTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ChromakeyTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t ChromakeyTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ChromakeyTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ChromakeyTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ChromakeyTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ChromakeyTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ChromakeyTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ChromakeyTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ChromakeyTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void ChromakeyTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(chromakey_spec);
    manager->registerFactory(profile,
                             RTC::Create<ChromakeyTest>,
                             RTC::Delete<ChromakeyTest>);
  }

};


