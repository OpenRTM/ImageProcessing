cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
if(POLICY CMP0048)
  cmake_policy(SET CMP0048 OLD)
endif()

project(BaseClasses)

include(FindDirectShow.cmake)

file(GLOB SRCS ${DIRECTSHOW_BASECLASS_DIR}/*.cpp)
file(GLOB HEADERS ${DIRECTSHOW_BASECLASS_DIR}/*.h)

include_directories(${DIRECTSHOW_BASECLASS_DIR})
include_directories(${WINDOWSSDK_INCLUDE_DIR})

add_library(strmbase STATIC ${SRCS})

