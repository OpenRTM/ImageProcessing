//
// @file   ConfigurationInfo.h
// @brief  Configuration parameters.
//

#ifndef ConfigurationInfo_H
#define ConfigurationInfo_H

#include <iostream>

typedef struct {
  int    device_num;
  int    frame_width;
  int    frame_height;
  int    frame_rate;
  int    brightness;
  int    contrast;
  int    saturation;
  int    hue;
  int    gain;
  int    exposure_mode;
  int    exposure_absolute; 
}CONFIG_PRM;

#endif
