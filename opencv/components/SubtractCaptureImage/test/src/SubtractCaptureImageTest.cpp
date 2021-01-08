// -*- C++ -*-
/*!
 * @file  SubtractCaptureImageTest.cpp
 * @brief SubtractCaptureImage component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "SubtractCaptureImageTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* subtractcaptureimage_spec[] =
  {
    "implementation_id", "SubtractCaptureImageTest",
    "type_name",         "SubtractCaptureImageTest",
    "description",       "SubtractCaptureImage component",
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
SubtractCaptureImageTest::SubtractCaptureImageTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("original_image", m_image_orig),
    m_image_outOut("output_image", m_image_out),
    m_foreMaskImgOut("foreMaskImg", m_foreMaskImg),
    m_stillMaskImgOut("stillMaskImg", m_stillMaskImg),
    m_backGroundImgOut("backGroundImg", m_backGroundImg),
    m_stillImgOut("stillImg", m_stillImg)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
SubtractCaptureImageTest::~SubtractCaptureImageTest()
{
}



RTC::ReturnCode_t SubtractCaptureImageTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("output_image", m_image_outIn);
  addInPort("foreMaskImg", m_foreMaskImgIn);
  addInPort("stillMaskImg", m_stillMaskImgIn);
  addInPort("backGroundImg", m_backGroundImgIn);
  addInPort("stillImg", m_stillImgIn);

  // Set OutPort buffer
  addOutPort("original_image", m_image_origOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SubtractCaptureImageTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SubtractCaptureImageTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SubtractCaptureImageTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t SubtractCaptureImageTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t SubtractCaptureImageTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t SubtractCaptureImageTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SubtractCaptureImageTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SubtractCaptureImageTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SubtractCaptureImageTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SubtractCaptureImageTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SubtractCaptureImageTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void SubtractCaptureImageTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(subtractcaptureimage_spec);
    manager->registerFactory(profile,
                             RTC::Create<SubtractCaptureImageTest>,
                             RTC::Delete<SubtractCaptureImageTest>);
  }

};


