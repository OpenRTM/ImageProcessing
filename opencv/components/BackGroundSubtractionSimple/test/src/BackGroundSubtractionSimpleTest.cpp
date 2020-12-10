// -*- C++ -*-
/*!
 * @file  BackGroundSubtractionSimpleTest.cpp
 * @brief BackGroundSubtractionSimple component
 * @date $Date$
 *
 * $Id$
 */

#include "BackGroundSubtractionSimpleTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* backgroundsubtractionsimple_spec[] =
  {
    "implementation_id", "BackGroundSubtractionSimpleTest",
    "type_name",         "BackGroundSubtractionSimpleTest",
    "description",       "BackGroundSubtractionSimple component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.control_mode", "b",
    "conf.default.diff_mode", "0",
    "conf.default.noise_mode", "0",
    "conf.default.threshold_level", "20",

    // Widget
    "conf.__widget__.control_mode", "radio",
    "conf.__widget__.diff_mode", "radio",
    "conf.__widget__.noise_mode", "radio",
    "conf.__widget__.threshold_level", "slider.1",
    // Constraints
    "conf.__constraints__.control_mode", "(b,M)",
    "conf.__constraints__.diff_mode", "(0,1,2)",
    "conf.__constraints__.noise_mode", "(0,1,2)",
    "conf.__constraints__.threshold_level", "0<=x<=255",

    "conf.__type__.control_mode", "char",
    "conf.__type__.diff_mode", "char",
    "conf.__type__.noise_mode", "char",
    "conf.__type__.threshold_level", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
BackGroundSubtractionSimpleTest::BackGroundSubtractionSimpleTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_img_origIn("original_image", m_img_orig),
    m_keyIn("key", m_key),
    m_img_currOut("current_image", m_img_curr),
    m_img_resuOut("result_image", m_img_resu),
    m_img_backOut("background_image", m_img_back)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
BackGroundSubtractionSimpleTest::~BackGroundSubtractionSimpleTest()
{
}



RTC::ReturnCode_t BackGroundSubtractionSimpleTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("current_image", m_img_currIn);
  addInPort("result_image", m_img_resuIn);
  addInPort("background_image", m_img_backIn);

  // Set OutPort buffer
  addOutPort("original_image", m_img_origOut);
  addOutPort("key", m_keyOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("control_mode", m_cont_mode, "b");
  bindParameter("diff_mode", m_diff_mode, "0");
  bindParameter("noise_mode", m_noise_mode, "0");
  bindParameter("threshold_level", m_threshold_level, "20");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t BackGroundSubtractionSimpleTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackGroundSubtractionSimpleTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackGroundSubtractionSimpleTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t BackGroundSubtractionSimpleTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t BackGroundSubtractionSimpleTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t BackGroundSubtractionSimpleTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t BackGroundSubtractionSimpleTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackGroundSubtractionSimpleTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackGroundSubtractionSimpleTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackGroundSubtractionSimpleTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackGroundSubtractionSimpleTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void BackGroundSubtractionSimpleTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(backgroundsubtractionsimple_spec);
    manager->registerFactory(profile,
                             RTC::Create<BackGroundSubtractionSimpleTest>,
                             RTC::Delete<BackGroundSubtractionSimpleTest>);
  }

};


