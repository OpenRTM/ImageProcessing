// -*- C++ -*-
/*!
 * @file  DilationErosionTest.cpp
 * @brief Dilation and erosion component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "DilationErosionTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* dilationerosion_spec[] =
  {
    "implementation_id", "DilationErosionTest",
    "type_name",         "DilationErosionTest",
    "description",       "Dilation and erosion component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.dilation_count", "3",
    "conf.default.erosion_count", "3",
    "conf.default.threshold", "128",

    // Widget
    "conf.__widget__.dilation_count", "text",
    "conf.__widget__.erosion_count", "text",
    "conf.__widget__.threshold", "slider.1",
    // Constraints
    "conf.__constraints__.threshold", "0<=x<=255",

    "conf.__type__.dilation_count", "int",
    "conf.__type__.erosion_count", "int",
    "conf.__type__.threshold", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
DilationErosionTest::DilationErosionTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_img_origIn("original_image", m_img_orig),
    m_img_outOut("output_image", m_img_out),
    m_img_dilationOut("dilation_image", m_img_dilation),
    m_img_erosionOut("erosion_image", m_img_erosion)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
DilationErosionTest::~DilationErosionTest()
{
}



RTC::ReturnCode_t DilationErosionTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("output_image", m_img_outIn);
  addInPort("dilation_image", m_img_dilationIn);
  addInPort("erosion_image", m_img_erosionIn);

  // Set OutPort buffer
  addOutPort("original_image", m_img_origOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("dilation_count", m_count_dilation, "3");
  bindParameter("erosion_count", m_count_erosion, "3");
  bindParameter("threshold", m_nThreshold, "128");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t DilationErosionTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DilationErosionTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DilationErosionTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t DilationErosionTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t DilationErosionTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t DilationErosionTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t DilationErosionTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DilationErosionTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DilationErosionTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DilationErosionTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DilationErosionTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void DilationErosionTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(dilationerosion_spec);
    manager->registerFactory(profile,
                             RTC::Create<DilationErosionTest>,
                             RTC::Delete<DilationErosionTest>);
  }

};


