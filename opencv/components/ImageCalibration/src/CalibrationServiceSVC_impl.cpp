// -*-C++-*-
/*!
 * @file  CalibrationServiceSVC_impl.cpp
 * @brief Service implementation code of CalibrationService.idl
 *
 */

#include <time.h>
#include "CalibrationServiceSVC_impl.h"
#include <rtm/CORBA_SeqUtil.h>

/*
 * Example implementational code for IDL interface ImageCalibService::CalibrationService
 */
CalibrationServiceSVC_impl::CalibrationServiceSVC_impl()
  :m_imageList(0)
{
  m_current_image_num = 0;
  m_old_image_num = 0;
}


CalibrationServiceSVC_impl::~CalibrationServiceSVC_impl()
{
  // Please add extra destructor code here.
}


/*
 * Methods corresponding to IDL attributes and operations
 */
void CalibrationServiceSVC_impl::setImageNumber(CORBA::Short num)
{
  m_old_image_num = m_current_image_num;
  m_current_image_num = num;
  if(m_old_image_num != 0 && m_old_image_num != m_current_image_num)
  {
    /* 保存画像を削除してGUI再描画 */
    std::cout << "setImageNumber : m_old_image_num = " << m_old_image_num << std::endl;
    std::cout << "setImageNumber : m_current_image_num = " << m_current_image_num << std::endl;
    removeCalibImage(-1);
  }
  return;
}

CORBA::Short CalibrationServiceSVC_impl::getImageNumber()
{
  m_mutex.lock();
  CORBA::Short ret_num = m_current_image_num;
  m_mutex.unlock();
  
  return ret_num;
}

RTC::CameraImage* CalibrationServiceSVC_impl::captureCalibImage(CORBA::Short num)
{
  cv::Mat frame;
  std::string filename = "capture";
  std::ostringstream oss;
  RTC::CameraImage_var vl;
  
  m_mutex.lock();
  vl = new RTC::CameraImage(m_currentCameraImg);
  frame.create(m_currentCameraImg.height, m_currentCameraImg.width, CV_8UC1);
  memcpy(frame.data,(void *)&(m_currentCameraImg.pixels[0]), m_currentCameraImg.pixels.length());

  CORBA::ULong len(m_imageList.length());
  if(len != (CORBA::ULong)m_current_image_num)
  {
    m_imageList.length(m_current_image_num);    
  }
  m_imageList[num] = m_currentCameraImg;
  m_mutex.unlock();
  
  filename = getSavePath() + filename;
  oss << filename << num << ".jpg";
  cv::imwrite(oss.str(), frame);
  return vl._retn();
}

RTC::CameraImage* CalibrationServiceSVC_impl::getCalibImage(CORBA::Short num)
{
  ImageCalibService::ImageList_var list;
  RTC::CameraImage_var image;
  list = new ImageCalibService::ImageList(m_imageList);
  image = new RTC::CameraImage(list[num]);

  return image._retn();
}

ImageCalibService::ImageList* CalibrationServiceSVC_impl::getCalibImages()
{
  ImageCalibService::ImageList_var list;
  list = new ImageCalibService::ImageList(m_imageList);

  return list._retn();
}

CORBA::Boolean CalibrationServiceSVC_impl::removeCalibImage(CORBA::Short num)
{
  std::string filename = "capture";
  std::ostringstream oss;
  CORBA::Boolean ret = true;
  
  filename = getSavePath() + filename;
  
  if(num == -1)
  {
    /* 全画像削除 */
    m_mutex.lock();
    CORBA::ULong len(m_imageList.length());
    CORBA_SeqUtil::clear(m_imageList);
    
    for(int i=0; i<(CORBA::Short)len; i++)
    {
      oss.str("");
      oss << filename << i << ".jpg";
      removeFile(oss.str());
    }
    m_mutex.unlock();
  }
  else
  {
    /* 個別削除の場合は、インデックスを詰めないのでメモリ上のデータはそのまま
     * 指定されたインデックスの画像ファイルのみ削除する
     */
    oss << filename << num << ".jpg";
    ret = removeFile(oss.str());
  }
    
  return ret;
}

