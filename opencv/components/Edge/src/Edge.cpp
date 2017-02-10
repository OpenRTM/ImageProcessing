// -*- C++ -*-
/*!
 * @file  Edge.cpp
 * @brief Edge image component
 * @date $Date$
 *
 * $Id$
 */

#include "Edge.h"
#include <iostream>

using namespace std;

// Module specification
// <rtc-template block="module_spec">
static const char* edge_spec[] =
  {
    "implementation_id", "Edge",
    "type_name",         "Edge",
    "description",       "Edge image component",
    "version",           "1.2.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.sobel_x_size", "3",
    "conf.default.sobel_y_size", "3",
    "conf.default.laplacian_size", "3",
    // Widget
    "conf.__widget__.sobel_x_size", "radio",
    "conf.__widget__.sobel_y_size", "radio",
    "conf.__widget__.laplacian_size", "radio",
    // Constraints
    "conf.__constraints__.sobel_x_size", "(1,3,5,7)",
    "conf.__constraints__.sobel_y_size", "(1,3,5,7)",
    "conf.__constraints__.laplacian_size", "(1,3,5,7)",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Edge::Edge(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("original_image", m_image_orig),
    m_image_edge_sobel_xOut("Edge_image_sobel_x", m_image_edge_sobel_x),
    m_image_edge_sobel_yOut("Edge_image_sobel_y", m_image_edge_sobel_y),
    m_image_edge_LAPLACIANOut("Edge_image_LAPLACIAN", m_image_edge_LAPLACIAN)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Edge::~Edge()
{
}



RTC::ReturnCode_t Edge::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("original_image", m_image_origIn);
  
  // Set OutPort buffer
  addOutPort("Edge_image_sobel_x", m_image_edge_sobel_xOut);
  addOutPort("Edge_image_sobel_y", m_image_edge_sobel_yOut);
  addOutPort("Edge_image_LAPLACIAN", m_image_edge_LAPLACIANOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("sobel_x_size", m_sobel_x_size, "3");
  bindParameter("sobel_y_size", m_sobel_y_size, "3");
  bindParameter("laplacian_size", m_laplacian_size, "3");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Edge::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Edge::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Edge::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Edge::onActivated(RTC::UniqueId ec_id)
{


  /* OutPort画面サイズの初期化 */
  m_image_edge_sobel_x.width = m_image_edge_sobel_y.width = m_image_edge_LAPLACIAN.width = 0;
  m_image_edge_sobel_x.height = m_image_edge_sobel_y.height = m_image_edge_LAPLACIAN.height = 0;

  len = 0;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t Edge::onDeactivated(RTC::UniqueId ec_id)
{



  return RTC::RTC_OK;
}


RTC::ReturnCode_t Edge::onExecute(RTC::UniqueId ec_id)
{
  /* 新しいデータのチェック */
  if(m_image_origIn.isNew()){
    /* InPortデータの読み込み */
    m_image_origIn.read();



    /* InPortの画面データをコピー */
	cv::Mat imageBuff(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC3, (void *)&(m_image_orig.pixels[0]));
    //memcpy( imageBuff.data, (void *)&(m_image_orig.pixels[0]), m_image_orig.pixels.length() );
	cv::Mat grayImage;
	cv::Mat destinationImage_x;
	cv::Mat destinationImage_y;
	cv::Mat destinationImage_LAPLACIAN;
	cv::Mat destinationEdge;
	cv::Mat edgeImage;

    /* RGBからグレースケールに変換 */
    cv::cvtColor( imageBuff, grayImage, CV_RGB2GRAY );

    /* Sobel_X */
    /* X方向のSobelオペレータをかける */
	cv::Sobel(grayImage, destinationImage_x, destinationImage_x.type(), 1, 0, m_sobel_x_size);

    /* 16ビットの符号ありデータを8ビットの符号なしデータに変換する */
    cv::convertScaleAbs( destinationImage_x, destinationEdge, SCALE, SHIFT );

    /* グレースケールからRGBに変換 */
    cv::cvtColor( destinationEdge, edgeImage, CV_GRAY2RGB );

    /* 画像データのサイズ取得 */
	len = edgeImage.channels() * edgeImage.size().width * edgeImage.size().height;
	
	m_image_edge_sobel_x.width = m_image_edge_sobel_y.width = m_image_edge_LAPLACIAN.width = m_image_orig.width;
	m_image_edge_sobel_x.height = m_image_edge_sobel_y.height = m_image_edge_LAPLACIAN.height = m_image_orig.height;

    m_image_edge_sobel_x.pixels.length(len);

    /* 反転した画像データをOutPortにコピー */
    memcpy( (void *)&(m_image_edge_sobel_x.pixels[0]), edgeImage.data, len );

    /* 反転した画像データをOutPortから出力 */
    m_image_edge_sobel_xOut.write();


    /* Sobel_Y */
    /* Y方向のSobelオペレータをかける */
	cv::Sobel(grayImage, destinationImage_y, destinationImage_y.type(), 0, 1, m_sobel_y_size);

    cv::convertScaleAbs( destinationImage_y, destinationEdge, SCALE, SHIFT );

    cv::cvtColor( destinationEdge, edgeImage, CV_GRAY2RGB );

	len = edgeImage.channels() * edgeImage.size().width * edgeImage.size().height;
    m_image_edge_sobel_y.pixels.length(len);
    memcpy( (void *)&(m_image_edge_sobel_y.pixels[0]), edgeImage.data, len );

    m_image_edge_sobel_yOut.write();


    // LAPLACIAN
	cv::Laplacian(grayImage, destinationImage_LAPLACIAN, m_laplacian_size);

    cv::convertScaleAbs( destinationImage_LAPLACIAN, destinationEdge, SCALE, SHIFT );

    cv::cvtColor( destinationEdge, edgeImage, CV_GRAY2RGB );

	len = edgeImage.channels() * edgeImage.size().width * edgeImage.size().height;
	
    m_image_edge_LAPLACIAN.pixels.length(len);
    memcpy( (void *)&(m_image_edge_LAPLACIAN.pixels[0]), edgeImage.data, len );

    m_image_edge_LAPLACIANOut.write();

  }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Edge::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Edge::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Edge::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Edge::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Edge::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void EdgeInit(RTC::Manager* manager)
  {
    coil::Properties profile(edge_spec);
    manager->registerFactory(profile,
                             RTC::Create<Edge>,
                             RTC::Delete<Edge>);
  }
  
};


