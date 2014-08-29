// -*- C++ -*-
/*!
 * @file  ImageSubstraction.cpp
 * @brief Image substraction conponent
 * @date $Date$
 *
 * $Id$
 */

#include "ImageSubstraction.h"

// Module specification
// <rtc-template block="module_spec">
static const char* imagesubstraction_spec[] =
  {
    "implementation_id", "ImageSubstraction",
    "type_name",         "ImageSubstraction",
    "description",       "Image substraction conponent",
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
    "conf.default.image_height", "240",
    "conf.default.image_width", "320",
    "conf.default.threshold_coefficient", "5.0",
    "conf.default.constant_threshold", "20",
    // Widget
    "conf.__widget__.control_mode", "radio",
    "conf.__widget__.image_height", "text",
    "conf.__widget__.image_width", "text",
    "conf.__widget__.threshold_coefficient", "text",
    "conf.__widget__.constant_threshold", "slider.1",
    // Constraints
    "conf.__constraints__.control_mode", "(b,m)",
    "conf.__constraints__.constant_threshold", "0<=x<=255",
    ""
  };
// </rtc-template>

int ImageSubstraction_count = 0;
int	mode = DYNAMIC_MODE;
int g_temp_w = 0;
int g_temp_h = 0;

std::string mode_str[2] = {
	"DYNAMIC_MODE",   /* 画素ごとに異なる閾値 */
	"CONSTANT_MODE"   /* 画像全体で一つの閾値 */
};

IplImage *backgroundAverageImage = NULL;	/* 背景の平均値保存用IplImage */
IplImage *backgroundThresholdImage = NULL;	/* 背景の閾値保存用IplImage */

IplImage *originalImage;		/* キャプチャ画像用IplImage */
IplImage *differenceImage;	/* 差分画像用IplImage */
IplImage *resultImage;	
IplImage *outputImage;

