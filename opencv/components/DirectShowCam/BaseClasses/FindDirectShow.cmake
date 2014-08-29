# Find DirectShow on Windows.
#
# Once loaded this will define
#   DIRECTSHOW_FOUND              - system has DirectShow
#   WINDOWSSDK_INCLUDE_DIR        - include directory for Windows SDK
#   WINDOWSSDK_LIBRARY_DIRS       - library directries for Windows SDK
#   DIRECTSHOW_BASECLASS_DIR      - directory for DirectShow baseclasses
#   WINDOWSSDK_STRMIIDS_LIB       - strmiids.lib you need to link to

if(MSVC)

  foreach(winsdkver v7.1 v7.0A v7.0)
    get_filename_component(sdkdir
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\${winsdkver};InstallationFolder]"
      ABSOLUTE)
    if(EXISTS "${sdkdir}")
      list(APPEND win_sdk_dir "${sdkdir}")
    endif()
  endforeach()

  foreach(sdkdir ${win_sdk_dir})
    if(EXISTS "${sdkdir}/Samples/Multimedia/DirectShow/BaseClasses/streams.h"
        AND EXISTS "${sdkdir}/Include/DShow.h")
      list(APPEND WINDOWSSDK_ROOT "${sdkdir}")
      list(APPEND WINDOWSSDK_INCLUDE_DIR "${sdkdir}/Include")
    endif()
  endforeach()

  message(STATUS "WindowsSDK Include : ${WINDOWSSDK_INCLUDE_DIR}")

  set(WINDOWSSDK_LIBRARY_DIRS "${WINDOWSSDK_ROOT}/Lib")

  set(DIRECTSHOW_BASECLASS_DIR "${WINDOWSSDK_ROOT}/Samples/multimedia/directshow/baseclasses")
  message(STATUS "DirectShow baseclasses : ${DIRECTSHOW_BASECLASS_DIR}")

  # if DirectShow include dir found, then find DirectShow libraries
  if(WINDOWSSDK_INCLUDE_DIR)
    if(CMAKE_CL_64)
      find_library(WINDOWSSDK_STRMIIDS_LIB
        NAMES strmiids
        PATHS
          "${WINDOWSSDK_ROOT}/Lib/x64"
        NO_DEFAULT_PATH
      )
    else(CMAKE_CL_64)
      find_library(WINDOWSSDK_STRMIIDS_LIB
        NAMES strmiids
        PATHS
          "${WINDOWSSDK_ROOT}/Lib"
          "${WINDOWSSDK_ROOT}/Lib/x86"
        NO_DEFAULT_PATH
      )
    endif(CMAKE_CL_64)
  endif(WINDOWSSDK_INCLUDE_DIR)

  if(DIRECTSHOW_BASECLASS_DIR)
    set(DIRECTSHOW_FOUND TRUE)
  else(DIRECTSHOW_BASECLASS_DIR)
    message(STATUS "Warning: cannot find DirectShow baseclasses")
  endif(DIRECTSHOW_BASECLASS_DIR)

endif(MSVC)
