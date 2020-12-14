// -*- C++ -*-
/*!
 * @file  BinarizationTest.cpp
 * @brief Binarization image component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "BinarizationTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* binarization_spec[] =
  {
    "implementation_id", "BinarizationTest",
    "type_name",         "BinarizationTest",
    "description",       "Binarization image component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.threshold_level", "128",

    // Widget
    "conf.__widget__.threshold_level", "slider.1",
    // Constraints
    "conf.__constraints__.threshold_level", "0<=x<=255",

    "conf.__type__.threshold_level", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
BinarizationTest::BinarizationTest(RTC::Manager* manager)
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
BinarizationTest::~BinarizationTest()
{
}



RTC::ReturnCode_t BinarizationTest::onInitialize()
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
  bindParameter("threshold_level", m_nThresholdLv, "128");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t BinarizationTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BinarizationTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BinarizationTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t BinarizationTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t BinarizationTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t BinarizationTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t BinarizationTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BinarizationTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BinarizationTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BinarizationTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BinarizationTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void BinarizationTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(binarization_spec);
    manager->registerFactory(profile,
                             RTC::Create<BinarizationTest>,
                             RTC::Delete<BinarizationTest>);
  }

};


