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
    "version",           "1.2.0",
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



  return RTC::RTC_OK;
}


RTC::ReturnCode_t Sepia::onDeactivated(RTC::UniqueId ec_id)
{
  /* イメージ用メモリの解放 */




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
    


	cv::Mat m_image_buff(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC3, (void *)&(m_image_orig.pixels[0]));       /* Original Image */

	cv::Mat m_hsvImage;         /* HSV画像用IplImage */
	cv::Mat m_hueImage;         /* 色相(H)情報用IplImage */
	cv::Mat m_saturationImage;  /* 彩度(S)情報用IplImage */
	cv::Mat m_valueImage;       /* 明度(V)情報用IplImage */

	cv::Mat m_mergeImage;       /* マージ用IplImage */
	cv::Mat m_destinationImage; /* 結果出力用IplImage */

    /* InPortの画像データをIplImageのimageDataにコピー */
   // memcpy(m_image_buff.data,(void *)&(m_image_orig.pixels[0]),m_image_orig.pixels.length());

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


