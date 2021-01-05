// -*- C++ -*-
/*!
 * @file  RockPaperScissorsTest.cpp
 * @brief RockpaperScissors check compoenet
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "RockPaperScissorsTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* rockpaperscissors_spec[] =
  {
    "implementation_id", "RockPaperScissorsTest",
    "type_name",         "RockPaperScissorsTest",
    "description",       "RockpaperScissors check compoenet",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.rock_max", "1.0",
    "conf.default.rock_min", "0.85",
    "conf.default.scissor_max", "0.85",
    "conf.default.scissor_min", "0.7",
    "conf.default.paper_max", "0.7",
    "conf.default.paper_min", "0.5",
    "conf.default.iterations", "4",
    "conf.default.out_mode", "1",

    // Widget
    "conf.__widget__.rock_max", "text",
    "conf.__widget__.rock_min", "text",
    "conf.__widget__.scissor_max", "text",
    "conf.__widget__.scissor_min", "text",
    "conf.__widget__.paper_max", "text",
    "conf.__widget__.paper_min", "text",
    "conf.__widget__.iterations", "text",
    "conf.__widget__.out_mode", "radio",
    // Constraints
    "conf.__constraints__.rock_max", "0<=x<=1.0",
    "conf.__constraints__.rock_min", "0<=x<=1.0",
    "conf.__constraints__.scissor_max", "0<=x<=1.0",
    "conf.__constraints__.scissor_min", "0<=x<=1.0",
    "conf.__constraints__.paper_max", "0<=x<=1.0",
    "conf.__constraints__.paper_min", "0<=x<=1.0",
    "conf.__constraints__.out_mode", "(0,1)",

    "conf.__type__.rock_max", "double",
    "conf.__type__.rock_min", "double",
    "conf.__type__.scissor_max", "double",
    "conf.__type__.scissor_min", "double",
    "conf.__type__.paper_max", "double",
    "conf.__type__.paper_min", "double",
    "conf.__type__.iterations", "int",
    "conf.__type__.out_mode", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
RockPaperScissorsTest::RockPaperScissorsTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_img_inputIn("image_input", m_img_input),
    m_img_outputOut("image_output", m_img_output),
    m_resultOut("result", m_result)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
RockPaperScissorsTest::~RockPaperScissorsTest()
{
}



RTC::ReturnCode_t RockPaperScissorsTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("image_output", m_img_outputIn);
  addInPort("result", m_resultIn);

  // Set OutPort buffer
  addOutPort("image_input", m_img_inputOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("rock_max", m_rock_max, "1.0");
  bindParameter("rock_min", m_rock_min, "0.85");
  bindParameter("scissor_max", m_scissor_max, "0.85");
  bindParameter("scissor_min", m_scissor_min, "0.7");
  bindParameter("paper_max", m_paper_max, "0.7");
  bindParameter("paper_min", m_paper_min, "0.5");
  bindParameter("iterations", m_iterations, "4");
  bindParameter("out_mode", m_out_mode, "1");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RockPaperScissorsTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RockPaperScissorsTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RockPaperScissorsTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RockPaperScissorsTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RockPaperScissorsTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RockPaperScissorsTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RockPaperScissorsTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RockPaperScissorsTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RockPaperScissorsTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RockPaperScissorsTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RockPaperScissorsTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void RockPaperScissorsTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(rockpaperscissors_spec);
    manager->registerFactory(profile,
                             RTC::Create<RockPaperScissorsTest>,
                             RTC::Delete<RockPaperScissorsTest>);
  }

};


