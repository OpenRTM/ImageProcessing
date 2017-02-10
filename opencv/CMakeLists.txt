cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(ImageProcessing_opencv)
SUBDIRS(components)
set(SolutionDir ${PROJECT_SOURCE_DIR})

if(NOT WIN32)
  find_program(DPKG_CMD dpkg)
  if(DPKG_CMD)
    set(LINUX_PACKAGE_GENERATOR "DEB")
    execute_process(COMMAND "${DPKG_CMD}" --print-architecture
      OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "dpkg is found. arch = ${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
  endif(DPKG_CMD)

  find_program(RPM_CMD rpmbuild)
  if(RPM_CMD)
    set(LINUX_PACKAGE_GENERATOR "RPM")
    execute_process(COMMAND uname "-m"
      OUTPUT_VARIABLE CPACK_RPM_PACKAGE_ARCHITECTURE
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "rpmbuild is found. arch = ${CPACK_RPM_PACKAGE_ARCHITECTURE}")
  endif(RPM_CMD)
endif(NOT WIN32)

configure_file(${SolutionDir}/build_linux_package.sh.in
  ${PROJECT_BINARY_DIR}/build_linux_package.sh
  @ONLY
)