RTC::CameraInfo CalibrationServiceSVC_impl::getCalibParameter()
{
  int i,j;
  RTC::CameraInfo cameraInfo = {{0.0},{0.0},0.0};
  cv::Mat gray;
  RTC::CameraImage image;
  /* チェッカー交点座標を格納する行列 */
  std::vector<std::vector<cv::Point2f> >	imagePoints(m_current_image_num);

  /* チェッカー交点座標と対応する世界座標の値を格納する行列 */
  std::vector<std::vector<cv::Point3f> >  worldPoints(m_current_image_num);
  
  /* カメラパラメータ行列 */
  cv::Mat cameraMatrix;
  cv::Mat distCoeffs;
  std::vector<cv::Mat> rotationVectors;
  std::vector<cv::Mat> translationVectors;
  
  /* コーナー位置高精度化のための繰り返し処理の停止基準
   * 「反復回数が20回に達する」または「イプシロンが0.001に達する」どちらかの条件を満たした時に終了する
   */
  cv::TermCriteria	criteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.001 );

  /* 保存されている画像枚数チェック */

  CORBA::ULong len(m_imageList.length());
  if((CORBA::Short)len != m_current_image_num)
  {
    /* 指定枚数の画像が保存されていないのでカメラパラメータは算出しない */
    std::cout << "getCalibParameter : error(image missing)" << std::endl;
    return cameraInfo;
  }
  
  /* チェッカーパターンの交点検出 */
  for( i = 0; i < m_current_image_num; i++ )
  {
    image = m_imageList[i];
    gray.create(image.height, image.width, CV_8UC1);
    memcpy(gray.data,(void *)&(image.pixels[0]), image.pixels.length());
    if(cv::findChessboardCorners(gray, cv::Size(m_checker_w, m_checker_h), imagePoints[i]))
    {
//      std::cout << " ... All corners found." << std::endl;
    } else {
    	std::cout << " ... at least 1 corner not found." << std::endl;
    	return cameraInfo;
    }
    /* コーナー位置を高精度化 */
    cv::cornerSubPix(gray, imagePoints[i], cv::Size(11,11), cv::Size(-1,-1), criteria );
  }
  
  /* 世界座標を決める */
  for(i = 0; i < m_current_image_num; i++)
  {
    for(j = 0 ; j < cv::Size(m_checker_w, m_checker_h).area(); j++)
    {
      worldPoints[i].push_back(cv::Point3f(static_cast<float>( j % m_checker_w * 10), 
          static_cast<float>(j / m_checker_w * 10), 0.0));
    }
  }

  /* カメラパラメータ算出 */
  cv::calibrateCamera( worldPoints, imagePoints, cv::Size(image.width, image.height), cameraMatrix,
                    distCoeffs, rotationVectors, translationVectors );
  
  cameraInfo.focalLength.x = cameraMatrix.at<double>(0,0);
  cameraInfo.focalLength.y = cameraMatrix.at<double>(1,1);
  cameraInfo.principalPoint.x = cameraMatrix.at<double>(0,2);
  cameraInfo.principalPoint.y = cameraMatrix.at<double>(1,2);
  cameraInfo.k1 = distCoeffs.at<double>(0);
  cameraInfo.k2 = distCoeffs.at<double>(1);
  cameraInfo.p1 = distCoeffs.at<double>(2);
  cameraInfo.p2 = distCoeffs.at<double>(3);
  
  /* 歪み補正した画像を出力 */
  outUndistortedImage(cameraMatrix, distCoeffs);
  
  /* yamlファイル出力 */
  outYamlFile(cameraMatrix, distCoeffs);
   
  return cameraInfo;
}

void CalibrationServiceSVC_impl::setCurrentImage(RTC::CameraImage* cameraimage)
{
  int len = cameraimage->pixels.length();
  m_currentCameraImg.pixels.length(len);
  m_currentCameraImg.width = cameraimage->width;
  m_currentCameraImg.height = cameraimage->height;
  memcpy((void *)&(m_currentCameraImg.pixels[0]), (void*)&(cameraimage->pixels[0]), len);
}

void CalibrationServiceSVC_impl::setCheckerSize(int width, int height)
{
  m_checker_w = width;
  m_checker_h = height;
}

CORBA::Boolean CalibrationServiceSVC_impl::removeFile(std::string name)
{
  std::cout << "removeFile : " << name.c_str() << std::endl;
  if(remove(name.c_str()) != 0)
  {
    std::cout << "removeFile : error(file remove) : " << name.c_str() << std::endl;
    return false;
  }
  return true;
}

void CalibrationServiceSVC_impl::outUndistortedImage(
				cv::Mat cameraMatrix, cv::Mat distCoeffs)
{
  cv::Mat gray, undistorted;
  std::string filename = "undistorted";
  std::ostringstream oss;
  RTC::CameraImage image;
  
  filename = getSavePath() + filename;
  
  for(int i = 0; i < m_current_image_num; i++ )
  {
    image = m_imageList[i];
    gray.create(image.height, image.width, CV_8UC1);
    memcpy(gray.data,(void *)&(image.pixels[0]), image.pixels.length());
    cv::undistort(gray, undistorted, cameraMatrix, distCoeffs);
    oss.str("");
    oss << filename << i << ".jpg";
    cv::imwrite(oss.str(), undistorted);
  }
  
  return;
}

void CalibrationServiceSVC_impl::outYamlFile(
				cv::Mat cameraMatrix, cv::Mat distCoeffs)
{
  cv::FileStorage fs("camera.yml", cv::FileStorage::WRITE);
  time_t rawtime; time(&rawtime);
  fs << "calibration_time" << asctime(localtime(&rawtime));
  fs << "image_width" << m_imageList[0].width;
  fs << "image_height" << m_imageList[0].height;
  fs << "board_width" << m_checker_w;
  fs << "board_height" << m_checker_h;
  fs << "cameraMatrix" << cameraMatrix << "distCoeffs" << distCoeffs;
  fs.release();
  
  return;
}

std::string CalibrationServiceSVC_impl::getSavePath()
{
  std::string path;
  
#ifdef WIN32
  char* strenv;
  if((strenv = getenv("TEMP")) != NULL || (strenv = getenv("TMP")) != NULL)
  {
    path = strenv;
    path += "\\";
  }else{
    path = "c:\\tmp\\";
  }
#else
  path = "/tmp/";
#endif

	std::cout << "getEnvironTmpPath : " << path.c_str() << std::endl;
	return path;
}

// End of example implementational code



