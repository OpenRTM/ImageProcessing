// -*- C++ -*-
/*!
 * @file  EdgeTest.cpp
 * @brief Edge image component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "EdgeTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* edge_spec[] =
  {
    "implementation_id", "EdgeTest",
    "type_name",         "EdgeTest",
    "description",       "Edge image component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.sobel_x_size", "3",
    "conf.default.sobel_y_size", "3",
    "conf.default.laplacian_size", "3",

    // Widget
    "conf.__widget__.sobel_x_size", "radio",
    "conf.__widget__.sobel_y_size", "radio",
    "conf.__widget__.laplacian_size", "radio",
    // Constraints
    "conf.__constraints__.sobel_x_size", "(1,3,5,7)",
    "conf.__constraints__.sobel_y_size", "(1,3,5,7)",
    "conf.__constraints__.laplacian_size", "(1,3,5,7)",

    "conf.__type__.sobel_x_size", "int",
    "conf.__type__.sobel_y_size", "int",
    "conf.__type__.laplacian_size", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
EdgeTest::EdgeTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("original_image", m_image_orig),
    m_image_edge_sobel_xOut("Edge_image_sobel_x", m_image_edge_sobel_x),
    m_image_edge_sobel_yOut("Edge_image_sobel_y", m_image_edge_sobel_y),
    m_image_edge_LAPLACIANOut("Edge_image_LAPLACIAN", m_image_edge_LAPLACIAN)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
EdgeTest::~EdgeTest()
{
}



RTC::ReturnCode_t EdgeTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("Edge_image_sobel_x", m_image_edge_sobel_xIn);
  addInPort("Edge_image_sobel_y", m_image_edge_sobel_yIn);
  addInPort("Edge_image_LAPLACIAN", m_image_edge_LAPLACIANIn);

  // Set OutPort buffer
  addOutPort("original_image", m_image_origOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("sobel_x_size", m_sobel_x_size, "3");
  bindParameter("sobel_y_size", m_sobel_y_size, "3");
  bindParameter("laplacian_size", m_laplacian_size, "3");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t EdgeTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t EdgeTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t EdgeTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t EdgeTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t EdgeTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t EdgeTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t EdgeTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t EdgeTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t EdgeTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t EdgeTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t EdgeTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void EdgeTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(edge_spec);
    manager->registerFactory(profile,
                             RTC::Create<EdgeTest>,
                             RTC::Delete<EdgeTest>);
  }

};


