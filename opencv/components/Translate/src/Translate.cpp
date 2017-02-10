// -*- C++ -*-
/*!
 * @file  Translate.cpp
 * @brief Translate image component
 * @date $Date$
 *
 * $Id$
 */

#include "Translate.h"

// Module specification
// <rtc-template block="module_spec">
static const char* translate_spec[] =
  {
    "implementation_id", "Translate",
    "type_name",         "Translate",
    "description",       "Translate image component",
    "version",           "1.2.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.Translate_x", "50",
    "conf.default.Translate_y", "50",
    // Widget
    "conf.__widget__.Translate_x", "text",
    "conf.__widget__.Translate_y", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Translate::Translate(RTC::Manager* manager)
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
Translate::~Translate()
{
}



RTC::ReturnCode_t Translate::onInitialize()
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
  bindParameter("Translate_x", m_nTransX, "50");
  bindParameter("Translate_y", m_nTransY, "50");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Translate::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Translate::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Translate::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Translate::onActivated(RTC::UniqueId ec_id)
{

  /* 行列を生成する */
  m_transformMatrix.create( 2, 3, CV_32FC1);

  return RTC::RTC_OK;
}


RTC::ReturnCode_t Translate::onDeactivated(RTC::UniqueId ec_id)
{
  



  

  return RTC::RTC_OK;
}


RTC::ReturnCode_t Translate::onExecute(RTC::UniqueId ec_id)
{
  // Common CV process
  /* 新しいデータのチェック */
  if (m_image_origIn.isNew()) 
  {
    /* InPortデータの読み込み */
    m_image_origIn.read();



    /* InPortの画像データをIplImageのimageDataにコピー */
	cv::Mat m_image_buff(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC3, (void *)&(m_image_orig.pixels[0]));
    //memcpy(m_image_buff.data,(void *)&(m_image_orig.pixels[0]),m_image_orig.pixels.length());
	cv::Mat m_image_dest;   /* 結果出力用IplImage */

	cv::Mat m_transformMatrix;


    // Anternative process
    cv::Point2f original[3];   /* 変換前座標 */
    cv::Point2f Translate[3];  /* 変換後座標 */

    /* 変換前の座標を設定する */
	original[0] = cv::Point2f(0, 0);
	original[1] = cv::Point2f(m_image_buff.size().width, 0);
	original[2] = cv::Point2f(0, m_image_buff.size().height);

    /* 変換後の座標を設定する */
	Translate[0] = cv::Point2f(m_nTransX, m_nTransY);
	Translate[1] = cv::Point2f(m_nTransX + m_image_buff.size().width, m_nTransY);
	Translate[2] = cv::Point2f(m_nTransX, m_nTransY + m_image_buff.size().height);

    /* 変換行列を求める */
	m_transformMatrix = cv::getAffineTransform(original, Translate);

    /* 変換行列を反映させる */
	warpAffine(m_image_buff, m_image_dest, m_transformMatrix, m_image_dest.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    // Common process
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
RTC::ReturnCode_t Translate::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Translate::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Translate::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Translate::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Translate::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void TranslateInit(RTC::Manager* manager)
  {
    coil::Properties profile(translate_spec);
    manager->registerFactory(profile,
                             RTC::Create<Translate>,
                             RTC::Delete<Translate>);
  }
  
};


