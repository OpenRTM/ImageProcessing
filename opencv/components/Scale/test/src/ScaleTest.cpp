// -*- C++ -*-
/*!
 * @file  ScaleTest.cpp
 * @brief Scale image component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "ScaleTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* scale_spec[] =
  {
    "implementation_id", "ScaleTest",
    "type_name",         "ScaleTest",
    "description",       "Scale image component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.output_scale_x", "1.0",
    "conf.default.output_scale_y", "1.0",

    // Widget
    "conf.__widget__.output_scale_x", "text",
    "conf.__widget__.output_scale_y", "text",
    // Constraints

    "conf.__type__.output_scale_x", "double",
    "conf.__type__.output_scale_y", "double",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
ScaleTest::ScaleTest(RTC::Manager* manager)
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
ScaleTest::~ScaleTest()
{
}



RTC::ReturnCode_t ScaleTest::onInitialize()
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
  bindParameter("output_scale_x", m_scale_x, "1.0");
  bindParameter("output_scale_y", m_scale_y, "1.0");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ScaleTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ScaleTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ScaleTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t ScaleTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ScaleTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ScaleTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ScaleTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ScaleTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ScaleTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ScaleTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ScaleTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void ScaleTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(scale_spec);
    manager->registerFactory(profile,
                             RTC::Create<ScaleTest>,
                             RTC::Delete<ScaleTest>);
  }

};