//
//	背景モデルを初期化する
//
//	引数:
//		num  : 背景モデルを生成するのに使用する画像の枚数
//		size : 画像サイズ
//
void initializeBackgroundModel( int num, CvSize size, double thre_coefficient ){
  int i;

  /* 以前の背景情報があれば破棄 */
  if( backgroundAverageImage != NULL ){
    cvReleaseImage( &backgroundAverageImage );
  }
  if( backgroundThresholdImage != NULL ){
    cvReleaseImage( &backgroundThresholdImage );
  }

  /* 画像情報蓄積用バッファを確保する */
  IplImage *acc = cvCreateImage( size, IPL_DEPTH_32F, 3 );
  IplImage *acc2 = cvCreateImage( size, IPL_DEPTH_32F, 3 );

  /* 画像の初期化を行う */
  cvSetZero( acc );
  cvSetZero( acc2 );

  /* 画像情報の蓄積 */
  printf( "Getting background...\n" );
  //IplImage *frameImage;
  for( i = 0; i < num; i++ ){
    //frameImage = cvQueryFrame( capture );
    cvAcc( originalImage, acc );
    cvSquareAcc( originalImage, acc2 );
    printf( "%d / %d image\n", i + 1, num );
  }
  printf( "Completion!\n" );

  /* cvAddS, cvSubS はあるが cvMulS はないので、cvConvertScale を使う */
  cvConvertScale( acc, acc, 1.0 / num );		/* 平均 */
  cvConvertScale( acc2, acc2, 1.0 / num );	/* 二乗和の平均 */

  /* 平均が求まったので backgroundAverageImage に格納する */
  backgroundAverageImage = cvCreateImage( size, IPL_DEPTH_8U, 3 );
  cvConvert( acc, backgroundAverageImage );

  /* 分散を計算する */
  IplImage *dispersion = cvCreateImage( size, IPL_DEPTH_32F, 3 );
  cvMul( acc, acc, acc );
  cvSub( acc2, acc, dispersion );

  /* 標準偏差を計算する */
  IplImage *sd = cvCreateImage( size, IPL_DEPTH_32F, 3 );
  cvPow( dispersion, sd, 0.5 );

  /* 閾値を計算する */
  backgroundThresholdImage = cvCreateImage( size, IPL_DEPTH_8U, 3 );
  cvConvertScale( sd, backgroundThresholdImage, thre_coefficient );

  cvReleaseImage( &acc );
  cvReleaseImage( &acc2 );
  cvReleaseImage( &dispersion );
  cvReleaseImage( &sd );
}

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
ImageSubstraction::ImageSubstraction(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_img_origIn("original_image", m_img_orig),
    m_KeyIn("Key", m_Key),
    m_img_captureOut("capture_image", m_img_capture),
    m_img_resultOut("result_image", m_img_result),
    m_img_backOut("back_image", m_img_back),
    m_img_thresholdOut("threshold_image", m_img_threshold)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
ImageSubstraction::~ImageSubstraction()
{
}



RTC::ReturnCode_t ImageSubstraction::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("original_image", m_img_origIn);
  addInPort("Key", m_KeyIn);
  
  // Set OutPort buffer
  addOutPort("capture_image", m_img_captureOut);
  addOutPort("result_image", m_img_resultOut);
  addOutPort("back_image", m_img_backOut);
  addOutPort("threshold_image", m_img_thresholdOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("control_mode", m_cont_mode, "b");
  bindParameter("image_height", m_img_height, "240");
  bindParameter("image_width", m_img_width, "320");
  bindParameter("threshold_coefficient", m_thre_coefficient, "5.0");
  bindParameter("constant_threshold", m_constant_thre, "20");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ImageSubstraction::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageSubstraction::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageSubstraction::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t ImageSubstraction::onActivated(RTC::UniqueId ec_id)
{
  ImageSubstraction_count = 0;
  g_temp_w = 0;
  g_temp_h = 0;

  originalImage = NULL;
  outputImage = NULL;
  resultImage = NULL;
  differenceImage = NULL;

  /* 閾値の初期設定を表示 */
  printf( "threshold: %s\n", mode_str[1-mode].c_str() );

  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageSubstraction::onDeactivated(RTC::UniqueId ec_id)
{
  if(differenceImage != NULL){
    cvReleaseImage(&differenceImage);
  }
  if(originalImage != NULL){
    cvReleaseImage(&originalImage);
  }
  if(resultImage != NULL){
    cvReleaseImage(&resultImage);
  }
  if(outputImage != NULL){
    cvReleaseImage(&outputImage);
  }

  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageSubstraction::onExecute(RTC::UniqueId ec_id)
{	
  /* キー入力判定 */
  if(m_KeyIn.isNew())
  {
    m_KeyIn.read();

    if( m_cont_mode == 'b' )
    {
      /* 'b'キーが押されたらその時点での画像を背景画像とする */
      initializeBackgroundModel( NUM_OF_BACKGROUND_FRAMES, cvSize(m_img_width, m_img_height), m_thre_coefficient);

      printf( "Background image update\n" );   /* 背景情報更新 */

    } else if( m_cont_mode == 'm' ){
      /* 'm'キーが押されたら閾値の設定方法を変更する */
      mode = 1 - mode;
      printf( "threshold: %s\n", mode_str[mode].c_str() );
    }
  }
				
  /* 初期値を取得する */
  if(ImageSubstraction_count == 0 && m_img_origIn.isNew()) {

    m_img_origIn.read();

    if(g_temp_w != m_img_orig.width || g_temp_h != m_img_orig.height){

      if(originalImage != NULL){
        cvReleaseImage(&originalImage);	
      }
      if(originalImage == NULL){
        originalImage = cvCreateImage(cvSize(m_img_orig.width, m_img_orig.height), IPL_DEPTH_8U, 3);  /* キャプチャ画像用IplImage */
      }
      if(outputImage != NULL){
        cvReleaseImage(&outputImage);
      }
      if(outputImage == NULL){
        outputImage = cvCreateImage(cvSize(m_img_orig.width, m_img_orig.height), IPL_DEPTH_8U, 3);
      }

      memcpy(originalImage->imageData,(void *)&(m_img_orig.pixels[0]), m_img_orig.pixels.length());

      if(differenceImage != NULL){
        cvReleaseImage(&differenceImage);
      }
      if(differenceImage == NULL){
        differenceImage = cvCloneImage(originalImage);
      }

      if(resultImage != NULL){
        cvReleaseImage(&resultImage);
      }
      if(resultImage == NULL){
        resultImage = cvCreateImage(cvSize(m_img_orig.width, m_img_orig.height),IPL_DEPTH_8U, 1);
      }

      initializeBackgroundModel( NUM_OF_BACKGROUND_FRAMES, cvSize(m_img_orig.width, m_img_orig.height) , m_thre_coefficient);

      ImageSubstraction_count = 1;
      g_temp_w = m_img_orig.width;
      g_temp_h = m_img_orig.height;
    }
  }

  if(ImageSubstraction_count == 1 && m_img_origIn.isNew()) {

    m_img_origIn.read();

    if(g_temp_w == m_img_orig.width && g_temp_h == m_img_orig.height){

      if(originalImage != NULL){
        cvReleaseImage(&originalImage);	
      }

      if(originalImage == NULL){
        originalImage = cvCreateImage(cvSize(m_img_orig.width, m_img_orig.height), IPL_DEPTH_8U, 3);  /* キャプチャ画像用IplImage */
      }

      if(outputImage != NULL){
        cvReleaseImage(&outputImage);
      }

      if(outputImage == NULL){
        outputImage = cvCreateImage(cvSize(m_img_orig.width, m_img_orig.height), IPL_DEPTH_8U, 3);
      }

      memcpy(originalImage->imageData,(void *)&(m_img_orig.pixels[0]), m_img_orig.pixels.length());

      /* 現在の背景との差の絶対値を成分ごとに取る */
      cvAbsDiff( originalImage, backgroundAverageImage, differenceImage );

      /* Sub はマイナスになったら0に切り詰めてくれる */
      if( mode == DYNAMIC_MODE ){
        cvSub( differenceImage, backgroundThresholdImage, differenceImage );
      } else{
        cvSubS( differenceImage, cvScalarAll( m_constant_thre ), differenceImage );
      }

      /* differenceImage の要素が1つでも0以上だったら前景 */
      cvCvtColor( differenceImage, resultImage, CV_BGR2GRAY );
      cvThreshold( resultImage, resultImage, 0, 255, CV_THRESH_BINARY );

      /* メディアンフィルタでノイズを除去する */
      cvSmooth( resultImage, resultImage, CV_MEDIAN );

      IplImage *tmp = cvCloneImage( differenceImage );
      cvConvertScale( tmp, tmp, 3 );
      //showFlipImage( windowNameThreshold, tmp );

      cvMerge( resultImage, resultImage, resultImage, NULL, outputImage );

      /* 画像データのサイズ取得 */
      double len1 = (originalImage->nChannels * originalImage->width * originalImage->height);
      double len2 = (outputImage->nChannels * outputImage->width * outputImage->height);
      double len3 = (backgroundAverageImage->nChannels * backgroundAverageImage->width * backgroundAverageImage->height);
      double len4 = (tmp->nChannels * tmp->width * tmp->height);

      m_img_capture.pixels.length(len1);
      m_img_result.pixels.length(len2);
      m_img_back.pixels.length(len3);
      m_img_threshold.pixels.length(len4);

      /* 該当のイメージをMemCopyする */
      memcpy((void *)&(m_img_capture.pixels[0]), originalImage->imageData, len1);
      memcpy((void *)&(m_img_result.pixels[0]), outputImage->imageData, len2);
      memcpy((void *)&(m_img_back.pixels[0]), backgroundAverageImage->imageData, len3);
      memcpy((void *)&(m_img_threshold.pixels[0]), tmp->imageData, len4);

      m_img_capture.width = originalImage->width;
      m_img_capture.height = originalImage->height;

      m_img_result.width = originalImage->width;
      m_img_result.height = originalImage->height;

      m_img_back.width = originalImage->width;
      m_img_back.height = originalImage->height;

      m_img_threshold.width = originalImage->width;
      m_img_threshold.height = originalImage->height;

      m_img_captureOut.write();
      m_img_resultOut.write();
      m_img_backOut.write();
      m_img_thresholdOut.write();

      cvReleaseImage( &tmp );

      cvReleaseImage(&originalImage);
      cvReleaseImage(&outputImage);

      g_temp_w = m_img_orig.width;
      g_temp_h = m_img_orig.height;

    }else if(g_temp_w != m_img_orig.width || g_temp_h != m_img_orig.height){
      ImageSubstraction_count = 0;
    }
  }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ImageSubstraction::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageSubstraction::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageSubstraction::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageSubstraction::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageSubstraction::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void ImageSubstractionInit(RTC::Manager* manager)
  {
    coil::Properties profile(imagesubstraction_spec);
    manager->registerFactory(profile,
                             RTC::Create<ImageSubstraction>,
                             RTC::Delete<ImageSubstraction>);
  }
  
};


