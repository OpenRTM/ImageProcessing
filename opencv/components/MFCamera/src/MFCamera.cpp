// -*- C++ -*-
/*!
 * @file  MFCamera.cpp
 * @brief Media Foundation Camera component
 * @date $Date$
 *
 * $Id$
 */

#include "MFCamera.h"
#include <chrono>
#include <iostream>
using namespace std;

// Module specification
// <rtc-template block="module_spec">
static const char* mfcamera_spec[] =
  {
    "implementation_id", "MFCamera",
    "type_name",         "MFCamera",
    "description",       "USB Camera Acquire component",
    "version",           "1.2.0",
    "vendor",            "AIST",
    "category",          "example",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.device_id", "0",
    "conf.default.vfmt_id", "0",
    // Widget
    "conf.__widget__.device_id", "text",
    "conf.__widget__.vfmt_id", "text",
    // Constraints
    ""
  };
// </rtc-template>
  int device_id_old = 1000;
  int vfmt_id_old = 1000;

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
MFCamera::MFCamera(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_outOut("out", m_out),
    m_capture(NULL)
    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
MFCamera::~MFCamera()
{
  endMediaFoundation();
}



RTC::ReturnCode_t MFCamera::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("out", m_outOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  startMediaFoundation();

  m_capture = new MFCapture();
//  m_num_devices = m_capture->nDevs;

  std::cout<< "Num of device = "<< m_capture->nDevs << std::endl;
  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("device_id", m_device_id, "0");
  bindParameter("vfmt_id", m_vfmt_id, "0");
  // </rtc-template>  

  return RTC::RTC_OK;
}


RTC::ReturnCode_t MFCamera::onFinalize()
{
  endMediaFoundation();

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t MFCamera::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t MFCamera::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t MFCamera::onActivated(RTC::UniqueId ec_id)
{
  HRESULT hr;

  if(m_capture == NULL){
    m_capture = new MFCapture();
  }
  
  if(m_capture->nDevs == 0)
  {
    std::cout<<"Video capture device does not exist."<<std::endl;
    return RTC::RTC_ERROR;
  }

  std::cout<< "Video capture device : "<< m_capture->nDevs << std::endl;
  /// 
  hr = m_capture->ActivateDevice(m_device_id);
  if(FAILED(hr))
  {
    return RTC::RTC_ERROR; 
  }
  std::cout<< "Format:"<< m_vfmt_id <<  std::endl;
  hr = m_capture->SetDeviceFormat(m_vfmt_id);
  if(FAILED(hr))
  {
    return RTC::RTC_ERROR; 
  }
  m_capture->CreateImageBuffer();

  device_id_old = m_device_id;
  vfmt_id_old = m_vfmt_id;
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t MFCamera::onDeactivated(RTC::UniqueId ec_id)
{
  if(m_capture){
    delete m_capture;
    m_capture=NULL;
  }

  device_id_old = 1000;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t MFCamera::onExecute(RTC::UniqueId ec_id)
{
    static auto tm_pre = std::chrono::system_clock::now();
    static int count = 0;
    
    if(m_device_id != device_id_old){
#if 1 
	if (device_id_old != 1000)
	{
	  delete m_capture;
	}

	device_id_old = m_device_id;
	m_capture = new MFCapture();
	m_capture->ActivateDevice(m_device_id);
	m_capture->SetDeviceFormat(m_vfmt_id);
	m_capture->CreateImageBuffer();
#endif
    }

    if(m_vfmt_id != vfmt_id_old){
	vfmt_id_old = m_vfmt_id;
	delete m_capture;
	m_capture = new MFCapture();
	m_capture->ActivateDevice(m_device_id);
	m_capture->SetDeviceFormat(m_vfmt_id);
	m_capture->CreateImageBuffer();
	std::cout << "Format = "<< m_vfmt_id << std::endl;
    }

    if(m_capture->image_buf.empty())
    {
      std::cout << "No image buffer..." << std::endl;
      return RTC::RTC_ERROR; 
    }

	if (!m_capture->GetBufferData().empty())
    {
      m_out.pixels.length(m_capture->image_length);
      m_out.width  = m_capture->image_buf.size().width;
	  m_out.height = m_capture->image_buf.size().height;

      memcpy((void *)&(m_out.pixels[0]), m_capture->image_buf.data, m_capture->image_length);
      m_outOut.write();
    }
    
    if (count > 100)
    {
      count = 0;
      auto tm = std::chrono::system_clock::now();
      double sec(std::chrono::duration<double>(tm - tm_pre).count());

      if (sec > 1.0 && sec < 1000.0)
      {
        std::cout << 100/sec << " [FPS]" << std::endl;
      }

      tm_pre = tm;
    }
    ++count;

    return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t MFCamera::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t MFCamera::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t MFCamera::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t MFCamera::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t MFCamera::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void MFCameraInit(RTC::Manager* manager)
  {
    coil::Properties profile(mfcamera_spec);
    manager->registerFactory(profile,
                             RTC::Create<MFCamera>,
                             RTC::Delete<MFCamera>);
  }
  
};


