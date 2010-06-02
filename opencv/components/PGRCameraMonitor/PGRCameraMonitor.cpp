// -*- C++ -*-
/*!
 * @file  PGRCameraMonitor.cpp
 * @brief PGRCameraMonitor
 * @date $Date$
 *
 * $Id$
 */

#include "PGRCameraMonitor.h"

// Module specification
// <rtc-template block="module_spec">
static const char* pgrcameramonitor_spec[] =
  {
    "implementation_id", "PGRCameraMonitor",
    "type_name",         "PGRCameraMonitor",
    "description",       "PGRCameraMonitor",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    "exec_cxt.periodic.rate", "1000.0",
	// Configuration variables
    "conf.default.image_height", "480",
    "conf.default.image_width", "640",
    ""
  };
// </rtc-template>

IplImage* m_img;
/*!
 * @brief constructor
 * @param manager Maneger Object
 */
PGRCameraMonitor::PGRCameraMonitor(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_inIn("in", m_in)

    // </rtc-template>
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  registerInPort("in", m_inIn);
  
  // Set OutPort buffer
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

}

/*!
 * @brief destructor
 */
PGRCameraMonitor::~PGRCameraMonitor()
{
}



RTC::ReturnCode_t PGRCameraMonitor::onInitialize()
{
  bindParameter("image_height", m_img_height, "240");
  bindParameter("image_width", m_img_width, "320");

  return RTC::RTC_OK;
}


/*
RTC::ReturnCode_t PGRCameraMonitor::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t PGRCameraMonitor::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t PGRCameraMonitor::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t PGRCameraMonitor::onActivated(RTC::UniqueId ec_id)
{

  m_img=cvCreateImage(cvSize(m_img_width,m_img_height),IPL_DEPTH_8U,3);
  
  //画像表示用ウィンドウの作成
  cvNamedWindow("CaptureImage", CV_WINDOW_AUTOSIZE);

  return RTC::RTC_OK;
}


RTC::ReturnCode_t PGRCameraMonitor::onDeactivated(RTC::UniqueId ec_id)
{
  cvReleaseImage(&m_img);
  //表示ウィンドウの消去
  cvDestroyWindow("CaptureImage");

  return RTC::RTC_OK;
}


RTC::ReturnCode_t PGRCameraMonitor::onExecute(RTC::UniqueId ec_id)
{
  static coil::TimeValue tm_pre;
  static int count = 0;
  
  if (!m_inIn.isNew())
    {
      return RTC::RTC_OK;
    }
  
  m_inIn.read();
  if (!(m_in.pixels.length() > 0))
    {
      return RTC::RTC_OK;
    }
  
  memcpy(m_img->imageData,(void *)&(m_in.pixels[0]),m_in.pixels.length());
  
  //画像表示
  cvShowImage("CaptureImage", m_img);
  
  cvWaitKey(1);
  if (count > 100)
    {
      count = 0;
      coil::TimeValue tm;
      tm = coil::gettimeofday();
      double sec(tm - tm_pre);
      if (sec > 1.0 && sec < 1000.0)
        {
          std::cout << 100.0/sec << " [FPS]" << std::endl;
        }
      tm_pre = tm;
    }
  ++count;
  
  return RTC::RTC_OK;

}

/*
RTC::ReturnCode_t PGRCameraMonitor::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t PGRCameraMonitor::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t PGRCameraMonitor::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t PGRCameraMonitor::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t PGRCameraMonitor::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void PGRCameraMonitorInit(RTC::Manager* manager)
  {
    coil::Properties profile(pgrcameramonitor_spec);
    manager->registerFactory(profile,
                             RTC::Create<PGRCameraMonitor>,
                             RTC::Delete<PGRCameraMonitor>);
  }
  
};


