//
// @file   CaptureCameraControl.cpp
// @brief  OpenCV camera control class.
//

#include "CaptureCameraControl.h"
#include <iostream>

CaptureCameraControl::CaptureCameraControl(VideoCapture *cap)
{
  m_cap = cap;
}

bool CaptureCameraControl::open_camera(CONFIG_PRM *prm)
{
  using std::cout;
  using std::endl;

#ifdef __linux__
  m_cap->open(prm->device_num, CAP_V4L);
#else
  m_cap->open(prm->device_num, CAP_ANY);
#endif
  cout << "Camera device :" << prm->device_num << endl;
  RTC_TRACE(("*** Camera device :%d", prm->device_num));

  m_cap->set(CAP_PROP_FPS, prm->frame_rate);
  cout << "set frame rate :" << prm->frame_rate << endl;
  RTC_TRACE(("*** set frame rate :%d", prm->frame_rate));

  m_cap->set(CAP_PROP_FRAME_WIDTH, prm->frame_width);
  m_cap->set(CAP_PROP_FRAME_HEIGHT, prm->frame_height);
  cout << "set frame_width :" << prm->frame_width << endl;
  cout << "set frame_height :" << prm->frame_height << endl;
  RTC_TRACE(("*** set frame width :%d", prm->frame_width));
  RTC_TRACE(("*** set frame height :%d", prm->frame_height));

  if (!m_cap->isOpened())
  {
    cout << "No Camera Device" << endl;
    RTC_TRACE(("*** No Camera Device"));
    return false;
  }
  return true;
}

bool CaptureCameraControl::open_video_file(std::string video_file)
{
  using std::cout;
  using std::endl;
  
  cout << "Video file :" << video_file << endl;
  RTC_TRACE(("*** Video file :%s", video_file.c_str()));
  m_cap->open(video_file);
  
  if (!m_cap->isOpened())
  {
    cout << "No Video File." << endl;
    RTC_TRACE(("*** No Video File."));
    return false;
  }
  return true;
}

bool CaptureCameraControl::open_url(std::string url)
{
  using std::cout;
  using std::endl;
  
  cout << "URL :" << url << endl;
  RTC_TRACE(("*** URL :%s", url.c_str()));
  m_cap->open(url);
  
  if (!m_cap->isOpened())
  {
    cout << "No URL." << endl;
    RTC_TRACE(("*** No URL."));
    return false;
  }
  return true;
}

int CaptureCameraControl::get_camera_property(
        std::string target,
        VideoCaptureProperties label)
{
  using std::cout;
  using std::endl;
    
  double value = m_cap->get(label);
  if (value > 0)
  {
    cout << "get " << target << " :" << value << endl;
    RTC_TRACE(("*** get camera %s :%f", target.c_str(), value));
  }
  else
  {
    value = 0;
    cout << target << " property is not supported." << endl;
    RTC_TRACE(("*** %s property is not supported.", target.c_str()));
  }
  return (int)value;
}

void CaptureCameraControl::set_camera_property(
        std::string target,
        int config_value,
        VideoCaptureProperties label)
{
  using std::cout;
  using std::endl;

  m_cap->set(label, (double)config_value);
  cout << "set " << target << " :" << config_value << endl;
  RTC_TRACE(("*** set %s :%d", target.c_str(), config_value));
}

void CaptureCameraControl::check_and_set_camera_property(
        std::string target,
        int real_camera_value,
        int config_value,
        VideoCaptureProperties label)
{
  using std::cout;
  using std::endl;
  
  if(real_camera_value != 0)
  {
    if (!m_cap->set(label, config_value))
    {      
      cout << "set " << target << " is not supported." << endl;
      RTC_TRACE(("*** set %s is not supported.", target.c_str()));
    }
    else
    {
      cout << "set " << target << " :" << config_value << endl;
      RTC_TRACE(("*** set %s :%d", target.c_str(), config_value));
    }
    cout << "check : get camera " << target << " : " << m_cap->get(label) << endl;
    RTC_TRACE(("*** check : get camera %s :%f", target.c_str(), m_cap->get(label)));
  }
}

