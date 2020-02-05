  // -*- C++ -*-
/*!
 * @file  SubStractCaptureImage.cpp
 * @brief SubStractCaptureImage component
 * @date $Date$
 *
 * $Id$
 */

#include "SubStractCaptureImage.h"

// Module specification
// <rtc-template block="module_spec">
static const char* substractcaptureimage_spec[] =
  {
    "implementation_id", "SubStractCaptureImage",
    "type_name",         "SubStractCaptureImage",
    "description",       "SubStractCaptureImage component",
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

CvSize imageSize;

int g_temp_w = 0;
int g_temp_h = 0;
int SubStractCaptureImage_count = 0;

/*
void showFlipImage(char *windowName, cv::Mat image) {
	if ( image->origin == 0 ) {
		//cvFlip( image, image, 0 );
		cvShowImage( windowName, image );
		//cvFlip( image, image, 0 );
	}
}
*/

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
SubStractCaptureImage::SubStractCaptureImage(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("original_image", m_image_orig),
    m_image_outOut("output_image", m_image_out),
    m_foreMaskImgOut("foreMaskImg", m_foreMaskImg),
    m_stillMaskImgOut("stillMaskImg", m_stillMaskImg),
    m_backGroundImgOut("backGroundImg", m_backGroundImg),
    m_stillImgOut("stillImg", m_stillImg)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
SubStractCaptureImage::~SubStractCaptureImage()
{
}



RTC::ReturnCode_t SubStractCaptureImage::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("original_image", m_image_origIn);
  
  // Set OutPort buffer
  addOutPort("output_image", m_image_outOut);
  addOutPort("foreMaskImg", m_foreMaskImgOut);
  addOutPort("stillMaskImg", m_stillMaskImgOut);
  addOutPort("backGroundImg", m_backGroundImgOut);
  addOutPort("stillImg", m_stillImgOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SubStractCaptureImage::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SubStractCaptureImage::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SubStractCaptureImage::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t SubStractCaptureImage::onActivated(RTC::UniqueId ec_id)
{
  g_temp_w = 0;
  g_temp_h = 0;
  SubStractCaptureImage_count = 0;



  return RTC::RTC_OK;
}


RTC::ReturnCode_t SubStractCaptureImage::onDeactivated(RTC::UniqueId ec_id)
{	

	



	

	return RTC::RTC_OK;
}


RTC::ReturnCode_t SubStractCaptureImage::onExecute(RTC::UniqueId ec_id)
{	
  if(m_image_origIn.isNew() && SubStractCaptureImage_count == 0)
  {
    m_image_origIn.read();

    if(g_temp_w != m_image_orig.width || g_temp_h != m_image_orig.height)
    {
      /* 画像サイズを保存 */
      imageSize = cv::Size(m_image_orig.width, m_image_orig.height);

      /* 画像を生成する */

	  
	  //inputImage.create(imageSize, CV_8UC3);				 /* 背景の平均値保存用IplImage */
	  backgroundAverageImage.create(imageSize, CV_32FC3);	 /* 背景の閾値保存用IplImage */
	  backgroundThresholdImage.create(imageSize, CV_32FC3);	 /* 静止物体の平均値保存用IplImage */
	  stillObjectAverageImage.create(imageSize, CV_32FC3);	   /* 静止物体の閾値保存用IplImage */
	  stillObjectCounterImage.create(imageSize, CV_8UC1);/* 静止物体のカウンタ用IplImage */
	  stillObjectThresholdImage.create(imageSize, CV_32FC3);/* 背景差分画像用IplImage */
	  stillObjectCounterImage.create(imageSize, CV_32FC3);	    /* 静止物体差分画像用IplIMage */
	  backgroundDifferenceImage.create(imageSize, CV_32FC3);	  /* 32bitの閾値画像用IplImage */ 
	  stillObjectDifferenceImage.create(imageSize, CV_32FC3);		/* 閾値画像用IplImage */
	  thresholdImage32.create(imageSize, CV_32FC3);/* 結果画像用IplImage */
	  thresholdImage.create(imageSize, CV_8UC3);	/* 背景マスク画像用IplImage */
	  resultImage.create(imageSize, CV_8UC1);		/* 前景マスク用IplImage */	
	  backgroundMaskImage.create(imageSize, CV_8UC1);	/* 静止物体マスク用IplImage */
	  foregroundMaskImage.create(imageSize, CV_8UC1);	/* 動物体マスク用IplImage */ 
	  stillObjectMaskImage.create(imageSize, CV_8UC1);	/* 背景にコピーする際に使用するマスク用IplImage */
	  movingObjectMask.create(imageSize, CV_8UC1);	/* テンポラリ用IplImage */
	  backgroundCopyMaskImage.create(imageSize, CV_8UC1); /* テンポラリ用IplImage(その2) */
	  tmpMaskImage.create(imageSize, CV_8UC1);		 /* 32bitのキャプチャした画像用IplImage */	 
	  tmp2MaskImage.create(imageSize, CV_8UC1);	/* 背景画像用IplImage */		 
	  frameImage32.create(imageSize, CV_32FC3);			    
	  backgroundImage.create(imageSize, CV_8UC3);
	  stillObjectImage.create(imageSize, CV_8UC3);
	  outputImage.create(imageSize, CV_8UC3);
	  foreGroundMaskBuff.create(imageSize, CV_8UC3);
	  stillObjectMaskBuff.create(imageSize, CV_8UC3);
	  backGroundBuff.create(imageSize, CV_8UC3);
	  stillObjectImageBuff.create(imageSize, CV_8UC3);
	  stillObjectCounterBuff.create(imageSize, CV_8UC3);

	  cv::Mat inputImage(imageSize, CV_8UC3, (void *)&(m_image_orig.pixels[0]));
      //memcpy(inputImage.data,(void *)&(m_image_orig.pixels[0]), m_image_orig.pixels.length());

      /* 初期化する */
	  inputImage.convertTo(backgroundAverageImage, CV_32FC3);
      
	  backgroundThresholdImage.setTo(cv::Scalar::all(BACKGROUND_INITIAL_THRESHOLD));
	  stillObjectAverageImage = cv::Mat::zeros(imageSize, CV_32FC3);
	  stillObjectThresholdImage = cv::Mat::zeros(imageSize, CV_32FC3);
	  stillObjectCounterImage = cv::Mat::zeros(imageSize, CV_8UC1);


      g_temp_w = m_image_orig.width;
      g_temp_h = m_image_orig.height;

      SubStractCaptureImage_count = 1;

    }

  }else if(m_image_origIn.isNew() && SubStractCaptureImage_count != 0 )
  {
    m_image_origIn.read();

    if(g_temp_w == m_image_orig.width && g_temp_h == m_image_orig.height)
    {
		cv::Mat inputImage(imageSize, CV_8UC3, (void *)&(m_image_orig.pixels[0]));
      //memcpy(inputImage.data,(void *)&(m_image_orig.pixels[0]), m_image_orig.pixels.length());

      /* float 32bitに変換する */
	  
	  inputImage.convertTo(frameImage32, CV_32FC3);
	  

      // 背景との差 /////////////////////////////////////////////////////////

      /* 現在の背景との差の絶対値を成分ごとに取る */
      cv::absdiff( frameImage32, backgroundAverageImage, backgroundDifferenceImage );
	  
      /* 閾値の値を引く */
      cv::addWeighted( backgroundDifferenceImage, 1.0, backgroundThresholdImage, -THRESHOLD_COEFFICIENT, 0.0, thresholdImage32 );

      /* thresholdImage の要素が1つでも0以上だったら背景ではない */
	  thresholdImage32.convertTo(thresholdImage, CV_8UC3);
	  cv::cvtColor(thresholdImage, resultImage, cv::COLOR_BGR2GRAY);
      cv::threshold( resultImage, backgroundMaskImage, MASK_THRESHOLD, THRESHOLD_MAX_VALUE, cv::THRESH_BINARY_INV );

      // 背景候補との差 /////////////////////////////////////////////////////

      /* 現在の背景候補との差の絶対値を成分ごとに取る */
      cv::absdiff( frameImage32, stillObjectAverageImage, stillObjectDifferenceImage );

      /* 閾値の値を引く */
	  cv::addWeighted(stillObjectDifferenceImage, 1.0, stillObjectThresholdImage, -THRESHOLD_COEFFICIENT, 0.0, thresholdImage32); 

      /* thresholdImage の要素が1つでも0以上だったら背景候補ではない */
	  thresholdImage32.convertTo(thresholdImage, CV_8UC3);
	  cv::cvtColor(thresholdImage, resultImage, cv::COLOR_BGR2GRAY);
      cv::threshold( resultImage, stillObjectMaskImage, MASK_THRESHOLD, THRESHOLD_MAX_VALUE, cv::THRESH_BINARY_INV );
	  
      /* ここまでで、
       * backgroundDifferenceImage, backgroundMaskImage
       * stillObjectDifferenceImage, stillObjectMaskImage
       * に意味のある値が入る。
       */

      // 各種情報を更新する /////////////////////////////////////////////////

      /* 背景に同化する場合 (backgroundMaskImage=1の場合) */
	  
	  cv::accumulateWeighted(frameImage32, backgroundAverageImage, BACKGROUND_ALPHA, backgroundMaskImage);
	  cv::accumulateWeighted( backgroundDifferenceImage, backgroundThresholdImage, BACKGROUND_ALPHA, backgroundMaskImage);
	  
      /* 背景候補に同化する場合 (backgroundMaskImage=0 && stillObjectMaskImage=1) */
	  cv::bitwise_not(backgroundMaskImage, foregroundMaskImage);
	  cv::bitwise_and(foregroundMaskImage, stillObjectMaskImage, tmpMaskImage); /*  */

	  cv::accumulateWeighted(frameImage32, stillObjectAverageImage, STILL_OBJECT_ALPHA, tmpMaskImage);
	  cv::accumulateWeighted(stillObjectDifferenceImage, stillObjectThresholdImage, STILL_OBJECT_ALPHA, tmpMaskImage);

      /* 背景候補カウンタを増やす */
	  
	  cv::add(stillObjectCounterImage, cv::Scalar::all(1), stillObjectCounterImage, tmpMaskImage);
	  
      /* カウンタが閾値以上になったら、背景候補を背景として採用する */
	  
	  cv::threshold(stillObjectCounterImage, tmp2MaskImage, STILL_OBJECT_TO_BACKGROUND, THRESHOLD_MAX_VALUE, cv::THRESH_BINARY);
	  cv::bitwise_and(tmpMaskImage, tmp2MaskImage, backgroundCopyMaskImage);
	  stillObjectAverageImage.copyTo(backgroundAverageImage, backgroundCopyMaskImage);
	  stillObjectThresholdImage.copyTo(backgroundThresholdImage, backgroundCopyMaskImage);
      

      /* この backgroundCopyMaskImage は、後で背景候補を0に初期化する際に使用 */
	  stillObjectCounterImage.setTo(cv::Scalar::all(0), backgroundCopyMaskImage);
	  
      /* 背景候補でもなく、背景でもない場合 */
      /* (foregroundMaskImage = 1 && stillObjectMaskImage = 0) */
	  
	  cv::bitwise_not(stillObjectMaskImage, movingObjectMask);
	  

      /* カウンタを減らす(短時,間ノイズ対応)
       * これは、背景に分類されたピクセルに対しても行う。すなわち、
       * movingObjectMask == 1 || backgroundMaskImage == 1
       */
	  cv::bitwise_or(backgroundMaskImage, movingObjectMask, tmpMaskImage);
	  cv::subtract(stillObjectCounterImage, cv::Scalar::all(NOT_STILL_DEC_STEP), stillObjectCounterImage, tmpMaskImage);
	  
      /* カウンタが0になったら背景候補を初期化する */
	  cv::bitwise_not(stillObjectCounterImage, tmp2MaskImage);	/* tmp2 = 1 なら初期化 */

      /* 初期化する値の候補は2種類ある。
       * (1)現在の画像で初期化 --- 背景でもなく背景候補でもない場合
       * (2)登録なし状態で初期化 --- 背景もしくは背景候補をコピーした場合
       * ここでは(1)で初期化しておく
       */
	  cv::bitwise_or(tmpMaskImage, backgroundCopyMaskImage, tmpMaskImage);
	  cv::bitwise_and(tmpMaskImage, tmp2MaskImage, tmpMaskImage);

	  frameImage32.copyTo(stillObjectAverageImage, tmpMaskImage);
	  stillObjectThresholdImage.setTo(cv::Scalar::all(STILL_OBJECT_INITIAL_THRESHOLD), tmpMaskImage);
	  

      /* ノイズを除去する */
	  cv::medianBlur(foregroundMaskImage, foregroundMaskImage, 7);

	  backgroundAverageImage.convertTo(backgroundImage, CV_8UC3);
	  stillObjectAverageImage.convertTo(stillObjectImage, CV_8UC3);

      cvWaitKey( 1 );

      /* 画像データのサイズ取得 */
      double len;
	  len = (outputImage.channels() * outputImage.size().width * outputImage.size().height);
      m_image_out.pixels.length(len);

      memcpy((void *)&(m_image_out.pixels[0]), inputImage.data, len);

      m_image_out.width = m_image_orig.width;
      m_image_out.height = m_image_orig.height;

      m_image_outOut.write();

	  std::vector<cv::Mat> tmp;
	  tmp.push_back(foregroundMaskImage);
	  tmp.push_back(foregroundMaskImage);
	  tmp.push_back(foregroundMaskImage);

	  cv::merge(tmp, foreGroundMaskBuff);

	  len = (foreGroundMaskBuff.channels() * foreGroundMaskBuff.size().width * foreGroundMaskBuff.size().height);
      m_foreMaskImg.pixels.length(len);

      memcpy((void *)&(m_foreMaskImg.pixels[0]), foreGroundMaskBuff.data, len);

      m_foreMaskImg.width = m_image_orig.width;
      m_foreMaskImg.height = m_image_orig.height;

      m_foreMaskImgOut.write();
	  
	  tmp.clear();
	  tmp.push_back(stillObjectMaskImage);
	  tmp.push_back(stillObjectMaskImage);
	  tmp.push_back(stillObjectMaskImage);

	  cv::merge(tmp, stillObjectMaskBuff);
      
	  len = (stillObjectMaskBuff.channels() * stillObjectMaskBuff.size().width * stillObjectMaskBuff.size().height);
      m_stillMaskImg.pixels.length(len);

      memcpy((void *)&(m_stillMaskImg.pixels[0]), stillObjectMaskBuff.data, len);

      m_stillMaskImg.width = m_image_orig.width;
      m_stillMaskImg.height = m_image_orig.height;

      m_stillMaskImgOut.write();


	  len = (backgroundImage.channels() * backgroundImage.size().width * backgroundImage.size().height);
      m_backGroundImg.pixels.length(len);

      memcpy((void *)&(m_backGroundImg.pixels[0]), backgroundImage.data, len);

      m_backGroundImg.width = m_image_orig.width;
      m_backGroundImg.height = m_image_orig.height;

      m_backGroundImgOut.write();


	  len = (stillObjectImage.channels() * stillObjectImage.size().width * stillObjectImage.size().height);
      m_stillImg.pixels.length(len);

      memcpy((void *)&(m_stillImg.pixels[0]), stillObjectImage.data, len);

      m_stillImg.width = m_image_orig.width;
      m_stillImg.height = m_image_orig.height;

      m_stillImgOut.write();

      /*
      cvMerge( stillObjectCounterImage, stillObjectCounterImage, stillObjectCounterImage, NULL, stillObjectCounterBuff );

      len = (stillObjectCounterBuff->nChannels * stillObjectCounterBuff->width * stillObjectCounterBuff->height);
      m_stillCounterImg.pixels.length(len);

      memcpy((void *)&(m_stillCounterImg.pixels[0]), stillObjectCounterBuff->imageData, len);

      m_stillCounterImg.width = m_image_orig.width;
      m_stillCounterImg.height = m_image_orig.height;

      m_stillCounterImgOut.write();
      */
      g_temp_w = m_image_orig.width;
      g_temp_h = m_image_orig.height;

      key = '0';
    }
  }

  if(g_temp_w != m_image_orig.width || g_temp_h != m_image_orig.height)
  {

    SubStractCaptureImage_count = 0;
  }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SubStractCaptureImage::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SubStractCaptureImage::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SubStractCaptureImage::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SubStractCaptureImage::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SubStractCaptureImage::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void SubStractCaptureImageInit(RTC::Manager* manager)
  {
    coil::Properties profile(substractcaptureimage_spec);
    manager->registerFactory(profile,
                             RTC::Create<SubStractCaptureImage>,
                             RTC::Delete<SubStractCaptureImage>);
  }
  
};


