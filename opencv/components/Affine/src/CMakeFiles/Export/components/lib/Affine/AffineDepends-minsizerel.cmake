#----------------------------------------------------------------
# Generated CMake target import file for configuration "MinSizeRel".
#----------------------------------------------------------------

# Commands may need to know the format version.
SET(CMAKE_IMPORT_FILE_VERSION 1)

# Compute the installation prefix relative to this file.
GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)

# Import target "Affine" for configuration "MinSizeRel"
SET_PROPERTY(TARGET Affine APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
SET_TARGET_PROPERTIES(Affine PROPERTIES
  IMPORTED_IMPLIB_MINSIZEREL "${_IMPORT_PREFIX}/components/lib/Affine.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_MINSIZEREL "RTC110;coil110;omniORB415_rt;omniDynamic415_rt;omnithread34_rt;advapi32;ws2_32;mswsock;opencv_gpu231;opencv_contrib231;opencv_legacy231;opencv_objdetect231;opencv_calib3d231;opencv_features2d231;opencv_video231;opencv_highgui231;opencv_ml231;opencv_imgproc231;opencv_flann231;opencv_core231"
  IMPORTED_LOCATION_MINSIZEREL "${_IMPORT_PREFIX}/components/bin/Affine.dll"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS Affine )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_Affine "${_IMPORT_PREFIX}/components/lib/Affine.lib" "${_IMPORT_PREFIX}/components/bin/Affine.dll" )

# Import target "AffineComp" for configuration "MinSizeRel"
SET_PROPERTY(TARGET AffineComp APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
SET_TARGET_PROPERTIES(AffineComp PROPERTIES
  IMPORTED_LOCATION_MINSIZEREL "${_IMPORT_PREFIX}/components/bin/AffineComp.exe"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS AffineComp )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_AffineComp "${_IMPORT_PREFIX}/components/bin/AffineComp.exe" )

# Loop over all imported files and verify that they actually exist
FOREACH(target ${_IMPORT_CHECK_TARGETS} )
  FOREACH(file ${_IMPORT_CHECK_FILES_FOR_${target}} )
    IF(NOT EXISTS "${file}" )
      MESSAGE(FATAL_ERROR "The imported target \"${target}\" references the file
   \"${file}\"
but this file does not exist.  Possible reasons include:
* The file was deleted, renamed, or moved to another location.
* An install or uninstall procedure did not complete successfully.
* The installation package was faulty and contained
   \"${CMAKE_CURRENT_LIST_FILE}\"
but not all the files it references.
")
    ENDIF()
  ENDFOREACH()
  UNSET(_IMPORT_CHECK_FILES_FOR_${target})
ENDFOREACH()
UNSET(_IMPORT_CHECK_TARGETS)

# Cleanup temporary variables.
SET(_IMPORT_PREFIX)

# Commands beyond this point should not need to know the version.
SET(CMAKE_IMPORT_FILE_VERSION)
