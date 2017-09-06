cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(ImageProcessing_opencv)
SUBDIRS(components)
set(SolutionDir ${PROJECT_SOURCE_DIR})

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
if(PKGMGR)
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

configure_file(${SolutionDir}/build_linux_package.sh.in
  ${PROJECT_BINARY_DIR}/build_linux_package.sh
  @ONLY
)
