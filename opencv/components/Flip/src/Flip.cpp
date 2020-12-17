// -*- C++ -*-
/*!
 * @file  Flip.cpp
 * @brief Flip image component
 * @date $Date$
 *
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * $Id$
 */

#include "Flip.h"
#include <iostream>
using namespace std;

// Module specification
// <rtc-template block="module_spec">
static const char* flip_spec[] =
  {
    "implementation_id", "Flip",
    "type_name",         "Flip",
    "description",       "Flip image component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.flip_mode", "1",

    // Widget
    "conf.__widget__.flip_mode", "radio",
    // Constraints
    "conf.__constraints__.flip_mode", "(-1,0,1)",

    "conf.__type__.flip_mode", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Flip::Flip(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("original_image", m_image_orig),
    m_image_flipOut("fliped_image", m_image_flip)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Flip::~Flip()
{
}



RTC::ReturnCode_t Flip::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("original_image", m_image_origIn);

  // Set OutPort buffer
  addOutPort("fliped_image", m_image_flipOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("flip_mode", m_flip_mode, "1");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Flip::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Flip::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Flip::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Flip::onActivated(RTC::UniqueId ec_id)
{
  /* OutPortの画面サイズの初期化 */
  m_image_flip.width = 0;
  m_image_flip.height = 0;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t Flip::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t Flip::onExecute(RTC::UniqueId ec_id)
{
  // 新しいデータのチェック
  if (m_image_origIn.isNew())
  {
    // InPortデータの読み込み
    m_image_origIn.read();

    // InPortとOutPortの画面サイズ処理およびイメージ用メモリの確保
    if (m_image_orig.width != m_image_flip.width || m_image_orig.height != m_image_flip.height)
    {
      m_image_flip.width = m_image_orig.width;
      m_image_flip.height = m_image_orig.height;
    }

    cv::Mat m_imageBuff(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC3, (void *)&(m_image_orig.pixels[0]));
    cv::Mat m_flipImageBuff;

    // InPortからの画像データを反転する。 m_flipMode 0: X軸周り, 1: Y軸周り, -1: 両方の軸周り
    cv::flip(m_imageBuff, m_flipImageBuff, m_flip_mode);

    // 画像データのサイズ取得
    int len = m_flipImageBuff.channels() * m_flipImageBuff.cols * m_flipImageBuff.rows;
    m_image_flip.pixels.length(len);

    // 反転した画像データをOutPortにコピー
    memcpy((void *)&(m_image_flip.pixels[0]), m_flipImageBuff.data, len);

    // 反転した画像データをOutPortから出力する。
    m_image_flipOut.write();
  }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Flip::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Flip::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Flip::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Flip::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Flip::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void FlipInit(RTC::Manager* manager)
  {
    coil::Properties profile(flip_spec);
    manager->registerFactory(profile,
                             RTC::Create<Flip>,
                             RTC::Delete<Flip>);
  }

};


