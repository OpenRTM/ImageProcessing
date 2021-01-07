// -*- C++ -*-
/*!
 * @file  SepiaTest.cpp
 * @brief Sepia component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "SepiaTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* sepia_spec[] =
  {
    "implementation_id", "SepiaTest",
    "type_name",         "SepiaTest",
    "description",       "Sepia component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.image_hue", "22",
    "conf.default.image_Saturation", "90",

    // Widget
    "conf.__widget__.image_hue", "text",
    "conf.__widget__.image_Saturation", "text",
    // Constraints

    "conf.__type__.image_hue", "int",
    "conf.__type__.image_Saturation", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
SepiaTest::SepiaTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("original_image", m_image_orig),
    m_image_sepiaOut("sepia_image", m_image_sepia)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
SepiaTest::~SepiaTest()
{
}



RTC::ReturnCode_t SepiaTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("sepia_image", m_image_sepiaIn);

  // Set OutPort buffer
  addOutPort("original_image", m_image_origOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("image_hue", m_nHue, "22");
  bindParameter("image_Saturation", m_nSaturation, "90");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SepiaTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SepiaTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SepiaTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t SepiaTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t SepiaTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t SepiaTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SepiaTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SepiaTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SepiaTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SepiaTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SepiaTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void SepiaTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(sepia_spec);
    manager->registerFactory(profile,
                             RTC::Create<SepiaTest>,
                             RTC::Delete<SepiaTest>);
  }

};


