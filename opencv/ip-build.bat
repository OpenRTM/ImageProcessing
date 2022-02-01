@echo off
@rem ============================================================
@rem OpenCVRTC (Image Processing) for Windows build batch
@rem
@rem @author Noriaki Ando <n-ando@aist.go.jp>
@rem                Copyright (C) 2022 n-ando All Rights Reserved
@rem
@rem In order to compile OpenCVRTC (Image Processing) on Windows, 
@rem this batch file can be used with the following prerequisite 
@rem tools and environments.
@rem
@rem 0. 64bit Windows 7 or later
@rem   If you want to compile both 32 bit and 64 bit binaries on one OS,
@rem   64 bit OS is required.  Because the omniidl which is built as
@rem   target architecture executable in midstream is required to
@rem   compile omniORB itself. OS have to execute both (32 bit /64 bit)
@rem   executable.
@rem
@rem 1. Cygwin
@rem   cygwin-lite distributed from omniORB is not recommended.
@rem   The following command should be installed on Cygwin
@rem   wget, make, zip, svn
@rem
@rem 2. Visual C++
@rem   Currently VC2008, 2010, 2012, 2013 are supported.
@rem   Different versions in the same machine are not recommended.
@rem
@rem Environment vairables required
@rem
@rem OMNI_ROOT: omniORB binary installation dir
@rem ARCH: x86 or x86_64
@rem VC_VERSION: 9(=VC2008), 10(=VC2010), 11(=VC2012), 12(=VC2013),....
@rem PYTHON_DIR: /cygdrive/c/Python27
@rem OpenCV_DIR:
@rem
@rem ============================================================

set PATH=%PATH%;C:\cygwin64\bin
set OPENCV_RTC_ROOT=%~dp0
set CMAKE_GENERATOR="Visual Studio 16 2019"

if not DEFINED ARCH       set ARCH=x86_64
if not DEFINED VC_VERSION set VC_VERSION=16
if not DEFINED PYTHON_DIR set PYTHON_DIR=c:\python39
if not DEFINED PYTHONPATH set PYTHONPATH=%PYTHON_DIR%\Lib
if not DEFINED OpenCV_DIR set OpenCV_DIR=C:\distribution\OpenCV4.4
if not DEFINED RTM_ROOT   set RTM_ROOT=C:\localRTM\2.0.0
if not DEFINED OMNI_ROOT  set OMNI_ROOT=%RTM_ROOT%\omniORB\4.2.5_vc16

set OpenRTM_DIR=%RTM_ROOT%\cmake


@rem ------------------------------------------------------------
@rem Printing env variables
echo Environment variables:
echo OMNI_ROOT  : %OMNI_ROOT%
echo ARCH       : %ARCH%
echo VC_VERSION : %VC_VERSION%
echo PYTHON_DIR : %PYTHON_DIR%

set PATH_ORG=%PATH%
set PATH=%PATH%;%OMNI_ROOT%\bin\x86_win32;%PYTHON_DIR%;

if %ARCH% == x86_64    set DLL_ARCH=_x64

@rem set cmake parameter
if exist build-release rmdir /s/q build-release
mkdir build-release
cd build-release
cmake -G %CMAKE_GENERATOR% -A x64 ..
cmake --build . --verbose --config Release

@rem ------------------------------------------------------------
@rem MAKE_ZIP Making ZIP archive
@rem ------------------------------------------------------------
:MAKE_ZIP
cd %OPENCV_RTC_ROOT%/bin
python setup.py py2exe
if not "%ERRORLEVEL%" == "0" (
	goto END
)

cd ../
set ZIP_DIR=ImageProcessing
c:\cygwin64\bin\bash ip_make_package.sh

goto END

:END
cd %OPENCV_RTC_ROOT%
set PATH=%PATH_ORG%
