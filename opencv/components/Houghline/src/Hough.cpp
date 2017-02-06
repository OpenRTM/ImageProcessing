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
    "version",           "1.1.0",
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
    "conf.__constraints__.hough_thresld", "1<=x<=255",
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




  
  
  debug_method = -1;
  debug_type = -1;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t Hough::onDeactivated(RTC::UniqueId ec_id)
{


  



  return RTC::RTC_OK;
}


RTC::ReturnCode_t Hough::onExecute(RTC::UniqueId ec_id)
{
  /* 新しいデータのチェック */
  if(m_image_origIn.isNew())
  {
    /* InPortデータの読み込み */
    m_image_origIn.read();


	/* InPortの画面データをコピー */
	cv::Mat imageBuff(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC3, (void *)&(m_image_orig.pixels[0]));
	cv::Mat grayImage;
	cv::Mat edgeImage;
	cv::Mat hough;
	cv::Mat houghImage;
    
    

    /* RGBからグレースケールに変換 */
    cv::cvtColor( imageBuff, grayImage, CV_RGB2GRAY );

    /* ハフ変換に必要なメモリ領域 */
	std::vector<cv::Vec4i> storage;

    /* エッジ抽出を行う */
    cv::Canny( grayImage, edgeImage, m_canny_threshold1, m_canny_threshold2, APERTURE_SIZE );

    /* グレースケールからRGBに変換する */
    cv::cvtColor( edgeImage, houghImage, CV_GRAY2RGB );

	std::vector<cv::Vec2f>  lines;
	std::vector<cv::Vec4i>  lines_P;
    /* ハフ変換により直線の抽出を行う */
    int hough_method;
    if ( m_hough_method == "PROBABILISTIC" )
    {
      /* 確率的ハフ変換 */
      //hough_method = CV_HOUGH_PROBABILISTIC;
		cv::HoughLinesP(edgeImage, lines_P, RHO, THETA, m_hough_threshold);
    }
    else if ( m_hough_method == "STANDARD" )
    {
      /* 標準的ハフ変換 */
      //hough_method = CV_HOUGH_STANDARD;
		cv::HoughLines(edgeImage, lines, RHO, THETA, m_hough_threshold);
    }
    else
    {
      /* マルチスケール型の古典的ハフ変換 */
      //hough_method = CV_HOUGH_MULTI_SCALE;
		cv::HoughLines(edgeImage, lines, RHO, THETA, m_hough_threshold, m_hough_param1, m_hough_param2);
		
    }
    /*if ( hough_method != debug_method )
    {
      std::cout << "hough_method = " << hough_method << std::endl;
      debug_method = hough_method;
    }*/

	
	//cv::HoughCircles(edgeImage, lines, hough_method, RHO, m_hough_threshold , m_hough_param1, m_hough_param2);
	
    /* 抽出された直線を描く */
    int line_type;
    if ( m_line_type == "CV_AA" )
    {
      line_type = CV_AA;
    }
    else
    {
      line_type = atoi( m_line_type.c_str() );
    }
    /*if ( line_type != debug_type )
    {
      std::cout << "line_type = " << line_type << std::endl;
      debug_type = line_type;
    }*/
	
	if (m_hough_method == "PROBABILISTIC")
	{
		for (unsigned int i = 0; i < lines_P.size(); i++) {
			//CvPoint *line = ( CvPoint* )cv::getSeqElem( lines, i );
			cv::line(houghImage, cv::Point(lines_P[i][0], lines_P[i][1]), cv::Point(lines_P[i][2], lines_P[i][3]),
				cv::Scalar(m_line_color_R, m_line_color_G, m_line_color_B), m_line_thickness,
				line_type, SHIFT);
		}
	}
	else
	{
		for (unsigned int i = 0; i < lines.size(); i++) {
			float rho = lines[i][0];
			float theta = lines[i][1];
			cv::Point pt1, pt2;
			double a = cos(theta);
			double b = sin(theta);
			double x0 = a*rho;
			double y0 = b*rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			//CvPoint *line = ( CvPoint* )cv::getSeqElem( lines, i );
			cv::line(houghImage, pt1, pt2, cv::Scalar(m_line_color_R, m_line_color_G, m_line_color_B), m_line_thickness,
				line_type, SHIFT);
		}
	}
	

    /* 画像データのサイズ取得 */
	int len = houghImage.channels() * houghImage.size().width * houghImage.size().height;
    m_image_hough.pixels.length(len);
    m_image_hough.width  = houghImage.size().width;
	m_image_hough.height = houghImage.size().height;

    /* 反転した画像データをOutPortにコピー */
    memcpy( (void *)&(m_image_hough.pixels[0]), houghImage.data, len );

    /* 反転した画像データをOutPortから出力 */
    m_image_houghOut.write();


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


