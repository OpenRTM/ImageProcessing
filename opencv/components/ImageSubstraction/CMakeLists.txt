cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
cmake_policy(SET CMP0002 OLD)

project(ImageSubstraction)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
include("${PROJECT_SOURCE_DIR}/cmake/utils.cmake")
set(PROJECT_VERSION 1.1.0 CACHE STRING "ImageSubstraction version")
DISSECT_VERSION()
set(PROJECT_DESCRIPTION "Image substraction conponent")
set(PROJECT_VENDOR "AIST")
set(PROJECT_AUTHOR "AIST")
set(PROJECT_AUTHOR_SHORT "AIST")

function(get_dist ARG0)
  if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(${ARG0} ${CMAKE_SYSTEM_NAME} PARENT_SCOPE)
    return()
  endif()
  foreach(dist Debian Ubuntu RedHat Fedora CentOS)
    execute_process(
      COMMAND grep ${dist} -s /etc/issue /etc/os-release /etc/redhat-release /etc/system-release
      OUTPUT_VARIABLE dist_name
      )
    if(${dist_name} MATCHES ${dist})
      set(${ARG0} ${dist} PARENT_SCOPE)
      return()
    endif()
  endforeach()
endfunction(get_dist)

function(get_pkgmgr ARG0)
  get_dist(DIST_NAME)
  if(${DIST_NAME} MATCHES "Debian" OR
     ${DIST_NAME} MATCHES "Ubuntu")
     set(${ARG0} "DEB" PARENT_SCOPE)
     return()
  endif()
  if(${DIST_NAME} MATCHES "RedHat" OR
     ${DIST_NAME} MATCHES "Fedora" OR
     ${DIST_NAME} MATCHES "CentOS")
     set(${ARG0} "RPM" PARENT_SCOPE)
     return()
  endif()
endfunction(get_pkgmgr)

get_dist(DIST_NAME)
MESSAGE(STATUS "Distribution is ${DIST_NAME}")

get_pkgmgr(PKGMGR)
if(PKGMGR AND NOT LINUX_PACKAGE_GENERATOR)
  set(LINUX_PACKAGE_GENERATOR ${PKGMGR})
  if(${PKGMGR} MATCHES "DEB")    
    execute_process(COMMAND dpkg --print-architecture
      OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "Package manager is ${PKGMGR}. Arch is ${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}.")   
  endif()
  if(${PKGMGR} MATCHES "RPM")
    execute_process(COMMAND uname "-m"
      OUTPUT_VARIABLE CPACK_RPM_PACKAGE_ARCHITECTURE
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "Package manager is ${PKGMGR}. Arch is ${CPACK_RPM_PACKAGE_ARCHITECTURE}.")   
  endif()    
endif()

set(PROJECT_MAINTAINER "Noriaki Ando <n-ando@aist.go.jp>")
set(PROJECT_TYPE "c++/opencv-rtcs")

# Add an "uninstall" target
CONFIGURE_FILE ("${PROJECT_SOURCE_DIR}/cmake/uninstall_target.cmake.in"
    "${PROJECT_BINARY_DIR}/uninstall_target.cmake" IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET (${PROJECT_NAME}_uninstall "${CMAKE_COMMAND}" -P
    "${PROJECT_BINARY_DIR}/uninstall_target.cmake")

#option(BUILD_EXAMPLES "Build and install examples" OFF)
option(BUILD_DOCUMENTATION "Build the documentation" OFF)
#option(BUILD_TESTS "Build the tests" OFF)
#option(BUILD_TOOLS "Build the tools" OFF)
option(BUILD_IDL "Build and install idl" ON)
option(BUILD_SOURCES "Build and install sources" OFF)

option(STATIC_LIBS "Build static libraries" OFF)
if(STATIC_LIBS)
    set(LIB_TYPE STATIC)
else(STATIC_LIBS)
    set(LIB_TYPE SHARED)
endif(STATIC_LIBS)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   # Mac OS X specific code
   SET(CMAKE_CXX_COMPILER "g++")
endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


# Set up installation directories
if(WIN32)
#  set(BIN_INSTALL_DIR "components/bin")
#  set(LIB_INSTALL_DIR "components/lib")
  set(INSTALL_PREFIX "components/${PROJECT_TYPE}")
else(WIN32)
  set(OPENRTM_SHARE_PREFIX "share/openrtm-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
  set(INSTALL_PREFIX "${OPENRTM_SHARE_PREFIX}/components/${PROJECT_TYPE}")
endif(WIN32)

#set(INC_INSTALL_DIR
#    "components/include/${PROJECT_NAME_LOWER}-${PROJECT_VERSION_MAJOR}")
#set(SHARE_INSTALL_DIR
#    "components/share/${PROJECT_NAME_LOWER}-${PROJECT_VERSION_MAJOR}")

# Get necessary dependency information
find_package(OpenRTM)
if(${OpenRTM_FOUND})
  MESSAGE(STATUS "OpenRTM configuration Found")
else(${OpenRTM_FOUND})
  message(STATUS "Use cmake/Modules/FindOpenRTM.cmake in the project")
  list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
  find_package(OpenRTM REQUIRED)
endif(${OpenRTM_FOUND})

# Universal settings
#enable_testing()

# Subdirectories
add_subdirectory(cmake)
if(BUILD_DOCUMENTATION)
    add_subdirectory(doc)
endif(BUILD_DOCUMENTATION)

#if(BUILD_EXAMPLES)
#    add_subdirectory(examples)
#endif(BUILD_EXAMPLES)

if(BUILD_IDL)
    add_subdirectory(idl)
endif(BUILD_IDL)

add_subdirectory(include)
MAP_ADD_STR(headers  "include/" comp_hdrs)
add_subdirectory(src)

#if(BUILD_TESTS)
#    add_subdirectory(test)
#endif(BUILD_TESTS)

#if(BUILD_TOOLS)
#    add_subdirectory(tools)
#endif(BUILD_TOOLS)

if(BUILD_SOURCES)
    add_subdirectory(include)
    add_subdirectory(src)
endif(BUILD_SOURCES)

# Package creation
# By default, do not warn when built on machines using only VS Express:
IF(NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)
SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS ON)
ENDIF()
include(InstallRequiredSystemLibraries)
set(PROJECT_EXECUTABLES ${PROJECT_NAME_LOWER}Comp
    "${PROJECT_NAME_LOWER}Comp")

set(cpack_options "${PROJECT_BINARY_DIR}/cpack_options.cmake")

configure_file("${PROJECT_SOURCE_DIR}/cmake/cpack_options.cmake.in"
    ${cpack_options} @ONLY)

set(CPACK_PROJECT_CONFIG_FILE ${cpack_options})
include(${CPACK_PROJECT_CONFIG_FILE})
include(CPack)


