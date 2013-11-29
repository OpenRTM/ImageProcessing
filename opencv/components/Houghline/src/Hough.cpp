// -*- C++ -*-
/*!
 * @file  Hough.cpp
 * @brief Hough line component
 * @date $Date$
 *
 * $Id$
 */

#include "Hough.h"

// Module specification
// <rtc-template block="module_spec">
static const char* hough_spec[] =
  {
    "implementation_id", "Hough",
    "type_name",         "Hough",
    "description",       "Hough line component",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.canny_thresld1", "100",
    "conf.default.canny_thresld2", "100",
    "conf.default.hough_method", "PROBABILISTIC",
    "conf.default.hough_thresld", "50",
    "conf.default.hough_param1", "50",
    "conf.default.hough_param2", "10",
    "conf.default.line_color_R", "255",
    "conf.default.line_color_G", "0",
    "conf.default.line_color_B", "0",
    "conf.default.line_thickness", "2",
    "conf.default.line_type", "8",
    // Widget
    "conf.__widget__.canny_thresld1", "slider.1",
    "conf.__widget__.canny_thresld2", "slider.1",
    "conf.__widget__.hough_method", "radio",
    "conf.__widget__.hough_thresld", "slider.1",
    "conf.__widget__.hough_param1", "text",
    "conf.__widget__.hough_param2", "text",
    "conf.__widget__.line_color_R", "text",
    "conf.__widget__.line_color_G", "text",
    "conf.__widget__.line_color_B", "text",
    "conf.__widget__.line_thickness", "text",
    "conf.__widget__.line_type", "radio",
    // Constraints
    "conf.__constraints__.canny_thresld1", "0<=x<=255",
    "conf.__constraints__.canny_thresld2", "0<=x<=255",
    "conf.__constraints__.hough_method", "(PROBABILISTIC,STANDARD,MULTI_SCALE)",
    "conf.__constraints__.hough_thresld", "0<=x<=255",
    "conf.__constraints__.line_type", "(8,4,CV_AA)",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Hough::Hough(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("origin_image", m_image_orig),
    m_image_houghOut("houghline_image", m_image_hough)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Hough::~Hough()
{
}



RTC::ReturnCode_t Hough::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("origin_image", m_image_origIn);
  
  // Set OutPort buffer
  addOutPort("houghline_image", m_image_houghOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("canny_thresld1", m_canny_threshold1, "100");
  bindParameter("canny_thresld2", m_canny_threshold2, "100");
  bindParameter("hough_method", m_hough_method, "PROBABILISTIC");
  bindParameter("hough_thresld", m_hough_threshold, "50");
  bindParameter("hough_param1", m_hough_param1, "50");
  bindParameter("hough_param2", m_hough_param2, "10");
  bindParameter("line_color_R", m_line_color_R, "255");
  bindParameter("line_color_G", m_line_color_G, "0");
  bindParameter("line_color_B", m_line_color_B, "0");
  bindParameter("line_thickness", m_line_thickness, "2");
  bindParameter("line_type", m_line_type, "8");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Hough::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Hough::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Hough::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Hough::onActivated(RTC::UniqueId ec_id)
{
  //  イメージ用メモリの初期化
  imageBuff = NULL;
  grayImage = NULL;
  edgeImage = NULL;
  hough = NULL;
  houghImage = NULL;

  m_in_height        = 0;
  m_in_width         = 0;
  lines = NULL;
  len=0;
  
  debug_method = -1;
  debug_type = -1;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t Hough::onDeactivated(RTC::UniqueId ec_id)
{
  if(imageBuff != NULL)
  {
    // イメージ用メモリの解放
    cvReleaseImage(&imageBuff);
    cvReleaseImage(&grayImage);
    cvReleaseImage(&edgeImage);
    cvReleaseImage(&hough);
    cvReleaseImage(&houghImage);
  }

  return RTC::RTC_OK;
}


RTC::ReturnCode_t Hough::onExecute(RTC::UniqueId ec_id)
{
  //  新しいデータのチェック
  if(m_image_origIn.isNew())
  {
    //  InPortデータの読み込み
    m_image_origIn.read();

    //  サイズが変わったときだけ再生成する
    if(m_image_orig.width != m_in_width || m_image_orig.height != m_in_height)
    {
      m_in_width = m_image_orig.width;
      m_in_height = m_image_orig.height;

      //  InPortのイメージサイズが変更された場合
      if(imageBuff != NULL)
      {
        cvReleaseImage(&imageBuff);
        cvReleaseImage(&grayImage);
        cvReleaseImage(&edgeImage);
        cvReleaseImage(&hough);
        cvReleaseImage(&houghImage);
      }

      //  イメージ用メモリの確保
      imageBuff = cvCreateImage( cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3 );
      grayImage = cvCreateImage( cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1 );
      edgeImage = cvCreateImage( cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1 );
      hough = cvCreateImage( cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3 );
      houghImage = cvCreateImage( cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3 );
    }

    // InPortの画面データをコピー
    memcpy( imageBuff->imageData, (void *)&(m_image_orig.pixels[0]), m_image_orig.pixels.length() );

    //  RGBからグレースケールに変換
    cvCvtColor( imageBuff, grayImage, CV_RGB2GRAY );

    //  ハフ変換に必要なメモリ領域
    CvMemStorage *storage = cvCreateMemStorage( 0 );

    //エッジ抽出を行う
    cvCanny( grayImage, edgeImage, m_canny_threshold1, m_canny_threshold2, APERTURE_SIZE );

    //グレースケールからRGBに変換する
    cvCvtColor( edgeImage, houghImage, CV_GRAY2RGB );

    //ハフ変換により直線の抽出を行う
    int hough_method;
    if ( m_hough_method == "PROBABILISTIC" )
    {
      //確率的ハフ変換
      hough_method = CV_HOUGH_PROBABILISTIC;
    }
    else if ( m_hough_method == "STANDARD" )
    {
      //標準的ハフ変換
      hough_method = CV_HOUGH_STANDARD;
    }
    else
    {
      //マルチスケール型の古典的ハフ変換
      hough_method = CV_HOUGH_MULTI_SCALE;
    }
    if ( hough_method != debug_method )
    {
      std::cout << "hough_method = " << hough_method << std::endl;
      debug_method = hough_method;
    }
    lines = cvHoughLines2( edgeImage, storage, hough_method, RHO, THETA, m_hough_threshold, m_hough_param1, m_hough_param2 );

    //抽出された直線を描く
    int line_type;
    if ( m_line_type == "CV_AA" )
    {
      line_type = CV_AA;
    }
    else
    {
      line_type = atoi( m_line_type.c_str() );
    }
    if ( line_type != debug_type )
    {
      std::cout << "line_type = " << line_type << std::endl;
      debug_type = line_type;
    }
    for ( int i = 0; i < lines->total; i++ ) {
      CvPoint *line = ( CvPoint* )cvGetSeqElem( lines, i );
      cvLine( houghImage, line[0], line[1], CV_RGB( m_line_color_R, m_line_color_G, m_line_color_B ), m_line_thickness, line_type, SHIFT );
    }

    //  画像データのサイズ取得
    len = houghImage->nChannels * houghImage->width * houghImage->height;
    m_image_hough.pixels.length(len);
    m_image_hough.width  = houghImage->width;
    m_image_hough.height = houghImage->height;

    //  反転した画像データをOutPortにコピー
    memcpy( (void *)&(m_image_hough.pixels[0]), houghImage->imageData, len );

    //  反転した画像データをOutPortから出力
    m_image_houghOut.write();

    //  ハフ変換に使用したメモリ解放
    cvReleaseMemStorage(&storage);
  }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Hough::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Hough::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Hough::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Hough::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Hough::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void HoughInit(RTC::Manager* manager)
  {
    coil::Properties profile(hough_spec);
    manager->registerFactory(profile,
                             RTC::Create<Hough>,
                             RTC::Delete<Hough>);
  }
  
};


