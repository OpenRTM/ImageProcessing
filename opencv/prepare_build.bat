@echo off
@rem ============================================================
@rem Preparing ImageProcessing build environment on Windows
@rem
@rem @author Nobu Kawauchi
@rem                Copyright (C) 2018 AIST All Rights Reserved
@rem 
@rem Rewrite OpenRTMConfig.cmake as follows.
@rem
@rem set(OMNIORB_DIR "${OPENRTM_DIR}/omniORB/${OMNIORB_VERSION}_${RTM_VC_VER}")
@rem 　　　↓
@rem set(OMNIORB_DIR "${OPENRTM_DIR}/omniORB/") 
@rem
@rem set(OPENRTM_LIBRARY_DIRS ${OPENRTM_BIN_PATH};${OMNIORB_DIR}/lib/x86_win32)
@rem 　　　↓
@rem set(OPENRTM_LIBRARY_DIRS ${OPENRTM_BIN_PATH};${OMNIORB_DIR}/lib/x86_win32;${OPENRTM_DIR}/OpenSSL/build/lib) 
@rem ============================================================

if not exist %RTM_ROOT%\cmake\OpenRTMConfig.cmake.org (
  move %RTM_ROOT%\cmake\OpenRTMConfig.cmake %RTM_ROOT%\cmake\OpenRTMConfig.cmake.org
)
if exist OpenRTMConfig.cmake del OpenRTMConfig.cmake

setlocal enabledelayedexpansion
for /f "delims=" %%a in (%RTM_ROOT%\cmake\OpenRTMConfig.cmake.org) do (
  set line=%%a
  set line=!line:/${OMNIORB_VERSION}_${RTM_VC_VER}=/!
  set line=!line:;${OMNIORB_DIR}/lib/x86_win32=;${OMNIORB_DIR}/lib/x86_win32;${OPENRTM_DIR}/OpenSSL/build/lib!
  echo !line! >> %RTM_ROOT%\cmake\OpenRTMConfig.cmake
)
endlocal
