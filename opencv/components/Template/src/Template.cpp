// -*- C++ -*-
/*!
 * @file  Template.cpp
 * @brief Template image component
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

#include "Template.h"
#include <iostream>
using namespace std;

// Module specification
// <rtc-template block="module_spec">
static const char* template_spec[] =
  {
    "implementation_id", "Template",
    "type_name",         "Template",
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
Template::Template(RTC::Manager* manager)
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
Template::~Template()
{
}



RTC::ReturnCode_t Template::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("original_image", m_image_origIn);

  // Set OutPort buffer
  addOutPort("template", m_image_templateOut);
  addOutPort("picture", m_image_pictureOut);

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
RTC::ReturnCode_t Template::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Template::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Template::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Template::onActivated(RTC::UniqueId ec_id)
{
  /* 対象画像用メモリの初期化 */
  templateID = "";
  templateWidth = 0;
  templateHeight = 0;

  /* OutPort１の画面サイズの初期化 */
  m_image_template.width = 0;
  m_image_template.height = 0;

  len = 0;
  flag = 0;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t Template::onDeactivated(RTC::UniqueId ec_id)
{
  if (!templateImage.empty())
  {
	  templateImage.release();
  }
  if (!templateGrayImage.empty())
  {
	  templateGrayImage.release();
  }
  if (!templateBinaryImage.empty())
  {
	  templateBinaryImage.release();
  }

  return RTC::RTC_OK;
}


RTC::ReturnCode_t Template::onExecute(RTC::UniqueId ec_id)
{
  /* 新しいデータのチェック */
  if(m_image_origIn.isNew())
  {
    /* InPortデータの読み込み */
    m_image_origIn.read();

    if (templateID != m_img_path)
    {
      /* 対象画像を読み込む */
      templateImage = cv::imread(m_img_path, IMREAD_ANYDEPTH | IMREAD_ANYCOLOR);
    }

    if( templateImage.empty() )
    {
      templateID = "";
      templateWidth = templateHeight = 0;
    }

    /* 対象画像チェック */
    /* 対象画像のPathとか名が無い場合テンプレートマッチングしなくて入力されたイメージをそのまま出力 */
    if (!templateImage.empty() && templateID != m_img_path)
    {
      /* フラッグ設定(正しい対象画像が入力） */
      flag = 1;
      templateID = m_img_path;
      templateWidth = templateImage.size().width;
      templateHeight = templateImage.size().height;

      cout << "templateID : "<<templateID<<endl;
      cout << "template - width :"<<templateWidth<<endl;
      cout << "template - height :"<<templateHeight<<endl;

      /* RGBからグレースケールに変換する */
      cv::cvtColor( templateImage, templateGrayImage, COLOR_RGB2GRAY );

      /* グレースケールから2値に変換する */
      cv::threshold( templateGrayImage, templateBinaryImage, THRESHOLD, THRESHOLD_MAX_VALUE, cv::THRESH_BINARY );

      /* OutPort２用の画面サイズ初期化 */
      m_image_picture.width = templateImage.size().width;
      m_image_picture.height = templateImage.size().height;
    }

    /* InPortとOutPortの画面サイズ処理およびイメージ用メモリの確保(正しい対象画像が入れるとdifferenceMapImageが変換される-フラッグを見て判断） */
    m_image_template.width = m_image_orig.width;
    m_image_template.height = m_image_orig.height;

    /* InPortの画像データをコピー */
    cv::Mat imageBuff(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC3, (void *)&(m_image_orig.pixels[0]));
	
    cv::Mat sourceGrayImage;
    cv::Mat sourceBinaryImage;

    cv::Mat differenceMapImage;

    cv::Point minLocation;
	
    if( !templateImage.empty() )
    {
      /* RGBからグレースケールに変換する */
      cv::cvtColor( imageBuff, sourceGrayImage, COLOR_RGB2GRAY );

      /* グレースケールから2値に変換する */
      cv::threshold( sourceGrayImage, sourceBinaryImage, THRESHOLD, THRESHOLD_MAX_VALUE, cv::THRESH_BINARY );

      /* テンプレートマッチングを行う */
      cv::matchTemplate(sourceBinaryImage, templateBinaryImage, differenceMapImage, TM_SQDIFF);

      /* テンプレートが元画像のどの部分にあるのかという情報を得る */
      cv::Point max_pt;
      double maxVal;
      cv::minMaxLoc(differenceMapImage, NULL, NULL, &minLocation, NULL);
      
      /* 一致する場所を元画像に四角で描く */
      cv::rectangle(
      imageBuff,
      minLocation,
      cv::Point(minLocation.x + templateImage.size().width, minLocation.y + templateImage.size().height),
                  CV_RGB( 255, 0, 0 ),
                  LINE_THICKNESS,
                  LINE_TYPE,
                  SHIFT
      );

      /* 画像データのサイズ取得 */
      len = imageBuff.channels() * imageBuff.size().width * imageBuff.size().height;
      m_image_template.pixels.length(len);

      /* 反転した画像データをOutPortにコピー */
      memcpy((void *)&(m_image_template.pixels[0]), imageBuff.data, len);

      /* 反転した画像データをOutPortから出力 */
      m_image_templateOut.write();

      /* 対象画像データのサイズ取得 */
      len = templateImage.channels() * templateImage.size().width * templateImage.size().height;
      m_image_picture.pixels.length(len);

      /* 反転した対象画像データをOutPortにコピー */
      memcpy( (void *)&(m_image_picture.pixels[0]), templateImage.data, len );

      m_image_pictureOut.write();

    }else{

      /* 画像データのサイズ取得 */
      len = imageBuff.channels() * imageBuff.size().width * imageBuff.size().height;
      m_image_template.pixels.length(len);

      /* 反転した画像データをOutPortにコピー */
      memcpy( (void *)&(m_image_template.pixels[0]), imageBuff.data, len );

      m_image_templateOut.write();

    }

  }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Template::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Template::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Template::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Template::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Template::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void TemplateInit(RTC::Manager* manager)
  {
    coil::Properties profile(template_spec);
    manager->registerFactory(profile,
                             RTC::Create<Template>,
                             RTC::Delete<Template>);
  }

};


