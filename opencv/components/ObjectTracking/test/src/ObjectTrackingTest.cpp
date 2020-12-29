// -*- C++ -*-
/*!
 * @file  ObjectTrackingTest.cpp
 * @brief Objecttrack component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "ObjectTrackingTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* objecttracking_spec[] =
  {
    "implementation_id", "ObjectTrackingTest",
    "type_name",         "ObjectTrackingTest",
    "description",       "Objecttrack component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
ObjectTrackingTest::ObjectTrackingTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_orig_imgIn("img_original", m_orig_img),
    m_eventIn("m_event", m_event),
    m_xIn("m_x", m_x),
    m_yIn("m_y", m_y),
    m_out_imgOut("img_output", m_out_img),
    m_hist_imgOut("img_hist", m_hist_img)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
ObjectTrackingTest::~ObjectTrackingTest()
{
}



RTC::ReturnCode_t ObjectTrackingTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("img_output", m_out_imgIn);
  addInPort("img_hist", m_hist_imgIn);

  // Set OutPort buffer
  addOutPort("img_original", m_orig_imgOut);
  addOutPort("m_event", m_eventOut);
  addOutPort("m_x", m_xOut);
  addOutPort("m_y", m_yOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ObjectTrackingTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ObjectTrackingTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ObjectTrackingTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t ObjectTrackingTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ObjectTrackingTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ObjectTrackingTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ObjectTrackingTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ObjectTrackingTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ObjectTrackingTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ObjectTrackingTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ObjectTrackingTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void ObjectTrackingTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(objecttracking_spec);
    manager->registerFactory(profile,
                             RTC::Create<ObjectTrackingTest>,
                             RTC::Delete<ObjectTrackingTest>);
  }

};


