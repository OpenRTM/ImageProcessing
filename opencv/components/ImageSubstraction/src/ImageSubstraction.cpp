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


const char* ImageSubstraction::mode_str[2] = {
	"DYNAMIC_MODE",   /* 画素ごとに異なる閾値 */
	"CONSTANT_MODE"   /* 画像全体で一つの閾値 */
};


//
//	背景モデルを初期化する
//
//	引数:
//		num  : 背景モデルを生成するのに使用する画像の枚数
//		size : 画像サイズ
//
void ImageSubstraction::initializeBackgroundModel(int num, cv::Size size, double thre_coefficient){
  int i;



  /* 画像情報蓄積用バッファを確保する */
  cv::Mat  acc = cv::Mat::zeros(size, CV_32FC3);
  cv::Mat  acc2 = cv::Mat::zeros(size, CV_32FC3);
  





  /* 画像情報の蓄積 */
  printf( "Getting background...\n" );
  //IplImage *frameImage;
  for( i = 0; i < num; i++ ){
    //frameImage = cvQueryFrame( capture );

	  cv::accumulate(originalImage, acc, cv::noArray());

	  cv::accumulateSquare(originalImage, acc2, cv::noArray());

    printf( "%d / %d image\n", i + 1, num );
  }
  printf( "Completion!\n" );

  
  /* cvAddS, cvSubS はあるが cvMulS はないので、cvConvertScale を使う */
  acc.convertTo(acc, CV_32F, 1.0 / num);		/* 平均 */
  acc2.convertTo(acc2, CV_32F, 1.0 / num);	/* 二乗和の平均 */
  
  

  /* 平均が求まったので backgroundAverageImage に格納する */
  backgroundAverageImage.create(size, CV_8UC3);
  acc.convertTo(backgroundAverageImage, CV_8UC3);

  

  /* 分散を計算する */
  cv::Mat  dispersion;
  dispersion.create(size, CV_32FC3);
  cv::multiply(acc, acc, acc);
  
  cv::subtract(acc2, acc, dispersion);
  
  

  /* 標準偏差を計算する */
  cv::Mat  sd;
  sd.create(size, CV_32FC3);
  
  cv::pow(dispersion, 0.5, sd);
  
  



  /* 閾値を計算する */
  backgroundThresholdImage.create(size, CV_8UC3);
  sd.convertTo(backgroundThresholdImage, CV_8U, thre_coefficient);
  
  
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



  /* 閾値の初期設定を表示 */
  printf( "threshold: %s\n", mode_str[1-mode] );

  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageSubstraction::onDeactivated(RTC::UniqueId ec_id)
{



  if (!differenceImage.empty())
  {
	  differenceImage.release();
  }
  if (!originalImage.empty())
  {
	  originalImage.release();
  }
  if (!resultImage.empty())
  {
	  resultImage.release();
  }
  if (!outputImage.empty())
  {
	  outputImage.release();
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
		if (g_temp_w != 0 && g_temp_h != 0)
		{
			/* 'b'キーが押されたらその時点での画像を背景画像とする */
			initializeBackgroundModel(NUM_OF_BACKGROUND_FRAMES, cv::Size(g_temp_w, g_temp_h), m_thre_coefficient);

			printf("Background image update\n");   /* 背景情報更新 */
		}

    } else if( m_cont_mode == 'm' ){
      /* 'm'キーが押されたら閾値の設定方法を変更する */
      mode = 1 - mode;
      printf( "threshold: %s\n", mode_str[mode] );
    }
  }
				
  /* 初期値を取得する */
  if(ImageSubstraction_count == 0 && m_img_origIn.isNew()) {

    m_img_origIn.read();

    if(g_temp_w != m_img_orig.width || g_temp_h != m_img_orig.height){


	  if (originalImage.empty()){
		  originalImage.create(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC3);  /* キャプチャ画像用IplImage */
      }

	  if (outputImage.empty()){
		  outputImage.create(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC3);
      }

      memcpy(originalImage.data,(void *)&(m_img_orig.pixels[0]), m_img_orig.pixels.length());

      
      if(differenceImage.empty()){
		  differenceImage = originalImage.clone();
      }

	  if (resultImage.empty()){
		  resultImage.create(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC3);
      }

      initializeBackgroundModel( NUM_OF_BACKGROUND_FRAMES, cv::Size(m_img_orig.width, m_img_orig.height) , m_thre_coefficient);

      ImageSubstraction_count = 1;
      g_temp_w = m_img_orig.width;
      g_temp_h = m_img_orig.height;
    }
  }

  if(ImageSubstraction_count == 1 && m_img_origIn.isNew()) {

    m_img_origIn.read();

    if(g_temp_w == m_img_orig.width && g_temp_h == m_img_orig.height){


      if(originalImage.empty()){
		  originalImage.create(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC3);  /* キャプチャ画像用IplImage */
      }


      if(outputImage.empty()){
		  outputImage.create(cv::Size(m_img_orig.width, m_img_orig.height), CV_8UC3);
      }

      memcpy(originalImage.data,(void *)&(m_img_orig.pixels[0]), m_img_orig.pixels.length());

      /* 現在の背景との差の絶対値を成分ごとに取る */
	  cv::absdiff(originalImage, backgroundAverageImage, differenceImage);

      /* Sub はマイナスになったら0に切り詰めてくれる */
      if( mode == DYNAMIC_MODE ){
		  cv::subtract(differenceImage, backgroundThresholdImage, differenceImage);
      } else{
		  cv::subtract(differenceImage, cv::Scalar::all(m_constant_thre), differenceImage);
      }

      /* differenceImage の要素が1つでも0以上だったら前景 */
	  cv::cvtColor(differenceImage, resultImage, cv::COLOR_BGR2GRAY);
	  cv::threshold(resultImage, resultImage, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

      /* メディアンフィルタでノイズを除去する */
	  cv::medianBlur(resultImage, resultImage, 7);

	  cv::Mat tmp = differenceImage.clone();
	  tmp.convertTo(tmp, CV_8U, 3);
      //showFlipImage( windowNameThreshold, tmp );
	  std::vector<cv::Mat> tmp_vec;
	  tmp_vec.push_back(resultImage);
	  tmp_vec.push_back(resultImage);
	  tmp_vec.push_back(resultImage);

	  cv::merge(tmp_vec, outputImage);

      /* 画像データのサイズ取得 */
	  double len1 = (originalImage.channels() * originalImage.size().width * originalImage.size().height);
	  double len2 = (outputImage.channels() * outputImage.size().width * outputImage.size().height);
	  double len3 = (backgroundAverageImage.channels() * backgroundAverageImage.size().width * backgroundAverageImage.size().height);
	  double len4 = (tmp.channels() * tmp.size().width * tmp.size().height);

      m_img_capture.pixels.length(len1);
      m_img_result.pixels.length(len2);
      m_img_back.pixels.length(len3);
      m_img_threshold.pixels.length(len4);

      /* 該当のイメージをMemCopyする */
      memcpy((void *)&(m_img_capture.pixels[0]), originalImage.data, len1);
	  memcpy((void *)&(m_img_result.pixels[0]), outputImage.data, len2);
	  memcpy((void *)&(m_img_back.pixels[0]), backgroundAverageImage.data, len3);
	  memcpy((void *)&(m_img_threshold.pixels[0]), tmp.data, len4);

	  m_img_capture.width = originalImage.size().width;
	  m_img_capture.height = originalImage.size().height;

	  m_img_result.width = originalImage.size().width;
	  m_img_result.height = originalImage.size().height;

	  m_img_back.width = originalImage.size().width;
	  m_img_back.height = originalImage.size().height;

	  m_img_threshold.width = originalImage.size().width;
	  m_img_threshold.height = originalImage.size().height;

      m_img_captureOut.write();
      m_img_resultOut.write();
      m_img_backOut.write();
      m_img_thresholdOut.write();



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


