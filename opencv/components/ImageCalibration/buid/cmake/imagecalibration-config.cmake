# ImageCalibration CMake config file
#
# This file sets the following variables:
# ImageCalibration_FOUND - Always TRUE.
# ImageCalibration_INCLUDE_DIRS - Directories containing the ImageCalibration include files.
# ImageCalibration_IDL_DIRS - Directories containing the ImageCalibration IDL files.
# ImageCalibration_LIBRARIES - Libraries needed to use ImageCalibration.
# ImageCalibration_DEFINITIONS - Compiler flags for ImageCalibration.
# ImageCalibration_VERSION - The version of ImageCalibration found.
# ImageCalibration_VERSION_MAJOR - The major version of ImageCalibration found.
# ImageCalibration_VERSION_MINOR - The minor version of ImageCalibration found.
# ImageCalibration_VERSION_REVISION - The revision version of ImageCalibration found.
# ImageCalibration_VERSION_CANDIDATE - The candidate version of ImageCalibration found.

message(STATUS "Found ImageCalibration-1.2.3")
set(ImageCalibration_FOUND TRUE)

find_package(<dependency> REQUIRED)

#set(ImageCalibration_INCLUDE_DIRS
#    "/usr/local/include/imagecalibration-1"
#    ${<dependency>_INCLUDE_DIRS}
#    )
#
#set(ImageCalibration_IDL_DIRS
#    "/usr/local/include/imagecalibration-1/idl")
set(ImageCalibration_INCLUDE_DIRS
    "/usr/local/include/"
    ${<dependency>_INCLUDE_DIRS}
    )
set(ImageCalibration_IDL_DIRS
    "/usr/local/include//idl")


if(WIN32)
    set(ImageCalibration_LIBRARIES
        "/usr/local//libimagecalibration.a"
        ${<dependency>_LIBRARIES}
        )
else(WIN32)
    set(ImageCalibration_LIBRARIES
        "/usr/local//libimagecalibration.so"
        ${<dependency>_LIBRARIES}
        )
endif(WIN32)

set(ImageCalibration_DEFINITIONS ${<dependency>_DEFINITIONS})

set(ImageCalibration_VERSION 1.2.3)
set(ImageCalibration_VERSION_MAJOR 1)
set(ImageCalibration_VERSION_MINOR 2)
set(ImageCalibration_VERSION_REVISION 3)
set(ImageCalibration_VERSION_CANDIDATE )

