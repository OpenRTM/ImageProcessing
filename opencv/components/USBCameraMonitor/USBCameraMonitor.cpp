// -*- C++ -*-
/*!
 * @file  USBCameraMonitor.cpp
 * @brief USB Camera Monitor component
 * @date $Date$
 *
 * $Id$
 */

#include "USBCameraMonitor.h"

// Module specification
// <rtc-template block="module_spec">
static const char* usbcameramonitor_spec[] =
  {
    "implementation_id", "USBCameraMonitor",
    "type_name",         "USBCameraMonitor",
    "description",       "USB Camera Monitor component",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "example",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    "exec_cxt.periodic.rate", "1000.0",
    // Configuration variables
    "conf.default.image_height", "240",
    "conf.default.image_width", "320",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
USBCameraMonitor::USBCameraMonitor(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_inIn       ("in",          m_in),
    m_lKeyOut    ("Key out",     m_lKey),
    m_lMouseEvOut("Mouse event", m_lMouseEv),
    m_lMouseXOut ("Mouse X pos", m_lMouseX),
    m_lMouseYOut ("Mouse Y pos", m_lMouseY),

    // </rtc-template>
    dummy(0)
{
    // Registration: InPort/OutPort/Service
    // <rtc-template block="registration">
    // Set InPort buffers
    registerInPort ("in",          m_inIn);
    registerOutPort("Key out",     m_lKeyOut);
    registerOutPort("Mouse event", m_lMouseEvOut);
    registerOutPort("Mouse X pos", m_lMouseXOut);
    registerOutPort("Mouse Y pos", m_lMouseYOut);
  
  // Set OutPort buffer
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

}

/*!
 * @brief destructor
 */
USBCameraMonitor::~USBCameraMonitor()
{
}

RTC::ReturnCode_t USBCameraMonitor::onInitialize()
{
  bindParameter("image_height", m_img_height, "240");
  bindParameter("image_width", m_img_width,   "320");

  m_nOldHeight = m_img_height;
  m_nOldWidth  = m_img_width;

  m_lKey.data = 0;

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t USBCameraMonitor::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t USBCameraMonitor::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t USBCameraMonitor::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t USBCameraMonitor::onActivated(RTC::UniqueId ec_id)
{ 

    m_img       = NULL;
    m_orig_img  = NULL;

    m_in_height = 0;
    m_in_width  = 0;

  //画像表示用ウィンドウの作成
  cvNamedWindow("CaptureImage", CV_WINDOW_AUTOSIZE);
  cvSetMouseCallback("CaptureImage", onMouse, (void*)this);
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t USBCameraMonitor::onDeactivated(RTC::UniqueId ec_id)
{
    if(m_img != NULL)
        cvReleaseImage(&m_img);

    if(m_orig_img != NULL)
        cvReleaseImage(&m_orig_img);

    //表示ウィンドウの消去    
    cvDestroyWindow("CaptureImage");
    return RTC::RTC_OK;
}


RTC::ReturnCode_t USBCameraMonitor::onExecute(RTC::UniqueId ec_id)
{
    static coil::TimeValue tm_pre;
    static int count = 0;
	
    int nLength;

    m_lKey.data = cvWaitKey(1);
    if(m_lKey.data >= 0)
    {
        printf("[onExecute] Key number %d is down\n", m_lKey.data);
        m_lKeyOut.write();
    }

    // Check input image is new
    if (!m_inIn.isNew())
    {	
		return RTC::RTC_OK;
    }
    m_inIn.read();    

    nLength = m_in.pixels.length();

    if (!(nLength > 0))
    {
        return RTC::RTC_OK;
    }

    // 出力サイズはコンフィグレーションのをつかう
    if(m_img == NULL || isCFGChanged())
    {
        printf("[onExecute] CFG is changed!\n");
        
        m_nOldHeight = m_img_height;
        m_nOldWidth  = m_img_width;

        if(m_img != NULL)
            cvReleaseImage(&m_img);

        // チャンネルは３で固定
        m_img = cvCreateImage(cvSize(m_img_width, m_img_height), IPL_DEPTH_8U, 3);
    }

    // サイズが変わったときだけ再生成する
    if(m_in_height != (int)m_in.height || m_in_width != (int)m_in.width)
    {
        printf("[onExecute] Size of input image is not match!\n");
        
        if(m_orig_img != NULL)
            cvReleaseImage(&m_orig_img);

        m_in_height = m_in.height;
        m_in_width  = m_in.width;

        // サイズ変換のためTempメモリーをよいする
        m_orig_img = cvCreateImage(cvSize(m_in.width, m_in.height), IPL_DEPTH_8U, 3);
    }

    // データコピー
    memcpy(m_orig_img->imageData,(void *)&(m_in.pixels[0]), m_in.pixels.length());

    // 入力映像を出力サイズに合わせてコピー
    cvResize( m_orig_img, m_img, CV_INTER_LINEAR );

    //画像表示
    cvShowImage("CaptureImage", m_img);

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
RTC::ReturnCode_t USBCameraMonitor::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t USBCameraMonitor::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t USBCameraMonitor::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t USBCameraMonitor::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t USBCameraMonitor::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

bool USBCameraMonitor::isCFGChanged()
{
    if(m_img_height != m_nOldHeight || m_img_width != m_nOldWidth)
        return true;

    return false;
}

void onMouse(int nEvent, int x, int y, int nFlags, void *param)
{
    //printf("[onExecute] Mouse event [%d] is occured on (%d, %d)\n", 
    //                    nEvent, x, y);

    USBCameraMonitor *rtmParam = (USBCameraMonitor *)param;

    rtmParam->m_lMouseEv.data = nEvent;
    rtmParam->m_lMouseX.data  = x;
    rtmParam->m_lMouseY.data  = y;

    rtmParam->m_lMouseEvOut.write();
    rtmParam->m_lMouseXOut.write();
    rtmParam->m_lMouseYOut.write();
}

extern "C"
{
 
  void USBCameraMonitorInit(RTC::Manager* manager)
  {
    coil::Properties profile(usbcameramonitor_spec);
    manager->registerFactory(profile,
                             RTC::Create<USBCameraMonitor>,
                             RTC::Delete<USBCameraMonitor>);
  }
  
};
