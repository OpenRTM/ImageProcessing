// -*- C++ -*-
/*!
 * @file  Sepia.cpp
 * @brief Sepia component
 * @date $Date$
 *
 * $Id$
 */

#include "Sepia.h"

// Module specification
// <rtc-template block="module_spec">
static const char* sepia_spec[] =
  {
    "implementation_id", "Sepia",
    "type_name",         "Sepia",
    "description",       "Sepia component",
    "version",           "1.1.0",
    "vendor",            "AIST",
    "category",          "Category",
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
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Sepia::Sepia(RTC::Manager* manager)
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
Sepia::~Sepia()
{
}



RTC::ReturnCode_t Sepia::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("original_image", m_image_origIn);
  
  // Set OutPort buffer
  addOutPort("sepia_image", m_image_sepiaOut);
  
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
RTC::ReturnCode_t Sepia::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Sepia::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Sepia::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Sepia::onActivated(RTC::UniqueId ec_id)
{


  m_in_height        = 0;
  m_in_width         = 0;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t Sepia::onDeactivated(RTC::UniqueId ec_id)
{
  /* イメージ用メモリの解放 */


  if (!m_image_buff.empty())
  {
	  m_image_buff.release();
  }
  if (!m_hsvImage.empty())
  {
	  m_hsvImage.release();
  }
  if (!m_hueImage.empty())
  {
	  m_hueImage.release();
  }
  if (!m_saturationImage.empty())
  {
	  m_saturationImage.release();
  }
  if (!m_valueImage.empty())
  {
	  m_valueImage.release();
  }
  if (!m_mergeImage.empty())
  {
	  m_mergeImage.release();
  }
  if (!m_destinationImage.empty())
  {
	  m_destinationImage.release();
  }

  return RTC::RTC_OK;
}


RTC::ReturnCode_t Sepia::onExecute(RTC::UniqueId ec_id)
{
  /* Common CV actions */
  /* 新しいデータのチェック */
  if (m_image_origIn.isNew()) 
  {
    /* InPortデータの読み込み */
    m_image_origIn.read();

    /* サイズが変わったときだけ再生成する */
    if(m_in_height != m_image_orig.height || m_in_width != m_image_orig.width)
    {
      printf("[onExecute] Size of input image is not match!\n");

      m_in_height = m_image_orig.height;
      m_in_width  = m_image_orig.width;



      
      
	  m_image_buff.create(cv::Size(m_in_width, m_in_height), CV_8UC3);
	  m_hsvImage.create(cv::Size(m_in_width, m_in_height), CV_8UC3);
	  m_hueImage.create(cv::Size(m_in_width, m_in_height), CV_8UC1);
	  m_saturationImage.create(cv::Size(m_in_width, m_in_height), CV_8UC1);
	  m_valueImage.create(cv::Size(m_in_width, m_in_height), CV_8UC1);
	  m_mergeImage.create(cv::Size(m_in_width, m_in_height), CV_8UC3);
	  m_destinationImage.create(cv::Size(m_in_width, m_in_height), CV_8UC3);
    }

    /* InPortの画像データをIplImageのimageDataにコピー */
    memcpy(m_image_buff.data,(void *)&(m_image_orig.pixels[0]),m_image_orig.pixels.length());

    // Anternative actions

    /* BGRからHSVに変換する */
    cv::cvtColor(m_image_buff, m_hsvImage, CV_BGR2HSV);

    /* HSV画像をH、S、V画像に分ける */
	std::vector<cv::Mat> tmp;
	tmp.push_back(m_hueImage);
	tmp.push_back(m_saturationImage);
	tmp.push_back(m_valueImage);
	cv::split(m_hsvImage, tmp);

    /* HとSの値を変更する */
	m_hueImage.setTo(cv::Scalar(m_nHue));
	m_saturationImage.setTo(cv::Scalar(m_nSaturation));
    
    

    /* 3チャンネルを結合 */
	tmp.clear();
	tmp.push_back(m_hueImage);
	tmp.push_back(m_saturationImage);
	tmp.push_back(m_valueImage);
	cv::merge(tmp, m_mergeImage);

    /* HSVからBGRに変換する */
    cv::cvtColor(m_mergeImage, m_destinationImage, CV_HSV2BGR);

    /* 画像データのサイズ取得 */
	int len = m_destinationImage.channels() * m_destinationImage.size().width * m_destinationImage.size().height;
          
    /* 画面のサイズ情報を入れる */
    m_image_sepia.pixels.length(len);        
	m_image_sepia.width = m_destinationImage.size().width;
	m_image_sepia.height = m_destinationImage.size().height;

    /* 反転した画像データをOutPortにコピー */
    memcpy((void *)&(m_image_sepia.pixels[0]), m_destinationImage.data,len);

    m_image_sepiaOut.write();
  }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Sepia::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Sepia::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Sepia::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Sepia::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Sepia::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void SepiaInit(RTC::Manager* manager)
  {
    coil::Properties profile(sepia_spec);
    manager->registerFactory(profile,
                             RTC::Create<Sepia>,
                             RTC::Delete<Sepia>);
  }
  
};


