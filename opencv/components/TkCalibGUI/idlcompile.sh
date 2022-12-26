#!/bin/sh
IDL_PATH=`rtm2-config --rtm-idldir`
omniidl -bpython -I$IDL_PATH idl/CalibrationService.idl 