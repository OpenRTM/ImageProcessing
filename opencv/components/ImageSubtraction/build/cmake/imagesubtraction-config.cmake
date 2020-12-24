# ImageSubtraction CMake config file
#
# This file sets the following variables:
# ImageSubtraction_FOUND - Always TRUE.
# ImageSubtraction_INCLUDE_DIRS - Directories containing the ImageSubtraction include files.
# ImageSubtraction_IDL_DIRS - Directories containing the ImageSubtraction IDL files.
# ImageSubtraction_LIBRARIES - Libraries needed to use ImageSubtraction.
# ImageSubtraction_DEFINITIONS - Compiler flags for ImageSubtraction.
# ImageSubtraction_VERSION - The version of ImageSubtraction found.
# ImageSubtraction_VERSION_MAJOR - The major version of ImageSubtraction found.
# ImageSubtraction_VERSION_MINOR - The minor version of ImageSubtraction found.
# ImageSubtraction_VERSION_REVISION - The revision version of ImageSubtraction found.
# ImageSubtraction_VERSION_CANDIDATE - The candidate version of ImageSubtraction found.

message(STATUS "Found ImageSubtraction-1.2.3")
set(ImageSubtraction_FOUND TRUE)

find_package(<dependency> REQUIRED)

#set(ImageSubtraction_INCLUDE_DIRS
#    "/usr/local/include/imagesubtraction-1"
#    ${<dependency>_INCLUDE_DIRS}
#    )
#
#set(ImageSubtraction_IDL_DIRS
#    "/usr/local/include/imagesubtraction-1/idl")
set(ImageSubtraction_INCLUDE_DIRS
    "/usr/local/include/"
    ${<dependency>_INCLUDE_DIRS}
    )
set(ImageSubtraction_IDL_DIRS
    "/usr/local/include//idl")


if(WIN32)
    set(ImageSubtraction_LIBRARIES
        "/usr/local//libimagesubtraction.a"
        ${<dependency>_LIBRARIES}
        )
else(WIN32)
    set(ImageSubtraction_LIBRARIES
        "/usr/local//libimagesubtraction.so"
        ${<dependency>_LIBRARIES}
        )
endif(WIN32)

set(ImageSubtraction_DEFINITIONS ${<dependency>_DEFINITIONS})

set(ImageSubtraction_VERSION 1.2.3)
set(ImageSubtraction_VERSION_MAJOR 1)
set(ImageSubtraction_VERSION_MINOR 2)
set(ImageSubtraction_VERSION_REVISION 3)
set(ImageSubtraction_VERSION_CANDIDATE )

