// -*- C++ -*-
/*!
 * @file  Binarization.cpp
 * @brief Binarization image component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "Binarization.h"

#define THRESHOLD_MAX_VALUE	255   /* 2値化の際に使用する最大値 */

// Module specification
// <rtc-template block="module_spec">
static const char* binarization_spec[] =
  {
    "implementation_id", "Binarization",
    "type_name",         "Binarization",
    "description",       "Binarization image component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.threshold_level", "128",

    // Widget
    "conf.__widget__.threshold_level", "slider.1",
    // Constraints
    "conf.__constraints__.threshold_level", "0<=x<=255",

    "conf.__type__.threshold_level", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Binarization::Binarization(RTC::Manager* manager)
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
Binarization::~Binarization()
{
}



RTC::ReturnCode_t Binarization::onInitialize()
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
  bindParameter("threshold_level", m_nThresholdLv, "128");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Binarization::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Binarization::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Binarization::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Binarization::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t Binarization::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t Binarization::onExecute(RTC::UniqueId ec_id)
{
  /* Common CV process */
  /* 新しいデータのチェック */
  if (m_image_origIn.isNew()) 
  {
    /* InPortデータの読み込み */
    m_image_origIn.read();

    /* InPortの画像データをIplImageのimageDataにコピー */
    //memcpy(m_image_buff.data,(void *)&(m_image_orig.pixels[0]),m_image_orig.pixels.length());
    cv::Mat m_image_buff(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC3, (void *)&(m_image_orig.pixels[0]));
    cv::Mat m_image_gray;         // Grayscale image
    cv::Mat m_image_binary;       // Binary image
    cv::Mat m_image_dest;         // 結果出力用IplImage

    /* Anternative process */
    /* BGRからグレースケールに変換する */
    cv::cvtColor(m_image_buff, m_image_gray, cv::COLOR_BGR2GRAY);

    /* グレースケールから2値に変換する */
    cv::threshold(m_image_gray, m_image_binary, m_nThresholdLv, THRESHOLD_MAX_VALUE, cv::THRESH_BINARY);

    std::vector<cv::Mat> tmp;
    tmp.push_back(m_image_binary);
    tmp.push_back(m_image_binary);
    tmp.push_back(m_image_binary);
    /* Convert to 3channel image */
    cv::merge(tmp, m_image_dest);

    /* Common process */
    /* 画像データのサイズ取得 */
    int len = m_image_dest.channels() * m_image_dest.size().width * m_image_dest.size().height;
          
    /* 画面のサイズ情報を入れる */
    m_image_output.pixels.length(len);        
    m_image_output.width = m_image_dest.size().width;
    m_image_output.height = m_image_dest.size().height;

    /* 反転した画像データをOutPortにコピー */
    memcpy((void *)&(m_image_output.pixels[0]), m_image_dest.data,len);

    /* 反転した画像データをOutPortから出力する */
    m_image_outputOut.write();
  }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Binarization::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Binarization::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Binarization::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Binarization::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Binarization::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void BinarizationInit(RTC::Manager* manager)
  {
    coil::Properties profile(binarization_spec);
    manager->registerFactory(profile,
                             RTC::Create<Binarization>,
                             RTC::Delete<Binarization>);
  }

};


