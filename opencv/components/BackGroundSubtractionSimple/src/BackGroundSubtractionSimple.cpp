// -*- C++ -*-
/*!
 * @file  BackGroundSubtractionSimple.cpp
 * @brief BackGroundSubtractionSimple component
 * @date $Date$
 *
 * $Id$
 */

#include "BackGroundSubtractionSimple.h"

// Module specification
// <rtc-template block="module_spec">
static const char* backgroundsubtractionsimple_spec[] =
  {
    "implementation_id", "BackGroundSubtractionSimple",
    "type_name",         "BackGroundSubtractionSimple",
    "description",       "BackGroundSubtractionSimple component",
    "version",           "1.2.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.control_mode", "b",
    "conf.default.diff_mode", "0",
    "conf.default.noise_mode", "0",
    "conf.default.threshold_level", "20",
    // Widget
    "conf.__widget__.control_mode", "radio",
    "conf.__widget__.diff_mode", "radio",
    "conf.__widget__.noise_mode", "radio",
    "conf.__widget__.threshold_level", "slider.1",
    // Constraints
    "conf.__constraints__.control_mode", "(b,M)",
    "conf.__constraints__.diff_mode", "(0,1,2)",
    "conf.__constraints__.noise_mode", "(0,1,2)",
    "conf.__constraints__.threshold_level", "0<=x<=255",
    ""
  };
// </rtc-template>

std::string differenceMethod[3] = {
	"RGB",
	"CIE L*a*b*",
	"gray scale"
};

