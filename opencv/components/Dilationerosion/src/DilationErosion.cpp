// -*- C++ -*-
/*!
 * @file  DilationErosion.cpp
 * @brief Dilation and erosion component
 * @date $Date$
 *
 * $Id$
 */

#include "DilationErosion.h"

// Module specification
// <rtc-template block="module_spec">
static const char* dilationerosion_spec[] =
  {
    "implementation_id", "DilationErosion",
    "type_name",         "DilationErosion",
    "description",       "Dilation and erosion component",
    "version",           "1.1.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.dilation_count", "3",
    "conf.default.erosion_count", "3",
    "conf.default.threshold", "128",
    // Widget
    "conf.__widget__.dilation_count", "text",
    "conf.__widget__.erosion_count", "text",
    "conf.__widget__.threshold", "slider.1",
    // Constraints
    "conf.__constraints__.threshold", "0<=x<=255",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
DilationErosion::DilationErosion(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_img_origIn("original_image", m_img_orig),
    m_img_outOut("output_image", m_img_out),
    m_img_dilationOut("dilation_image", m_img_dilation),
    m_img_erosionOut("erosion_image", m_img_erosion)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
DilationErosion::~DilationErosion()
{
}



RTC::ReturnCode_t DilationErosion::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("original_image", m_img_origIn);
  
  // Set OutPort buffer
  addOutPort("output_image", m_img_outOut);
  addOutPort("dilation_image", m_img_dilationOut);
  addOutPort("erosion_image", m_img_erosionOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("dilation_count", m_count_dilation, "3");
  bindParameter("erosion_count", m_count_erosion, "3");
  bindParameter("threshold", m_nThreshold, "128");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t DilationErosion::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DilationErosion::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DilationErosion::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t DilationErosion::onActivated(RTC::UniqueId ec_id)
{

	
	return RTC::RTC_OK;
}


RTC::ReturnCode_t DilationErosion::onDeactivated(RTC::UniqueId ec_id)
{
	/* メモリ解放 */
	if (!m_image_buff.empty())
	{
		m_image_buff.release();
	}
	if (!m_gray_buff.empty())
	{
		m_gray_buff.release();
	}
	if (!m_binary_buff.empty())
	{
		m_binary_buff.release();
	}
	if (!m_dilation_buff.empty())
	{
		m_dilation_buff.release();
	}
	if (!m_erosion_buff.empty())
	{
		m_erosion_buff.release();
	}
	if (!m_output_image_buff.empty())
	{
		m_output_image_buff.release();
	}
	if (!m_merge_Image.empty())
	{
		m_merge_Image.release();
	}
	if (!m_dilation_image.empty())
	{
		m_dilation_image.release();
	}
	if (!m_erosion_image.empty())
	{
		m_erosion_image.release();
	}
	if (!m_dila_merge_img.empty())
	{
		m_dila_merge_img.release();
	}
	if (!m_ero_merge_img.empty())
	{
		m_ero_merge_img.release();
	}
	if (!m_noise_merge_img.empty())
	{
		m_noise_merge_img.release();
	}

	
	return RTC::RTC_OK;
}


RTC::ReturnCode_t DilationErosion::onExecute(RTC::UniqueId ec_id)
{
  /* 新イメージのチェック */
  if(m_img_origIn.isNew()){

    m_img_origIn.read();


	m_image_buff.create(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC3);
	m_gray_buff.create(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC1);
	m_binary_buff.create(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC1);
	m_dilation_buff.create(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC1);
	m_erosion_buff.create(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC1);
	m_output_image_buff.create(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC3);
	m_merge_Image.create(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC3);
	m_dilation_image.create(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC1);
	m_erosion_image.create(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC1);
	m_dila_merge_img.create(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC3);
	m_ero_merge_img.create(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC3);
	m_noise_merge_img.create(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC3);



    /* InPortの映像データ */
    memcpy(m_image_buff.data,(void *)&(m_img_orig.pixels[0]), m_img_orig.pixels.length());

    /* BGRからグレースケールに変換する */
	cvtColor(m_image_buff, m_gray_buff, cv::COLOR_BGR2GRAY);

    /* グレースケールから2値に変換する */
    threshold( m_gray_buff, m_binary_buff, m_nThreshold, THRESHOLD_MAX_VALUE, cv::THRESH_BINARY );

    /* Dilation/Erosionを行ってノイズを消す */
	dilate(m_binary_buff, m_dilation_buff, cv::Mat(), cv::Point(-1, -1), 1);
	erode(m_dilation_buff, m_erosion_buff, cv::Mat(), cv::Point(-1, -1), 1);

    /* Dilationのみ行う */
	dilate(m_binary_buff, m_dilation_image, cv::Mat(), cv::Point(-1, -1), 1);

    /* Erosionのみ行う */
	erode(m_binary_buff, m_erosion_image, cv::Mat(), cv::Point(-1, -1), 1);

    /* 画像データのサイズ取得 */
	int len = (m_output_image_buff.channels() * m_output_image_buff.size().width * m_output_image_buff.size().height);
    m_img_out.pixels.length(len);
    m_img_dilation.pixels.length(len);
    m_img_erosion.pixels.length(len);

    /* DilationImageをマージする */
	std::vector<cv::Mat> tmp;
	tmp.push_back(m_dilation_image);
	tmp.push_back(m_dilation_image);
	tmp.push_back(m_dilation_image);
    cv::merge(tmp, m_dila_merge_img);

    /* ErosionImageをマージする */
	tmp.clear();
	tmp.push_back(m_erosion_image);
	tmp.push_back(m_erosion_image);
	tmp.push_back(m_erosion_image);
    cv::merge(tmp, m_ero_merge_img);

    /* ノイズを消したImageをマージする */
	tmp.clear();
	tmp.push_back(m_erosion_image);
	tmp.push_back(m_erosion_image);
	tmp.push_back(m_erosion_image);
	cv::merge(tmp, m_noise_merge_img);

    /* 該当のイメージをMemCopyする */
    memcpy((void *)&(m_img_out.pixels[0]), m_noise_merge_img.data, len);
	memcpy((void *)&(m_img_dilation.pixels[0]), m_dila_merge_img.data, len);
	memcpy((void *)&(m_img_erosion.pixels[0]), m_ero_merge_img.data, len);

    /* 反転した画像データをOutPortから出力する。 */
    m_img_out.width = m_image_buff.size().width;
	m_img_out.height = m_image_buff.size().height;

	m_img_dilation.width = m_image_buff.size().width;
	m_img_dilation.height = m_image_buff.size().height;

	m_img_erosion.width = m_image_buff.size().width;
	m_img_erosion.height = m_image_buff.size().height;

    m_img_outOut.write();
    m_img_dilationOut.write();
    m_img_erosionOut.write();



  }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t DilationErosion::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DilationErosion::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DilationErosion::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DilationErosion::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DilationErosion::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void DilationErosionInit(RTC::Manager* manager)
  {
    coil::Properties profile(dilationerosion_spec);
    manager->registerFactory(profile,
                             RTC::Create<DilationErosion>,
                             RTC::Delete<DilationErosion>);
  }
  
};


