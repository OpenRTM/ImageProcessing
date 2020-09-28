// -*- C++ -*-
/*!
 * @file  ImageCalibration.cpp
 * @brief Image Calibration
 * @date $Date$
 *
 * $Id$
 */

#include "ImageCalibration.h"

// Module specification
// <rtc-template block="module_spec">
static const char* imagecalibration_spec[] =
  {
    "implementation_id", "ImageCalibration",
    "type_name",         "ImageCalibration",
    "description",       "Image Calibration",
    "version",           "1.2.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.checker_w", "13",
    "conf.default.checker_h", "9",
    "conf.default.image_num", "5",
    // Widget
    "conf.__widget__.checker_w", "text",
    "conf.__widget__.checker_h", "text",
    "conf.__widget__.image_num", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
ImageCalibration::ImageCalibration(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_img_origIn("original_image", m_img_orig),
    m_img_checkOut("checker_image", m_img_check),
    m_CameraCalibrationServicePort("CameraCalibrationService")

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
ImageCalibration::~ImageCalibration()
{
}



RTC::ReturnCode_t ImageCalibration::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("original_image", m_img_origIn);
  
  // Set OutPort buffer
  addOutPort("checker_image", m_img_checkOut);
  
  // Set service provider to Ports
  m_CameraCalibrationServicePort.registerProvider("CalibrationService", "ImageCalibService::CalibrationService", m_CalibrationService);
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  addPort(m_CameraCalibrationServicePort);
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("checker_w", m_checker_w, "13");
  bindParameter("checker_h", m_checker_h, "9");
  bindParameter("image_num", m_image_num, "5");
  // </rtc-template>
  
  m_CalibrationService.setCheckerSize(m_checker_w, m_checker_h);
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ImageCalibration::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageCalibration::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageCalibration::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t ImageCalibration::onActivated(RTC::UniqueId ec_id)
{
  /* チェスボード撮影枚数セット */
  m_current_image_num = m_image_num;
  m_CalibrationService.setImageNumber(m_current_image_num);

  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageCalibration::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageCalibration::onExecute(RTC::UniqueId ec_id)
{
  cv::Mat image, gray;
  std::vector<cv::Point2f>	imagePoints;  /* チェッカー交点座標を格納する行列 */
  
  /* コーナー位置高精度化のための繰り返し処理の停止基準
   * 「反復回数が20回に達する」または「イプシロンが0.001に達する」どちらかの条件を満たした時に終了する 
   */
#if CV_MAJOR_VERSION < 3
  cv::TermCriteria	criteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.001 );
#else
  cv::TermCriteria	criteria( cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 20, 0.001 );
#endif
 
  if(m_img_origIn.isNew())
  {
    /* チェスボード撮影枚数確認 */
    checkImageNum();
    
    /* データの読み込み */
    m_img_origIn.read();
    
    /* イメージ用メモリの確保 */
    image.create(m_img_orig.height, m_img_orig.width, CV_8UC3);
    gray.create(m_img_orig.height, m_img_orig.width, CV_8UC1);

    /* InPortの映像データ */
    memcpy(image.data,(void *)&(m_img_orig.pixels[0]), m_img_orig.pixels.length());
    
    /* グレースケールに変換 */
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    
    /* プロバイダクラスへデータを渡す */
    RTC::CameraImage currentImg;
    int len = gray.channels() * gray.cols * gray.rows;   
    currentImg.pixels.length(len);
    currentImg.width = gray.cols;
    currentImg.height = gray.rows;
    memcpy((void *)&(currentImg.pixels[0]), gray.data, len);
    m_CalibrationService.setCurrentImage(&currentImg);
    
    /* チェッカーパターンの交点検出 */
    if(cv::findChessboardCorners(gray, cv::Size(m_checker_w, m_checker_h), imagePoints))
    {
      /* 検出点を描画 */  
      cv::cornerSubPix(gray, imagePoints, cv::Size(11,11), cv::Size(-1,-1), criteria );
      cv::drawChessboardCorners(image, cv::Size(m_checker_w, m_checker_h), (cv::Mat)imagePoints, true );
    }
    
    /* 画像データのサイズ取得 */
    len = image.channels() * image.cols * image.rows;
    m_img_check.pixels.length(len);
    m_img_check.width = image.cols;
    m_img_check.height = image.rows;        

    /* 画像データをOutPortにコピー */
    memcpy((void *)&(m_img_check.pixels[0]), image.data, len); 
  
    m_img_checkOut.write();
  }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ImageCalibration::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageCalibration::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageCalibration::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageCalibration::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageCalibration::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/* チェスボード撮影枚数確認
 * 枚数を変更されたらGUIを再描画し、それまでの保存画像を削除する
 */
void ImageCalibration::checkImageNum(void)
{
  if(m_current_image_num == m_image_num) return;
  m_current_image_num = m_image_num;
  m_CalibrationService.setImageNumber(m_image_num);
  return;
}

extern "C"
{
 
  void ImageCalibrationInit(RTC::Manager* manager)
  {
    coil::Properties profile(imagecalibration_spec);
    manager->registerFactory(profile,
                             RTC::Create<ImageCalibration>,
                             RTC::Delete<ImageCalibration>);
  }
  
};


