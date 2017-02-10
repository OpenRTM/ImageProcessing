// -*- C++ -*-
/*!
 * @file  Perspective.cpp
 * @brief Perspective image component
 * @date $Date$
 *
 * $Id$
 */

#include "Perspective.h"

// Module specification
// <rtc-template block="module_spec">
static const char* perspective_spec[] =
  {
    "implementation_id", "Perspective",
    "type_name",         "Perspective",
    "description",       "Perspective image component",
    "version",           "1.2.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Perspective::Perspective(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("original_image", m_image_orig),
    m_image_outOut("out_image", m_image_out)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Perspective::~Perspective()
{
}



RTC::ReturnCode_t Perspective::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("original_image", m_image_origIn);
  
  // Set OutPort buffer
  addOutPort("out_image", m_image_outOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Perspective::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Perspective::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Perspective::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Perspective::onActivated(RTC::UniqueId ec_id)
{




  /* 行列を生成する */
  m_perspectiveMatrix.create(cv::Size(3, 3), CV_8UC1);

  return RTC::RTC_OK;
}


RTC::ReturnCode_t Perspective::onDeactivated(RTC::UniqueId ec_id)
{



  if (!m_perspectiveMatrix.empty())
  {
	  m_perspectiveMatrix.release();
  }


  return RTC::RTC_OK;
}


RTC::ReturnCode_t Perspective::onExecute(RTC::UniqueId ec_id)
{
  /* Common CV actions */
  /* 新しいデータのチェック */
  if (m_image_origIn.isNew()) 
  {
    /* InPortデータの読み込み */
    m_image_origIn.read();


	cv::Mat m_image_dest;         /* 結果出力用IplImage */

	cv::Mat m_image_buff(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC3, (void *)&(m_image_orig.pixels[0]));

    // Anternative actions
	std::vector<cv::Point2f>  original;   /* 変換前座標 */
	std::vector<cv::Point2f> translate;  /* 変換後座標 */
	

    /* 変換前の座標を設定する */
	original.push_back(cv::Point2f(0, 0));
	original.push_back(cv::Point2f(m_image_buff.size().width, 0));
	original.push_back(cv::Point2f(0, m_image_buff.size().height));
	original.push_back(cv::Point2f(m_image_buff.size().width, m_image_buff.size().height));


    /* 変換後の座標を設定する */
	translate.push_back(cv::Point2f(m_image_buff.size().width / 5 * 1, m_image_buff.size().height / 5 * 2));
	translate.push_back(cv::Point2f(m_image_buff.size().width / 5 * 4, m_image_buff.size().height / 5 * 2));
	translate.push_back(cv::Point2f(0, m_image_buff.size().height / 5 * 4));
	translate.push_back(cv::Point2f(m_image_buff.size().width, m_image_buff.size().height / 5 * 4));

	// 変換前の画像での座標
	const cv::Point2f src_pt[] = {
		cv::Point2f(88.0, 81.0),
		cv::Point2f(1.2.0, 436.0),
		cv::Point2f(420.0, 346.0),
		cv::Point2f(424, 131) };

	// 変換後の画像での座標
	const cv::Point2f dst_pt[] = {
		cv::Point2f(0.0, 0.0),
		cv::Point2f(0, 0 - 1 - 200),
		cv::Point2f(0 - 1, 0 - 1 - 200),
		cv::Point2f(0 - 1, 0) };
	

    /* 変換行列を求める */	
	m_perspectiveMatrix = cv::getPerspectiveTransform(original, translate);

    /* 変換行列を反映させる */
	cv::warpPerspective(m_image_buff, m_image_dest, m_perspectiveMatrix,
						m_image_dest.size());
                        //CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS, cv::Scalar::all(255) );

    /* 画像データのサイズ取得 */
	int len = m_image_dest.channels() * m_image_dest.size().width * m_image_dest.size().height;
          
    /* 画面のサイズ情報を入れる */
    m_image_out.pixels.length(len);        
	m_image_out.width = m_image_dest.size().width;
	m_image_out.height = m_image_dest.size().height;

    /* 反転した画像データをOutPortにコピー */
    memcpy((void *)&(m_image_out.pixels[0]), m_image_dest.data,len);

    /* 反転した画像データをOutPortから出力する */
    m_image_outOut.write();
  }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Perspective::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Perspective::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Perspective::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Perspective::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Perspective::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void PerspectiveInit(RTC::Manager* manager)
  {
    coil::Properties profile(perspective_spec);
    manager->registerFactory(profile,
                             RTC::Create<Perspective>,
                             RTC::Delete<Perspective>);
  }
  
};


