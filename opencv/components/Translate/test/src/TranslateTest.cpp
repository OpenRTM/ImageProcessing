// -*- C++ -*-
/*!
 * @file  TranslateTest.cpp
 * @brief Translate image component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "TranslateTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* translate_spec[] =
  {
    "implementation_id", "TranslateTest",
    "type_name",         "TranslateTest",
    "description",       "Translate image component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.Translate_x", "50",
    "conf.default.Translate_y", "50",

    // Widget
    "conf.__widget__.Translate_x", "text",
    "conf.__widget__.Translate_y", "text",
    // Constraints

    "conf.__type__.Translate_x", "int",
    "conf.__type__.Translate_y", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
TranslateTest::TranslateTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("original_image", m_image_orig),
    m_image_outputOut("output_image", m_image_output)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
TranslateTest::~TranslateTest()
{
}



RTC::ReturnCode_t TranslateTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("output_image", m_image_outputIn);

  // Set OutPort buffer
  addOutPort("original_image", m_image_origOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("Translate_x", m_nTransX, "50");
  bindParameter("Translate_y", m_nTransY, "50");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t TranslateTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TranslateTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TranslateTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t TranslateTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TranslateTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TranslateTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t TranslateTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TranslateTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TranslateTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TranslateTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TranslateTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void TranslateTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(translate_spec);
    manager->registerFactory(profile,
                             RTC::Create<TranslateTest>,
                             RTC::Delete<TranslateTest>);
  }

};


