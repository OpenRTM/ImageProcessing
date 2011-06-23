

set(OPENCV_FOUND FALSE)


if(UNIX)
    include(FindPkgConfig)

    # OpenCV
    pkg_check_modules(OPENCV REQUIRED opencv)
    if(NOT OPENCV_DIR)
        if(OPENCV_FOUND)
            set(OPENCV_DIR ${OPENCV_PREFIX})
        endif()
        set(OPENCV_DIR ${OPENCV_DIR} CACHE PATH "OpenCV root directory")
    endif()

    set(OPENCV_CFLAGS ${OPENCV_CFLAGS_OTHER})
    set(OPENCV_LDFLAGS ${OPENCV_LDFLAGS_OTHER})

endif(UNIX)

if(WIN32)
    # OpenCV
    if(NOT OPENCV_DIR)
        if(NOT $ENV{OPENCV_ROOT} STREQUAL "")
            set(OPENCV_DIR $ENV{OPENCV_ROOT})
            set(OPENCV_FOUND TRUE)
        endif()
        set(OPENCV_DIR ${OPENCV_DIR} CACHE PATH "OpenCV root directory")
        if(NOT OPENCV_FOUND)
            message(FATAL_ERROR "OpenCV not found.")
        endif()
    endif()

    include(${OPENCV_DIR}/OpenCVConfig.cmake)
    set(OPENCV_VERSION ${OpenCV_VERSION})
    set(OPENCV_VERSION_MAJOR ${OpenCV_VERSION_MAJOR})
    set(OPENCV_VERSION_MINOR ${OpenCV_VERSION_MINOR})
    set(OPENCV_VERSION_PATCH ${OpenCV_VERSION_PATCH})

    set(OPENCV_INCLUDE_DIRS ${OPENCV_DIR}/include)
    set(OPENCV_LIBRARY_DIRS ${OPENCV_DIR}/lib)

    set(_ver "${OPENCV_VERSION_MAJOR}${OPENCV_VERSION_MINOR}${OPENCV_VERSION_PATCH}")

    foreach(_lib ${OPENCV_LIB_COMPONENTS})
        list(APPEND OPENCV_LIBRARIES optimized ${_lib}${_ver} debug ${_lib}${_ver}d)
    endforeach()

endif(WIN32)

message(STATUS "FindOpenCV setup done.")

message(STATUS "  OPENCV_DIR=" ${OPENCV_DIR})
message(STATUS "  OPENCV_VERSION=" ${OPENCV_VERSION})
message(STATUS "  OPENCV_CFLAGS=" ${OPENCV_CFLAGS})
message(STATUS "  OPENCV_INCLUDE_DIRS=" ${OPENCV_INCLUDE_DIRS})
message(STATUS "  OPENCV_LDFLAGS=" ${OPENCV_LDFLAGS})
message(STATUS "  OPENCV_LIBRARY_DIRS=" ${OPENCV_LIBRARY_DIRS})
message(STATUS "  OPENCV_LIBRARIES=" ${OPENCV_LIBRARIES})

