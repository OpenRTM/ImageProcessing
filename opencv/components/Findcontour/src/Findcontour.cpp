// -*- C++ -*-
/*!
 * @file  Findcontour.cpp
 * @brief Findcontour component
 * @date $Date$
 *
 * $Id$
 */

#include "Findcontour.h"
#include <iostream>
using namespace std;

// Module specification
// <rtc-template block="module_spec">
static const char* findcontour_spec[] =
  {
    "implementation_id", "Findcontour",
    "type_name",         "Findcontour",
    "description",       "Findcontour component",
    "version",           "1.1.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.threshold_level", "100",
    "conf.default.contour_level", "1",
    "conf.default.line_thickness", "2",
    "conf.default.line_type", "CV_AA",
    // Widget
    "conf.__widget__.threshold_level", "slider.1",
    "conf.__widget__.contour_level", "text",
    "conf.__widget__.line_thickness", "text",
    "conf.__widget__.line_type", "text",
    // Constraints
    "conf.__constraints__.threshold_level", "0<=x<=255",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Findcontour::Findcontour(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("original_image", m_image_orig),
    m_image_contourOut("contour_image", m_image_contour)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Findcontour::~Findcontour()
{
}



RTC::ReturnCode_t Findcontour::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("original_image", m_image_origIn);
  
  // Set OutPort buffer
  addOutPort("contour_image", m_image_contourOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("threshold_level", m_nThresholdLv, "100");
  bindParameter("contour_level", m_nContourLv, "1");
  bindParameter("line_thickness", m_nLineThickness, "2");
  bindParameter("line_type", m_nLineType, "CV_AA");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Findcontour::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Findcontour::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Findcontour::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Findcontour::onActivated(RTC::UniqueId ec_id)
{


  /* OutPort画面サイズの初期化 */
  m_image_contour.width = 0;
  m_image_contour.height = 0;

  find_contour.clear();
  red = cv::Scalar(255, 0, 0);
  green = cv::Scalar(0, 255, 0);

  return RTC::RTC_OK;
}


RTC::ReturnCode_t Findcontour::onDeactivated(RTC::UniqueId ec_id)
{


  if (!imageBuff.empty())
  {
	  imageBuff.release();
  }
  if (!grayImage.empty())
  {
	  grayImage.release();
  }
  if (!binaryImage.empty())
  {
	  binaryImage.release();
  }
  if (!contourImage.empty())
  {
	  contourImage.release();
  }


  return RTC::RTC_OK;
}


RTC::ReturnCode_t Findcontour::onExecute(RTC::UniqueId ec_id)
{
  /* 新しいデータのチェック */
  if(m_image_origIn.isNew())
  {
    /* InPortデータの読み込み */
    m_image_origIn.read();

    /* InPortとOutPortの画面サイズ処理およびイメージ用メモリ確保 */
    if( m_image_orig.width != m_image_contour.width || m_image_orig.height != m_image_contour.height)
    {
      m_image_contour.width = m_image_orig.width;
      m_image_contour.height = m_image_orig.height;



      /* イメージ用メモリの確保 */

	  imageBuff.create(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC3);
	  grayImage.create(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC1);
	  binaryImage.create(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC1);
	  contourImage.create(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC3);

    }

    /* InPortの画面データをコピー */
    memcpy( imageBuff.data, (void *)&(m_image_orig.pixels[0]), m_image_orig.pixels.length() );
    memcpy( contourImage.data, (void *)&(m_image_orig.pixels[0]), m_image_orig.pixels.length() );

    /* RGBからグレースケールに変換 */
    cv::cvtColor( imageBuff, grayImage, CV_RGB2GRAY);

    /* グレースケールから2値に変換する */
    cv::threshold( grayImage, binaryImage, m_nThresholdLv, THRESHOLD_MAX_VALUE, cv::THRESH_BINARY );


	std::vector<cv::Vec4i> hierarchy;
    /* 2値画像中の輪郭を見つけ、その数を返す */
    cv::findContours( 
          binaryImage,          /* 入力画像(８ビットシングルチャンネル） */
		  find_contour,        /* 一番外側の輪郭へのポインタへのポインタ */
		  hierarchy,
		  CV_RETR_CCOMP,         /* 抽出モード */
          CV_CHAIN_APPROX_NONE, /* 推定手法 */
          cv::Point( 0, 0 )       /* オフセット */
    );

	find_contour_num = find_contour.size();
	

	cv::drawContours(
		contourImage,     /* 輪郭を描画する画像 */
		find_contour,     /* 最初の輪郭へのポインタ */
		-1,
		green,              /* 外側輪郭線の色 */
		m_nLineThickness, /* 描画される輪郭線の太さ */
		8,
		hierarchy,
		2
		);

	cv::drawContours(
		contourImage,     /* 輪郭を描画する画像 */
		find_contour,     /* 最初の輪郭へのポインタ */
		-1,
		red,              /* 外側輪郭線の色 */
		m_nLineThickness, /* 描画される輪郭線の太さ */
		8,
		hierarchy,
		1
		);

	
	

    /* 画像データのサイズ取得 */
	int len = contourImage.channels() * contourImage.size().width * contourImage.size().height;
    m_image_contour.pixels.length(len);

    /* 変転した画像データをOutPortにコピー */
    memcpy((void *)&(m_image_contour.pixels[0]), contourImage.data, len);

    /* 変転した画像データをOutPortから出力 */
    m_image_contourOut.write();


  }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Findcontour::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Findcontour::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Findcontour::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Findcontour::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Findcontour::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void FindcontourInit(RTC::Manager* manager)
  {
    coil::Properties profile(findcontour_spec);
    manager->registerFactory(profile,
                             RTC::Create<Findcontour>,
                             RTC::Delete<Findcontour>);
  }
  
};


