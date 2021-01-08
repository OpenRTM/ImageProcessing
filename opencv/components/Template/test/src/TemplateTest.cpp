// -*- C++ -*-
/*!
 * @file  TemplateTest.cpp
 * @brief Template image component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "TemplateTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* template_spec[] =
  {
    "implementation_id", "TemplateTest",
    "type_name",         "TemplateTest",
    "description",       "Template image component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.image_path", "template.bmp",

    // Widget
    "conf.__widget__.image_path", "text",
    // Constraints
    "conf.__constraints__.image_path", "[50]",

    "conf.__type__.image_path", "char",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
TemplateTest::TemplateTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("original_image", m_image_orig),
    m_image_templateOut("template", m_image_template),
    m_image_pictureOut("picture", m_image_picture)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
TemplateTest::~TemplateTest()
{
}



RTC::ReturnCode_t TemplateTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("template", m_image_templateIn);
  addInPort("picture", m_image_pictureIn);

  // Set OutPort buffer
  addOutPort("original_image", m_image_origOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("image_path", m_img_path, "template.bmp");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t TemplateTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TemplateTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TemplateTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t TemplateTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TemplateTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TemplateTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t TemplateTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TemplateTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TemplateTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TemplateTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TemplateTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void TemplateTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(template_spec);
    manager->registerFactory(profile,
                             RTC::Create<TemplateTest>,
                             RTC::Delete<TemplateTest>);
  }

};


