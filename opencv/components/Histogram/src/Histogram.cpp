// -*- C++ -*-
/*!
 * @file  Histogram.cpp
 * @brief Histogram image component
 * @date $Date$
 *
 * This RT-Component source code is using the code in 
 * "OpenCVプログラミングブック" (OpenCV Programming book). 
 * Please refer: https://book.mynavi.jp/support/pc/opencv11/#F_DWN
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "Histogram.h"
#include <iostream>
using namespace std;

// Module specification
// <rtc-template block="module_spec">
static const char* histogram_spec[] =
  {
    "implementation_id", "Histogram",
    "type_name",         "Histogram",
    "description",       "Histogram image component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.brightness", "100",
    "conf.default.contrast", "100",

    // Widget
    "conf.__widget__.brightness", "slider.1",
    "conf.__widget__.contrast", "slider.1",
    // Constraints
    "conf.__constraints__.brightness", "0<=x<=200",
    "conf.__constraints__.contrast", "0<=x<=200",

    "conf.__type__.brightness", "int",
    "conf.__type__.contrast", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Histogram::Histogram(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("original_image", m_image_orig),
    m_image_histogramImageOut("histogram_image", m_image_histogramImage),
    m_image_histogramOut("histogram", m_image_histogram)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Histogram::~Histogram()
{
}



RTC::ReturnCode_t Histogram::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("original_image", m_image_origIn);

  // Set OutPort buffer
  addOutPort("histogram_image", m_image_histogramImageOut);
  addOutPort("histogram", m_image_histogramOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("brightness", m_brightness, "100");
  bindParameter("contrast", m_contrast, "100");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Histogram::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Histogram::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Histogram::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Histogram::onActivated(RTC::UniqueId ec_id)
{
  /* OutPort画面サイズの初期化 */
  /* ヒストグラムに描画される縦棒の数 */
  //histogramSize = 128;
  /* ヒストグラムの範囲 */
  //range_0[0] = 0;
  //range_0[1] = 256;

  /* ヒストグラム各次元の範囲を示す配列のポインタ */
  //ranges[0] = range_0 ;

  /* ヒストグラムに描画される縦棒の数 */
  histogramSize = 128;
  /* ヒストグラムの範囲 */
  range_0[0] = 0;
  range_0[1] = 256;

  /* 行列を生成 */
  lookUpTableMatrix.create(cv::Size(1, 256), CV_8UC1); /* 濃度対応行列 */
 
  return RTC::RTC_OK;
}


RTC::ReturnCode_t Histogram::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t Histogram::onExecute(RTC::UniqueId ec_id)
{
  /* 新しいデータのチェック */
  if(m_image_origIn.isNew())
  {
    /* InPortデータの読み込み */
    m_image_origIn.read();

    cv::Mat grayImage;
    cv::Mat destinationImage;
    cv::Mat histogramImage;
    cv::Mat histogramBarImage(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC3);
    cv::MatND histogram;
    
    m_image_histogram.width = m_image_histogramImage.width = m_image_orig.width;
    m_image_histogram.height = m_image_histogramImage.height = m_image_orig.height;
    bin_w = cvRound((double)histogramBarImage.size().width / histogramSize);

    /* InPortの画面データをコピー */
    cv::Mat imageBuff(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC3, (void *)&(m_image_orig.pixels[0]));
	
    /* RGBからグレースケールに変換 */
    cv::cvtColor( imageBuff, grayImage, COLOR_RGB2GRAY);

    int brightness = m_brightness - TRACKBAR_MAX_VALUE / 2;	//	輝度値
    int contrast = m_contrast - TRACKBAR_MAX_VALUE / 2;		//	コントラスト

    if ( contrast > 0 ) 
    {
      double delta = 127.0 * contrast / 100.0;
      double a = 255.0 / ( 255.0 - delta * 2 );
      double b = a * ( brightness - delta );
      for (int i = 0; i < 256; i++ )
      {
        /* 変換後の階調を求める */
        int v = cvRound( a * i + b );
        if( v < 0 ){
          v = 0;
        }
        if( v > 255 ){
          v = 255;
        }
        lookUpTableMatrix.at<uchar>(i) = (unsigned char)v;
      }
    } else {
      double delta = -128.0 * contrast / 100.0;
      double a = (256.0 - delta * 2.0) / 255.0;
      double b = a * brightness + delta;
      for(int i = 0; i < 256; i++ )
      {
        int v = cvRound( a * i + b);
        if( v < 0 ){
          v = 0;
        }
        if( v > 255 ){
          v = 255;
        }
        lookUpTableMatrix.at<uchar>(i) = (unsigned char)v;
      }
    }
	
    /* 濃度対応行列を用いた濃度階調変換を行う */
    cv::LUT(grayImage, lookUpTableMatrix, destinationImage);

    /* グレースケールからRGBに変換 */
    cv::cvtColor( destinationImage, histogramImage, COLOR_GRAY2RGB );

    /* 画像データのサイズ取得 */
    int len = histogramImage.channels() * histogramImage.size().width * histogramImage.size().height;
    m_image_histogramImage.pixels.length(len);

    /* 変転した画像データをOutPortにコピー */
    memcpy((void *)&(m_image_histogramImage.pixels[0]), histogramImage.data,len);

    /* 変転した画像データをOutPortから出力 */
    m_image_histogramImageOut.write();

    /* 画像のヒストグラムを計算する */
    int image_num = 1;
    int channels[] = { 0 };
    const float *ranges[] = { range_0 };

    /* ヒストグラム各次元の範囲を示す配列のポインタ */
    cv::calcHist(&destinationImage, image_num, channels, cv::Mat(), histogram, DIMENSIONS, &histogramSize, ranges);
	
    float max_value = 0;
    /* ヒストグラムを最大値によって正規化する */
    cv::normalize(histogram, histogram, (double)histogramBarImage.size().height);

    /* ヒストグラム画像を白で初期化する */
    histogramBarImage.setTo(cv::Scalar::all(255));
		
    /* ヒストグラムの縦棒を描画する */
    for ( int i = 0; i < histogramSize; i++ ) {
      cv::rectangle(
        histogramBarImage,
        cv::Point( i * bin_w, histogramBarImage.size().height ),
        cv::Point((i + 1) * bin_w, histogramBarImage.size().height - cvRound(histogram.at<float>(i))),
        cv::Scalar::all(0),
        LINE_THICKNESS,
        LINE_TYPE,
        SHIFT
      );
    }

    /* 画像データのサイズ取得 */
    len = histogramBarImage.channels() * histogramBarImage.size().width * histogramBarImage.size().height;
    m_image_histogram.pixels.length(len);

    /* 変転した画像データをOutPortにコピー */
    memcpy((void *)&(m_image_histogram.pixels[0]), histogramBarImage.data,len);

    /* 変転した画像データをOutPortから出力 */
    m_image_histogramOut.write();

  }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Histogram::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Histogram::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Histogram::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Histogram::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Histogram::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void HistogramInit(RTC::Manager* manager)
  {
    coil::Properties profile(histogram_spec);
    manager->registerFactory(profile,
                             RTC::Create<Histogram>,
                             RTC::Delete<Histogram>);
  }

};