std::string noiseMethod[3] = {
	"None",
	"Opening",
	"Median filter"
};

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
BackGroundSubtractionSimple::BackGroundSubtractionSimple(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_img_origIn("original_image", m_img_orig),
    m_keyIn("key", m_key),
    m_img_currOut("current_image", m_img_curr),
    m_img_resuOut("result_image", m_img_resu),
    m_img_backOut("background_image", m_img_back)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
BackGroundSubtractionSimple::~BackGroundSubtractionSimple()
{
}



RTC::ReturnCode_t BackGroundSubtractionSimple::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("original_image", m_img_origIn);
  addInPort("key", m_keyIn);
  
  // Set OutPort buffer
  addOutPort("current_image", m_img_currOut);
  addOutPort("result_image", m_img_resuOut);
  addOutPort("background_image", m_img_backOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("control_mode", m_cont_mode, "b");
  bindParameter("diff_mode", m_diff_mode, "0");
  bindParameter("noise_mode", m_noise_mode, "0");
  bindParameter("threshold_level", m_nThresholdLv, "20");
  // </rtc-template>


  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t BackGroundSubtractionSimple::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackGroundSubtractionSimple::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackGroundSubtractionSimple::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t BackGroundSubtractionSimple::onActivated(RTC::UniqueId ec_id)
{	
  m_differenceMode = COLOR_DIFFERENCE;	/* 差分の計算モード */
  m_noiseMode = NOISE_KEEP;             /* ノイズを除去するモード */

  m_temp_w = 0;
  m_temp_h = 0;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t BackGroundSubtractionSimple::onDeactivated(RTC::UniqueId ec_id)
{



  if (!m_backgroundImage.empty())
  {
	  m_backgroundImage.release();
  }



  return RTC::RTC_OK;
}


RTC::ReturnCode_t BackGroundSubtractionSimple::onExecute(RTC::UniqueId ec_id)
{

  if(m_img_origIn.isNew()) {

  /* イメージRead */
  m_img_origIn.read();




  if(m_img_orig.width != m_temp_w || m_img_orig.height != m_temp_h){
	if (m_backgroundImage.empty())
	{
		m_backgroundImage.create(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC3);
	}
  }



  

  /* InPortの映像の取得 */
  //memcpy(m_originalImage.data,(void *)&(m_img_orig.pixels[0]),m_img_orig.pixels.length());
  cv::Mat m_originalImage(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC3, (void *)&(m_img_orig.pixels[0]));
  
  m_currentImage = m_originalImage.clone();
  
  cv::Mat m_outputImage;
		
  /* 差の計算方法の切り替え */
  if( m_differenceMode == COLOR_DIFFERENCE ){	
    /* 成分ごとに評価をする */
    colorDifference();
  } else if( m_differenceMode == LAB_DIFFERENCE ){	
    /*	L*a*b*で距離を評価する */
    labDifference();
  } else if( m_differenceMode == GRAY_DIFFERENCE ){
    /* グレースケールで評価をする */
    grayScaleDifference();
  }

  /* ノイズ除去 */
  if( m_noiseMode == NOISE_MORPHOLOGY ){
	  //cv::Mat tmp();
	  erode(m_currentImage, m_backgroundImage, cv::Mat(), cv::Point(-1, -1), 1);
	  dilate(m_backgroundImage, m_resultImage, cv::Mat(), cv::Point(-1, -1), 1);
  }else if ( m_noiseMode == NOISE_MEDIAN ){
	  GaussianBlur(m_currentImage, m_resultImage, m_currentImage.size(), 0,0);
  }

  std::vector<cv::Mat> tmp;
  tmp.push_back(m_resultImage);
  tmp.push_back(m_resultImage);
  tmp.push_back(m_resultImage);
  merge(tmp, m_outputImage);

  /* 画像データのサイズ取得 */
  int len1 = (m_currentImage.channels() * m_currentImage.size().width * m_currentImage.size().height);
  int len2 = (m_outputImage.channels() * m_outputImage.size().width * m_outputImage.size().height);
  int len3 = (m_backgroundImage.channels() * m_backgroundImage.size().width * m_backgroundImage.size().height);

  m_img_curr.pixels.length(len1);
  m_img_resu.pixels.length(len2);
  m_img_back.pixels.length(len3);

  /* 該当のイメージをMemCopyする */
  memcpy((void *)&(m_img_curr.pixels[0]), m_currentImage.data, len1);
  memcpy((void *)&(m_img_resu.pixels[0]), m_outputImage.data, len2);
  memcpy((void *)&(m_img_back.pixels[0]), m_backgroundImage.data, len3);

  m_img_curr.width = m_originalImage.size().width;
  m_img_curr.height = m_originalImage.size().height;

  m_img_resu.width = m_originalImage.size().width;
  m_img_resu.height = m_originalImage.size().height;

  m_img_back.width = m_originalImage.size().width;
  m_img_back.height = m_originalImage.size().height;

  m_img_currOut.write();
  m_img_resuOut.write();
  m_img_backOut.write();
			
  /* Key入力Read */
  if(m_keyIn.isNew()){
    m_keyIn.read();

    if(m_cont_mode == 'b')
    {
      /* 背景画像更新 */

		m_backgroundImage = m_originalImage.clone();
      printf( "Background image update( %s : %s )\n", 
              differenceMethod[m_differenceMode].c_str(), noiseMethod[m_noiseMode].c_str() );   

    }else if(m_cont_mode == 'M')
    {
      /* 差の評価方法・ノイズ除去方法変更 */
      m_differenceMode = (int)(m_diff_mode - '0');
      m_noiseMode = (int)(m_noise_mode - '0');
      printf( "Change method: (diff : noise) = (%s : %s)\n", 
            differenceMethod[m_differenceMode].c_str(), noiseMethod[m_noiseMode].c_str() );
    }
  }
		
  
  //if(backgroundImage != NULL){
  //	cvReleaseImage(&backgroundImage);
  //}

  m_temp_w = m_img_orig.width;
  m_temp_h = m_img_orig.height;
			
	}

	return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t BackGroundSubtractionSimple::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackGroundSubtractionSimple::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackGroundSubtractionSimple::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackGroundSubtractionSimple::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackGroundSubtractionSimple::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

//
// 成分ごとに評価する
//
void BackGroundSubtractionSimple::colorDifference( void )
{
	
	/* 画像を生成する */
	cv::Mat differenceImage;
	cv::Mat differenceRImage;
	cv::Mat differenceGImage;
	cv::Mat differenceBImage;

	differenceImage.create(m_currentImage.size(), CV_8UC3);
	differenceRImage.create(m_currentImage.size(), CV_8UC1);
	differenceGImage.create(m_currentImage.size(), CV_8UC1);
	differenceBImage.create(m_currentImage.size(), CV_8UC1);

  /* 現在の背景との差の絶対値を成分ごとに取る */
  absdiff( m_currentImage, m_backgroundImage, differenceImage );

  /* 閾値処理を行う */
  threshold( differenceImage, differenceImage, m_nThresholdLv, THRESHOLD_MAX_VALUE, cv::THRESH_BINARY );

  /* 成分ごとの画像に分割する */
  std::vector<cv::Mat> tmp;
  tmp.push_back(differenceBImage);
  tmp.push_back(differenceGImage);
  tmp.push_back(differenceRImage);
  split(differenceImage, tmp);

  /* ORで合成する */
  cv::bitwise_or(differenceRImage, differenceGImage, m_resultImage);
  cv::bitwise_or(differenceBImage, m_resultImage, m_resultImage);



}

//
//	L*a*b*で距離を評価する
//
void BackGroundSubtractionSimple::labDifference( void )
{
  /* 画像を生成する */
	cv::Mat currentLabImage;
	cv::Mat backgroundLabImage;
	cv::Mat differenceLabImage;
	cv::Mat differenceLImage;
	cv::Mat differenceAImage;
	cv::Mat differenceBImage;
	cv::Mat sqrDifferenceImage;


	currentLabImage.create(m_currentImage.size(), CV_8UC3);   /* 現在の画像をL*a*b*に変換した画像用IplImage */
	backgroundLabImage.create(m_currentImage.size(), CV_8UC3);/* 背景をL*a*b*に変換した画像用IplImage */
	differenceLabImage.create(m_currentImage.size(), CV_8UC3);/* 差分画像用IplImage */
	differenceLImage.create(m_currentImage.size(), CV_8UC1);  /* L*値の差分用IplImage */
	differenceAImage.create(m_currentImage.size(), CV_8UC1);  /* a*値の差分用IplImage */
	differenceBImage.create(m_currentImage.size(), CV_8UC1);  /* b*値の差分用IplImage */
	sqrDifferenceImage.create(m_currentImage.size(), CV_8UC1);/* 距離算出用IplImage */

  /* 現在の画像と背景を共に CIE L*a*b* に変換 */
	currentLabImage.convertTo(m_currentImage, CV_32F, SCALE);
	backgroundLabImage.convertTo(m_backgroundImage, CV_32F, SCALE);

	cv::cvtColor(currentLabImage, currentLabImage, COLOR_BGR2Lab);
	cv::cvtColor(backgroundLabImage, backgroundLabImage, COLOR_BGR2Lab);

  /* 距離の二乗を計算する */
	subtract(currentLabImage, backgroundLabImage, differenceLabImage);
	cv::pow(differenceLabImage,2, differenceLabImage);

  /* 成分ごとの画像に分割する */
	std::vector<cv::Mat> tmp;
	tmp.push_back(differenceLImage);
	tmp.push_back(differenceAImage);
	tmp.push_back(differenceBImage);
	cv::split(differenceLabImage, tmp);

	sqrDifferenceImage = differenceLImage;
	//sqrDifferenceImage.copyTo(differenceLImage);
  cv::add( differenceAImage, sqrDifferenceImage, sqrDifferenceImage );
  cv::add(differenceBImage, sqrDifferenceImage, sqrDifferenceImage);

  /* 閾値処理を行う */
  threshold(sqrDifferenceImage, m_resultImage, m_nThresholdLv, THRESHOLD_MAX_VALUE, cv::THRESH_BINARY);

}

//
//	グレースケールで評価する
//
void BackGroundSubtractionSimple::grayScaleDifference( void )
{
  /* 画像を生成する */
  cv::Mat differenceImage;	/* 差分画像用IplImage */


  differenceImage.create(m_currentImage.size(), CV_8UC3);


  /* 現在の背景との差の絶対値を成分ごとに取る */
  absdiff( m_currentImage, m_backgroundImage, differenceImage );

  /* BGRからグレースケールに変換する */
  cvtColor(differenceImage, m_resultImage, cv::COLOR_BGR2GRAY);

  /* グレースケールから2値に変換する */
  threshold( m_resultImage, m_resultImage, m_nThresholdLv, THRESHOLD_MAX_VALUE, cv::THRESH_BINARY );


}

extern "C"
{
 
  void BackGroundSubtractionSimpleInit(RTC::Manager* manager)
  {
    coil::Properties profile(backgroundsubtractionsimple_spec);
    manager->registerFactory(profile,
                             RTC::Create<BackGroundSubtractionSimple>,
                             RTC::Delete<BackGroundSubtractionSimple>);
  }
  
};


