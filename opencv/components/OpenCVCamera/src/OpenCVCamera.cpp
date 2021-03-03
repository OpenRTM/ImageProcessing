// -*- C++ -*-
/*!
 * @file  OpenCVCamera.cpp
 * @brief USB Camera Acquire component
 * @date $Date$
 *
 * $Id$
 */

#include "OpenCVCamera.h"
#include <chrono>
#include <iostream>

// Module specification
// <rtc-template block="module_spec">
static const char* opencvcamera_spec[] =
  {
    "implementation_id", "OpenCVCamera",
    "type_name",         "OpenCVCamera",
    "description",       "USB Camera Acquire component",
    "version",           "1.2.3",
    "vendor",            "AIST",
    "category",          "opencv-rtcs",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.capture_mode", "camera",
    "conf.default.device_num", "0",
    "conf.default.video_file", "video.mp4",
    "conf.default.URL", " ",
    "conf.default.frame_width", "640",
    "conf.default.frame_height", "480",
    "conf.default.frame_rate", "30",
    "conf.default.brightness", "128",
    "conf.default.contrast", "32",
    "conf.default.saturation", "32",
    "conf.default.hue", "0",
    "conf.default.gain", "64",
    "conf.default.exposure_mode", "auto",
    "conf.default.exposure_absolute", "166",

    // Widget
    "conf.__widget__.capture_mode", "radio",
    "conf.__widget__.device_num", "text",
    "conf.__widget__.video_file", "text",
    "conf.__widget__.URL", "text",
    "conf.__widget__.frame_width", "text",
    "conf.__widget__.frame_height", "text",
    "conf.__widget__.frame_rate", "text",
    "conf.__widget__.brightness", "slider.1",
    "conf.__widget__.contrast", "slider.1",
    "conf.__widget__.saturation", "slider.1",
    "conf.__widget__.hue", "slider.1",
    "conf.__widget__.gain", "slider.1",
    "conf.__widget__.exposure_mode", "radio",
    "conf.__widget__.exposure_absolute", "slider.1",
    // Constraints
    "conf.__constraints__.capture_mode", "(camera,video,URL)",
    "conf.__constraints__.brightness", "0<=x<=255",
    "conf.__constraints__.contrast", "0<=x<=255",
    "conf.__constraints__.saturation", "0<=x<=255",
    "conf.__constraints__.hue", "-2000<=x<=2000",
    "conf.__constraints__.gain", "0<=x<=255",
    "conf.__constraints__.exposure_mode", "(manual,auto)",
    "conf.__constraints__.exposure_absolute", "3<=x<=2047",

    "conf.__type__.capture_mode", "string",
    "conf.__type__.device_num", "int",
    "conf.__type__.video_file", "string",
    "conf.__type__.URL", "string",
    "conf.__type__.frame_width", "int",
    "conf.__type__.frame_height", "int",
    "conf.__type__.frame_rate", "int",
    "conf.__type__.brightness", "int",
    "conf.__type__.contrast", "int",
    "conf.__type__.saturation", "int",
    "conf.__type__.hue", "int",
    "conf.__type__.gain", "int",
    "conf.__type__.exposure_mode", "string",
    "conf.__type__.exposure_absolute", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
OpenCVCamera::OpenCVCamera(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_outOut("out", m_out),

    // </rtc-template>
    m_currentBrightness(128),
    m_currentContrast(32),
    m_currentSaturation(32),
    m_currentHue(0),
    m_currentGain(64),    
    m_currentExposureMode(3),
    m_currentExposure(166),  
    dummy(0)
{
}

/*!
 * @brief destructor
 */
OpenCVCamera::~OpenCVCamera()
{
}



RTC::ReturnCode_t OpenCVCamera::onInitialize()
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

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("capture_mode", m_capture_mode, "camera");
  bindParameter("device_num", m_device_num, "0");
  bindParameter("video_file", m_video_file, "video.mp4");
  bindParameter("URL", m_URL, " ");
  bindParameter("frame_width", m_frame_width, "640");
  bindParameter("frame_height", m_frame_height, "480");
  bindParameter("frame_rate", m_frame_rate, "30");
  bindParameter("brightness", m_brightness, "128");
  bindParameter("contrast", m_contrast, "32");
  bindParameter("saturation", m_saturation, "32");
  bindParameter("hue", m_hue, "0");
  bindParameter("gain", m_gain, "64");
  bindParameter("exposure_mode", m_exposure_mode, "auto");
  bindParameter("exposure_absolute", m_exposure_absolute, "166");
  // </rtc-template>
 
  //m_current_device_id = -1;
  //m_current_video_file = "";
  

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t OpenCVCamera::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t OpenCVCamera::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t OpenCVCamera::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t OpenCVCamera::onActivated(RTC::UniqueId ec_id)
{
  using std::cout;
  using std::endl;
  m_CamCtl = new CaptureCameraControl(&m_capture);

  if (m_capture_mode == "camera")
  {
    copy_config_camera_property("all");
        
    if (m_CamCtl->open_camera(&m_config_prm))
    {
      cout << "Camera device opened." << endl;
      RTC_TRACE(("*** onActivated: Camera device opened. "));
    }
    else
    {
      return RTC::RTC_ERROR;
    }
    
    m_current_device_num = m_device_num;
    get_real_camera_property();
  }
  else if (m_capture_mode == "video")
  {
    if (m_CamCtl->open_video_file(m_video_file))
    {
      cout << "Video file opened." << endl;
      RTC_TRACE(("*** onActivated: Video file opened. "));
    }
    else
    {
      return RTC::RTC_ERROR;
    }
    m_current_video_file = m_video_file;

  }
  else
  {
    if (m_CamCtl->open_url(m_URL))
    {
      cout << "URL opened." << endl;
      RTC_TRACE(("*** onActivated: URL opened. "));
    }
    else
    {
      return RTC::RTC_ERROR;
    }
    m_current_URL = m_URL;
  }
  
  m_current_frame_width = m_frame_width;
  m_current_frame_height = m_frame_height;
  m_current_frame_rate = m_frame_rate;
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t OpenCVCamera::onDeactivated(RTC::UniqueId ec_id)
{
  delete m_CamCtl;
  m_capture.release();
  return RTC::RTC_OK;
}


RTC::ReturnCode_t OpenCVCamera::onExecute(RTC::UniqueId ec_id)
{
  using std::cout;
  using std::endl;
  static auto tmOld = std::chrono::system_clock::now();
  static int count = 0;
  const int DISPLAY_PERIOD_FRAME_NUM = 100;
  cv::Mat cam_frame;

  if (!check_config_parameters())
  {
    return RTC::RTC_ERROR;
  }
  try
  {
    if (!m_capture.read(cam_frame))
    {
      if (m_capture_mode == "camera" ||
          m_capture_mode == "URL")
      {
        cout << "Bad frame or no frame!!" << endl;
        RTC_TRACE(("*** onExecute: Bad frame or no frame!!"));
        return RTC::RTC_ERROR;
      }
      else
      {
        std::cout << "Video frame End." << std::endl;
        RTC_TRACE(("*** onExecute: Video frame End."));
        m_capture.set(CAP_PROP_POS_FRAMES, 0);
        std::cout << "To the first frame for loop playback." << std::endl;
        RTC_TRACE(("*** onExecute: To the first frame for loop playback."));
        return RTC::RTC_OK;
      }
    }
  }
  catch (...)
  {
    if (m_capture_mode == "camera" ||
          m_capture_mode == "URL")
    {
      std::cout << "Frame End (exception)" << std::endl;
      RTC_TRACE(("*** onExecute: Frame End (exception)"));
    }
    else
    {
      std::cout << "Video frame End (exception)" << std::endl;
      RTC_TRACE(("*** onExecute: Video frame End (exception)"));
    }
    return RTC::RTC_ERROR;
  }
  
  cv::Mat frame;
  frame = cam_frame;
  
  int len = frame.channels() * frame.size().width * frame.size().height;
  m_out.pixels.length(len);
  m_out.width = frame.size().width;
  m_out.height = frame.size().height;

  memcpy((void *)&(m_out.pixels[0]), frame.data, len);

  m_outOut.write();

  if (count++ > DISPLAY_PERIOD_FRAME_NUM)
  {
    auto tmNow = std::chrono::system_clock::now();
    double sec(std::chrono::duration<double>(tmNow - tmOld).count());

    if (sec > 1.0)
    {
      cout << (DISPLAY_PERIOD_FRAME_NUM / sec) << " [FPS]" << endl;
      tmOld = tmNow;
    }
    count = 0;
  }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t OpenCVCamera::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t OpenCVCamera::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t OpenCVCamera::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t OpenCVCamera::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t OpenCVCamera::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

bool OpenCVCamera::check_config_parameters()
{
  using std::cout;
  using std::endl;

  if (m_capture_mode == "camera")
  {
    if (m_current_device_num != m_device_num)
    {
      copy_config_camera_property("basic");
      
      if (m_CamCtl->open_camera(&m_config_prm))
      {
        cout << "Changed camera device opened." << endl;
        RTC_TRACE(("*** Changed camera device opened. "));
      }
      else
      {
        return false;
      }
      m_current_device_num = m_device_num;
      get_real_camera_property();
    }
  }
  else if (m_capture_mode == "video")
  {
    if (m_current_video_file != m_video_file)
    {   
      if (m_CamCtl->open_video_file(m_video_file))
      {
        cout << "Changed video filece opened." << endl;
        RTC_TRACE(("*** Changed video filece opened. "));
      }
      else
      {
        return false;
      }
      m_current_video_file = m_video_file;
    }
  }
  else
  {
    // capture_mode :URL
    if (m_current_URL != m_URL)
    {   
      if (m_CamCtl->open_url(m_URL))
      {
        cout << "Changed URL opened." << endl;
        RTC_TRACE(("*** Changed URL opened. "));
      }
      else
      {
        return false;
      }
      m_current_URL = m_URL;
    }
  }

  if (m_current_frame_width != m_frame_width &&
          m_current_frame_height != m_frame_height)
  {
    copy_config_camera_property("frame_width");
    copy_config_camera_property("frame_height");
    m_current_frame_width = m_frame_width;
    m_current_frame_height = m_frame_height;
    
    m_CamCtl->set_camera_property("frame_width", m_frame_width, CAP_PROP_FRAME_WIDTH);    
    m_CamCtl->set_camera_property("frame_height", m_frame_height, CAP_PROP_FRAME_HEIGHT);    
  }
  
  if (m_current_frame_rate != m_frame_rate)
  {
    copy_config_camera_property("frame_rate");
    m_current_frame_rate = m_frame_rate;
    m_CamCtl->set_camera_property("frame_rate", m_frame_rate, CAP_PROP_FPS);    
  }
  
  if (m_currentBrightness != m_brightness)
  {
    copy_config_camera_property("brightness");
    m_currentBrightness = m_brightness;
    m_CamCtl->check_and_set_camera_property("Brightness", 
                m_real_camera_Brightness, m_brightness, CAP_PROP_BRIGHTNESS);    
  }
  
  if (m_currentContrast != m_contrast)
  {
    copy_config_camera_property("contrast");
    m_currentContrast = m_contrast;
    m_CamCtl->check_and_set_camera_property("Contrast", 
                m_real_camera_Contrast, m_contrast, CAP_PROP_CONTRAST);    
  }
  
  if (m_currentSaturation != m_saturation)
  {
    copy_config_camera_property("saturation");
    m_currentSaturation = m_saturation;
    m_CamCtl->check_and_set_camera_property("Saturation", 
                m_real_camera_Saturation, m_saturation, CAP_PROP_SATURATION);
  }
  
  if (m_currentHue != m_hue)
  {
    copy_config_camera_property("hue");
    m_currentHue = m_hue;
    m_CamCtl->check_and_set_camera_property("Hue", 
                m_real_camera_Hue, m_hue, CAP_PROP_HUE);
  }
  
  if (m_currentGain != m_gain)
  {
    copy_config_camera_property("gain");
    m_currentGain = m_gain;
    m_CamCtl->check_and_set_camera_property("Gain", 
                m_real_camera_Gain, m_gain, CAP_PROP_GAIN);
  }
 
  int menu_number = get_exposure_mode_menu_number(m_exposure_mode);
  if (m_currentExposureMode != menu_number)
  {
    copy_config_camera_property("exposure_mode");
    m_currentExposureMode = menu_number;
    m_CamCtl->check_and_set_camera_property("ExposureMode", 
              m_real_camera_ExposureMode, menu_number, CAP_PROP_AUTO_EXPOSURE);
  }

  
  if (m_currentExposure != m_exposure_absolute)
  {
    copy_config_camera_property("exposure_absolute");
    m_currentExposure = m_exposure_absolute;
    m_CamCtl->check_and_set_camera_property("Exposure", 
                m_real_camera_Exposure, m_exposure_absolute, CAP_PROP_EXPOSURE);
  }
    
  return true;
}

void OpenCVCamera::copy_config_camera_property(std::string target)
{
  if (target == "all" || target == "basic")
  {
    m_config_prm.device_num = m_device_num;
    m_config_prm.frame_width = m_frame_width;
    m_config_prm.frame_height = m_frame_height;
    m_config_prm.frame_rate = m_frame_rate;
  }
  if (target == "all" || target == "brightness")
  {
    m_config_prm.brightness = m_brightness;
  }
  if (target == "all" || target == "contrast")
  {
    m_config_prm.contrast = m_contrast;
  }
  if (target == "all" || target == "saturation")
  {
    m_config_prm.saturation = m_saturation;
  }
  if (target == "all" || target == "hue")
  {
    m_config_prm.hue = m_hue;
  }
  if (target == "all" || target == "gain")
  {
    m_config_prm.gain = m_gain;
  }
  if (target == "all" || target == "exposure_mode")
  {
    m_config_prm.exposure_mode = get_exposure_mode_menu_number(m_exposure_mode);
  }
  if (target == "all" || target == "exposure_absolute")
  {
    m_config_prm.exposure_absolute = m_exposure_absolute;
  }  
}

void OpenCVCamera::get_real_camera_property()
{
  m_real_camera_Brightness = m_CamCtl->get_camera_property("Brightness", cv::CAP_PROP_BRIGHTNESS);
  m_real_camera_Contrast = m_CamCtl->get_camera_property("Contrast", cv::CAP_PROP_CONTRAST);
  m_real_camera_Saturation = m_CamCtl->get_camera_property("Saturation", cv::CAP_PROP_SATURATION);  
  m_real_camera_Hue = m_CamCtl->get_camera_property("Hue", cv::CAP_PROP_HUE);
  m_real_camera_Gain = m_CamCtl->get_camera_property("Gain", cv::CAP_PROP_GAIN);
  m_real_camera_ExposureMode = m_CamCtl->get_camera_property("ExposureMode", cv::CAP_PROP_AUTO_EXPOSURE);
  m_real_camera_Exposure = m_CamCtl->get_camera_property("Exposure", cv::CAP_PROP_EXPOSURE);
}

int OpenCVCamera::get_exposure_mode_menu_number(std::string config_val)
{
  int menu_number;
  
  if (m_exposure_mode == "auto")
  {
    menu_number = 3;
  }
  else
  {
    menu_number = 1;
  }
  return menu_number;
}

extern "C"
{

  void OpenCVCameraInit(RTC::Manager* manager)
  {
    coil::Properties profile(opencvcamera_spec);
    manager->registerFactory(profile,
                             RTC::Create<OpenCVCamera>,
                             RTC::Delete<OpenCVCamera>);
  }

};


