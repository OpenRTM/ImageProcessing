// -*- C++ -*-
/*!
 * @file  FlipTest.cpp
 * @brief Flip image component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "FlipTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* flip_spec[] =
  {
    "implementation_id", "FlipTest",
    "type_name",         "FlipTest",
    "description",       "Flip image component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.flip_mode", "1",

    // Widget
    "conf.__widget__.flip_mode", "radio",
    // Constraints
    "conf.__constraints__.flip_mode", "(-1,0,1)",

    "conf.__type__.flip_mode", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
FlipTest::FlipTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("original_image", m_image_orig),
    m_image_flipOut("fliped_image", m_image_flip)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
FlipTest::~FlipTest()
{
}



RTC::ReturnCode_t FlipTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("fliped_image", m_image_flipIn);

  // Set OutPort buffer
  addOutPort("original_image", m_image_origOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("flip_mode", m_flip_mode, "1");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t FlipTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t FlipTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t FlipTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t FlipTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t FlipTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t FlipTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t FlipTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t FlipTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t FlipTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t FlipTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t FlipTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void FlipTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(flip_spec);
    manager->registerFactory(profile,
                             RTC::Create<FlipTest>,
                             RTC::Delete<FlipTest>);
  }

};


