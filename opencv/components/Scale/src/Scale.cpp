// -*- C++ -*-
/*!
 * @file  Scale.cpp
 * @brief Scale image component
 * @date $Date$
 *
 * $Id$
 */

#include "Scale.h"

// Module specification
// <rtc-template block="module_spec">
static const char* scale_spec[] =
  {
    "implementation_id", "Scale",
    "type_name",         "Scale",
    "description",       "Scale image component",
    "version",           "1.1.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.output_scale_x", "1.0",
    "conf.default.output_scale_y", "1.0",
    // Widget
    "conf.__widget__.output_scale_x", "text",
    "conf.__widget__.output_scale_y", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Scale::Scale(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("original_image", m_image_orig),
    m_image_outputOut("output_image", m_image_output)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Scale::~Scale()
{
}



RTC::ReturnCode_t Scale::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("original_image", m_image_origIn);
  
  // Set OutPort buffer
  addOutPort("output_image", m_image_outputOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("output_scale_x", m_scale_x, "1.0");
  bindParameter("output_scale_y", m_scale_y, "1.0");

  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Scale::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Scale::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Scale::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Scale::onActivated(RTC::UniqueId ec_id)
{
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t Scale::onDeactivated(RTC::UniqueId ec_id)
{


  return RTC::RTC_OK;
}


RTC::ReturnCode_t Scale::onExecute(RTC::UniqueId ec_id)
{
  /* Common CV actions */
  /* 新しいデータのチェック */
  if (m_image_origIn.isNew()) 
  {
    /* InPortデータの読み込み */
    m_image_origIn.read();

    // Anternative actions

    
	cv::Mat m_image_buff(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC3, (void *)&(m_image_orig.pixels[0]));
   

	cv::Mat m_image_dest(cv::Size(m_image_orig.width  * m_scale_x, m_image_orig.height * m_scale_y), CV_8UC3);     /* 結果出力用IplImage */
          

    /* 画像の大きさを変換する */
	cv::resize(m_image_buff, m_image_dest, m_image_dest.size(), CV_INTER_LINEAR);

    /* 画像データのサイズ取得 */
	int len = m_image_dest.channels() * m_image_dest.size().width * m_image_dest.size().height;
          
    m_image_output.pixels.length(len);
    /* 画面のサイズ情報を入れる */
	m_image_output.width = m_image_dest.size().width;
	m_image_output.height = m_image_dest.size().height;

    /* 反転した画像データをOutPortにコピー */
    memcpy((void *)&(m_image_output.pixels[0]), m_image_dest.data, len);

    m_image_outputOut.write();
  }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Scale::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Scale::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Scale::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Scale::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Scale::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void ScaleInit(RTC::Manager* manager)
  {
    coil::Properties profile(scale_spec);
    manager->registerFactory(profile,
                             RTC::Create<Scale>,
                             RTC::Delete<Scale>);
  }
  
};


