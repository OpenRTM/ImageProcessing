# Find DirectShow on Windows.
#
# Once loaded this will define
#   DIRECTSHOW_FOUND							- system has DirectShow
#   WINDOWSSDK_INCLUDE_DIR			  - include directory for Windows SDK
#   WINDOWSSDK_LIBRARY_DIRS				- library directries for Windows SDK
#   DIRECTSHOW_BASECLASS_DIR			- directory for DirectShow baseclasses
#   WINDOWSSDK_STRMIIDS_LIB				- strmiids.lib you need to link to

set(DIRECTSHOW_FOUND "NO")

if(MSVC)

	get_filename_component(WINDOWSSDK_ROOT
			"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows;CurrentInstallFolder]" 
	    ABSOLUTE CACHE
	)
#	message("WINDOWSSDK_ROOT : ${WINDOWSSDK_ROOT}")

  find_path(WINDOWSSDK_INCLUDE_DIR 
			NAMES DShow.h
			PATHS
				"${WINDOWSSDK_ROOT}/Include"
			NO_DEFAULT_PATH
  )
#	message("${WINDOWSSDK_INCLUDE_DIR}")

  set(WINDOWSSDK_LIBRARY_DIRS "${WINDOWSSDK_ROOT}/Lib")
	message(STATUS "DirectShow Library: ${WINDOWSSDK_LIBRARY_DIRS}")

  find_path(DIRECTSHOW_BASECLASS_DIR 
			NAMES streams.h
			PATHS
				"${WINDOWSSDK_ROOT}"
			PATH_SUFFIXES
				"Samples/multimedia/directshow/baseclasses"
			NO_DEFAULT_PATH
  )
#	message(STATUS "${DIRECTSHOW_BASECLASS_DIR}")

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
		set(DIRECTSHOW_FOUND "YES")
	endif(DIRECTSHOW_BASECLASS_DIR)

endif(MSVC)