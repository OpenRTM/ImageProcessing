// -*- C++ -*-
/*!
 * @file  AffineTest.cpp
 * @brief Affine image component
 * @date $Date$
 *
 * $Id$
 */

#include "AffineTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* affine_spec[] =
  {
    "implementation_id", "AffineTest",
    "type_name",         "AffineTest",
    "description",       "Affine image component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.affine_matrix", "0.825,-0.167,40;-0.1,0.83,30",

    // Widget
    "conf.__widget__.affine_matrix", "text",
    // Constraints

    "conf.__type__.affine_matrix", "double",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
AffineTest::AffineTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("original_image", m_image_orig),
    m_image_affineOut("affined_image", m_image_affine)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
AffineTest::~AffineTest()
{
}



RTC::ReturnCode_t AffineTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("affined_image", m_image_affineIn);

  // Set OutPort buffer
  addOutPort("original_image", m_image_origOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("affine_matrix", m_ve2dbMatrix, "0.825,-0.167,40;-0.1,0.83,30");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t AffineTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t AffineTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t AffineTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t AffineTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t AffineTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t AffineTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t AffineTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t AffineTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t AffineTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t AffineTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t AffineTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void AffineTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(affine_spec);
    manager->registerFactory(profile,
                             RTC::Create<AffineTest>,
                             RTC::Delete<AffineTest>);
  }

};


