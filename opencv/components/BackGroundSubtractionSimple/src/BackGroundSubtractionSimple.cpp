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
    "version",           "1.1.0",
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
	"RGB",          //RGBの成分ごとに評価
	"CIE L*a*b*",   //CIE L*a*b* で距離を評価
	"gray scale"    //グレースケールで評価
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

	m_originalImage = NULL;
	m_currentImage = NULL;
	m_backgroundImage = NULL;
	m_resultImage = NULL;
	m_outputImage = NULL;
  
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
  if(m_originalImage != NULL){
    cvReleaseImage(&m_originalImage);
  }
  if(m_currentImage != NULL){
    cvReleaseImage(&m_currentImage);
  }
  if(m_resultImage != NULL){
    cvReleaseImage(&m_resultImage);
  }
  if(m_outputImage != NULL){
    cvReleaseImage(&m_outputImage);
  }
  if(m_backgroundImage != NULL){
    cvReleaseImage(&m_backgroundImage);
  }

  m_originalImage = NULL;
  m_currentImage = NULL;
  m_backgroundImage = NULL;
  m_resultImage = NULL;
  m_outputImage = NULL;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t BackGroundSubtractionSimple::onExecute(RTC::UniqueId ec_id)
{

  if(m_img_origIn.isNew()) {

  /* イメージRead */
  m_img_origIn.read();

  if(m_originalImage == NULL){
    m_originalImage = cvCreateImage(cvSize(m_img_orig.width, m_img_orig.height), IPL_DEPTH_8U, 3);
  }
  if(m_currentImage == NULL){
    m_currentImage = cvCreateImage(cvSize(m_img_orig.width, m_img_orig.height), IPL_DEPTH_8U, 3);
  }

  if(m_img_orig.width != m_temp_w || m_img_orig.height != m_temp_h){

    if(m_backgroundImage != NULL){
      cvReleaseImage(&m_backgroundImage);
    }
    m_backgroundImage = cvCreateImage(cvSize(m_img_orig.width, m_img_orig.height), IPL_DEPTH_8U, 3);
  }

  if(m_resultImage == NULL){
    m_resultImage =  cvCreateImage(cvSize(m_img_orig.width, m_img_orig.height), IPL_DEPTH_8U, 1);
  }
  if(m_outputImage == NULL){
    m_outputImage = cvCreateImage(cvSize(m_img_orig.width, m_img_orig.height), IPL_DEPTH_8U, 3);
  }

  /* InPortの映像の取得 */
  memcpy(m_originalImage->imageData,(void *)&(m_img_orig.pixels[0]),m_img_orig.pixels.length());
  m_currentImage = cvCloneImage( m_originalImage );
		
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
    cvErode( m_resultImage, m_resultImage );
    cvDilate( m_resultImage, m_resultImage );
  }else if ( m_noiseMode == NOISE_MEDIAN ){
    cvSmooth( m_resultImage, m_resultImage, CV_MEDIAN );
  }

  cvMerge( m_resultImage, m_resultImage, m_resultImage, NULL, m_outputImage );

  /* 画像データのサイズ取得 */
  double len1 = (m_currentImage->nChannels * m_currentImage->width * m_currentImage->height);
  double len2 = (m_outputImage->nChannels * m_outputImage->width * m_outputImage->height);
  double len3 = (m_backgroundImage->nChannels * m_backgroundImage->width * m_backgroundImage->height);

  m_img_curr.pixels.length(len1);
  m_img_resu.pixels.length(len2);
  m_img_back.pixels.length(len3);

  /* 該当のイメージをMemCopyする */
  memcpy((void *)&(m_img_curr.pixels[0]), m_currentImage->imageData, len1);
  memcpy((void *)&(m_img_resu.pixels[0]), m_outputImage->imageData, len2);
  memcpy((void *)&(m_img_back.pixels[0]), m_backgroundImage->imageData, len3);

  m_img_curr.width = m_originalImage->width;
  m_img_curr.height = m_originalImage->height;

  m_img_resu.width = m_originalImage->width;
  m_img_resu.height = m_originalImage->height;

  m_img_back.width = m_originalImage->width;
  m_img_back.height = m_originalImage->height;

  m_img_currOut.write();
  m_img_resuOut.write();
  m_img_backOut.write();
			
  /* Key入力Read */
  if(m_keyIn.isNew()){
    m_keyIn.read();

    if(m_cont_mode == 'b')
    {
      /* 背景画像更新 */
      if(m_backgroundImage != NULL) {
        cvReleaseImage(&m_backgroundImage);
      }
      m_backgroundImage = cvCloneImage(m_originalImage);
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
		
  if(m_originalImage != NULL){
    cvReleaseImage(&m_originalImage);
  }
  if(m_currentImage != NULL){
    cvReleaseImage(&m_currentImage);
  }
  if(m_resultImage != NULL){
    cvReleaseImage(&m_resultImage);
  }
  if(m_outputImage != NULL){
    cvReleaseImage(&m_outputImage);
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
  IplImage *differenceImage = cvCreateImage(cvSize(m_currentImage->width, m_currentImage->height), IPL_DEPTH_8U, 3);	//	差分画像用IplImage
  IplImage *differenceRImage = cvCreateImage(cvSize(m_currentImage->width, m_currentImage->height), IPL_DEPTH_8U, 1);	//	R値の差分用IplImage
  IplImage *differenceGImage = cvCreateImage(cvSize(m_currentImage->width, m_currentImage->height), IPL_DEPTH_8U, 1);	//	G値の差分用IplImage
  IplImage *differenceBImage = cvCreateImage(cvSize(m_currentImage->width, m_currentImage->height), IPL_DEPTH_8U, 1);	//	B値の差分用IplImage

  /* 現在の背景との差の絶対値を成分ごとに取る */
  cvAbsDiff( m_currentImage, m_backgroundImage, differenceImage );

  /* 閾値処理を行う */
  cvThreshold( differenceImage, differenceImage, m_nThresholdLv, THRESHOLD_MAX_VALUE, CV_THRESH_BINARY );

  /* 成分ごとの画像に分割する */
  cvSplit( differenceImage, differenceBImage, differenceGImage, differenceRImage, NULL );

  /* ORで合成する */
  cvOr( differenceRImage, differenceGImage, m_resultImage );
  cvOr( differenceBImage, m_resultImage, m_resultImage );

  /* メモリを解放する */
  cvReleaseImage( &differenceImage );
  cvReleaseImage( &differenceRImage );
  cvReleaseImage( &differenceGImage );
  cvReleaseImage( &differenceBImage );

}

//
//	L*a*b*で距離を評価する
//
void BackGroundSubtractionSimple::labDifference( void )
{
  /* 画像を生成する */
  IplImage *currentLabImage = cvCreateImage( cvSize(m_currentImage->width, m_currentImage->height),IPL_DEPTH_32F, 3 );		/* 現在の画像をL*a*b*に変換した画像用IplImage */
  IplImage *backgroundLabImage = cvCreateImage( cvSize(m_currentImage->width, m_currentImage->height), IPL_DEPTH_32F, 3 );	/* 背景をL*a*b*に変換した画像用IplImage */
  IplImage *differenceLabImage = cvCreateImage( cvSize(m_currentImage->width, m_currentImage->height), IPL_DEPTH_32F, 3 );	/* 差分画像用IplImage */
  IplImage *differenceLImage = cvCreateImage( cvSize(m_currentImage->width, m_currentImage->height), IPL_DEPTH_32F, 1 );		/* L*値の差分用IplImage */
  IplImage *differenceAImage = cvCreateImage( cvSize(m_currentImage->width, m_currentImage->height), IPL_DEPTH_32F, 1 );		/* a*値の差分用IplImage */
  IplImage *differenceBImage = cvCreateImage( cvSize(m_currentImage->width, m_currentImage->height), IPL_DEPTH_32F, 1 );		/* b*値の差分用IplImage */
  IplImage *sqrDifferenceImage = cvCreateImage( cvSize(m_currentImage->width, m_currentImage->height), IPL_DEPTH_32F, 1 );	/* 距離算出用IplImage */

  /* 現在の画像と背景を共に CIE L*a*b* に変換 */
  cvConvertScale( m_currentImage, currentLabImage, SCALE );
  cvConvertScale( m_backgroundImage, backgroundLabImage, SCALE );
  cvCvtColor( currentLabImage, currentLabImage, CV_BGR2Lab );
  cvCvtColor( backgroundLabImage, backgroundLabImage, CV_BGR2Lab );

  /* 距離の二乗を計算する */
  cvSub( currentLabImage, backgroundLabImage, differenceLabImage );
  cvPow( differenceLabImage, differenceLabImage, 2 );

  /* 成分ごとの画像に分割する */
  cvSplit( differenceLabImage, differenceLImage, differenceAImage, differenceBImage, NULL );

  cvCopy( differenceLImage, sqrDifferenceImage );
  cvAdd( differenceAImage, sqrDifferenceImage, sqrDifferenceImage );
  cvAdd( differenceBImage, sqrDifferenceImage, sqrDifferenceImage );

  /* 閾値処理を行う */
  cvThreshold( sqrDifferenceImage, m_resultImage, m_nThresholdLv * m_nThresholdLv, THRESHOLD_MAX_VALUE, CV_THRESH_BINARY );

  /* メモリを解放する */
  cvReleaseImage( &currentLabImage );
  cvReleaseImage( &backgroundLabImage );
  cvReleaseImage( &differenceLabImage );
  cvReleaseImage( &differenceLImage );
  cvReleaseImage( &differenceAImage );
  cvReleaseImage( &differenceBImage );
  cvReleaseImage( &sqrDifferenceImage );
}

//
//	グレースケールで評価する
//
void BackGroundSubtractionSimple::grayScaleDifference( void )
{
  /* 画像を生成する */
  IplImage *differenceImage = cvCreateImage( cvSize(m_currentImage->width, m_currentImage->height), IPL_DEPTH_8U, 3 );	/* 差分画像用IplImage */

  /* 現在の背景との差の絶対値を成分ごとに取る */
  cvAbsDiff( m_currentImage, m_backgroundImage, differenceImage );

  /* BGRからグレースケールに変換する */
  cvCvtColor( differenceImage, m_resultImage, CV_BGR2GRAY );

  /* グレースケールから2値に変換する */
  cvThreshold( m_resultImage, m_resultImage, m_nThresholdLv, THRESHOLD_MAX_VALUE, CV_THRESH_BINARY );

  /* メモリを解放する */
  cvReleaseImage( &differenceImage );
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


