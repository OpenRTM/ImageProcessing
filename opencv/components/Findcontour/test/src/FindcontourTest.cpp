// -*- C++ -*-
/*!
 * @file  FindcontourTest.cpp
 * @brief Findcontour component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "FindcontourTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* findcontour_spec[] =
  {
    "implementation_id", "FindcontourTest",
    "type_name",         "FindcontourTest",
    "description",       "Findcontour component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.threshold_level", "100",
    "conf.default.contour_level", "1",
    "conf.default.line_thickness", "2",
    "conf.default.line_type", "CV_AA",

    // Widget
    "conf.__widget__.threshold_level", "slider.1",
    "conf.__widget__.contour_level", "text",
    "conf.__widget__.line_thickness", "text",
    "conf.__widget__.line_type", "text",
    // Constraints
    "conf.__constraints__.threshold_level", "0<=x<=255",

    "conf.__type__.threshold_level", "int",
    "conf.__type__.contour_level", "int",
    "conf.__type__.line_thickness", "int",
    "conf.__type__.line_type", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
FindcontourTest::FindcontourTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("original_image", m_image_orig),
    m_image_contourOut("contour_image", m_image_contour)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
FindcontourTest::~FindcontourTest()
{
}



RTC::ReturnCode_t FindcontourTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("contour_image", m_image_contourIn);

  // Set OutPort buffer
  addOutPort("original_image", m_image_origOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("threshold_level", m_nThresholdLv, "100");
  bindParameter("contour_level", m_nContourLv, "1");
  bindParameter("line_thickness", m_nLineThickness, "2");
  bindParameter("line_type", m_nLineType, "CV_AA");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t FindcontourTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t FindcontourTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t FindcontourTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t FindcontourTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t FindcontourTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t FindcontourTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t FindcontourTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t FindcontourTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t FindcontourTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t FindcontourTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t FindcontourTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void FindcontourTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(findcontour_spec);
    manager->registerFactory(profile,
                             RTC::Create<FindcontourTest>,
                             RTC::Delete<FindcontourTest>);
  }

};


