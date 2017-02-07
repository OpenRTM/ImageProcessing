// -*-C++-*-
/*!
 * @file  CalibrationServiceSVC_impl.h
 * @brief Service implementation header of CalibrationService.idl
 *
 */

#include "InterfaceDataTypesSkel.h"
#include "BasicDataTypeSkel.h"
#include "ExtendedDataTypesSkel.h"

#include "CalibrationServiceSkel.h"

#ifndef CALIBRATIONSERVICESVC_IMPL_H
#define CALIBRATIONSERVICESVC_IMPL_H

#include <iostream>
#include <coil/Mutex.h>

/* OpenCVHeadファイルのIncluding */
#include <opencv2/opencv.hpp>

/*!
 * @class CalibrationServiceSVC_impl
 * Example class implementing IDL interface ImageCalibService::CalibrationService
 */
class CalibrationServiceSVC_impl
 : public virtual POA_ImageCalibService::CalibrationService,
   public virtual PortableServer::RefCountServantBase
{
 private:
   // Make sure all instances are built on the heap by making the
   // destructor non-public
   //virtual ~CalibrationServiceSVC_impl();

 public:
  /*!
   * @brief standard constructor
   */
   CalibrationServiceSVC_impl();
  /*!
   * @brief destructor
   */
   virtual ~CalibrationServiceSVC_impl();

   // attributes and operations
   void setImageNumber(CORBA::Short num);
   CORBA::Short getImageNumber();
   RTC::CameraImage* captureCalibImage(CORBA::Short num);
   RTC::CameraImage* getCalibImage(CORBA::Short num);
   ImageCalibService::ImageList* getCalibImages();
   CORBA::Boolean removeCalibImage(CORBA::Short num);
   RTC::CameraInfo getCalibParameter();
   
   void setCurrentImage(RTC::CameraImage* cameraimage);
   void setCheckerSize(int width, int height);
   
 private:
   CORBA::Boolean removeFile(std::string name);
   void outUndistortedImage(cv::Mat cameraMatrix, cv::Mat distCoeffs);
   void outYamlFile(cv::Mat cameraMatrix, cv::Mat distCoeffs);
   std::string getSavePath();

   ImageCalibService::ImageList m_imageList;
   RTC::CameraImage m_currentCameraImg;
   coil::Mutex m_mutex;
   CORBA::Short m_current_image_num;  /* チェスボード撮影枚数 */
   CORBA::Short m_old_image_num;      /* 変更前のチェスボード撮影枚数 */
   int m_checker_w;       /* チェスボード横方向の交点の数 */
   int m_checker_h;       /* チェスボード縦方向の交点の数 */
   std::string m_cap_filename;
};



#endif // CALIBRATIONSERVICESVC_IMPL_H


