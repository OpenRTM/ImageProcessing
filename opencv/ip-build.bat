@echo off
@rem ============================================================
@rem OpenCVRTC (Image Processing) for Windows build batch
@rem
@rem @author Noriaki Ando <n-ando@aist.go.jp>
@rem                Copyright (C) 2014 n-ando All Rights Reserved
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

if not DEFINED ARCH       set ARCH=x86
if not DEFINED VC_VERSION set VC_VERSION=12
if not DEFINED PYTHON_DIR set PYTHON_DIR=c:\python27
if not DEFINED OpenCV_DIR set OpenCV_DIR=C:\distribution\OpenCV-2.4.11
if not DEFINED RTM_ROOT   set RTM_ROOT=C:\distribution\OpenRTM-aist
if not DEFINED OMNI_ROOT  set OMNI_ROOT=%RTM_ROOT%\omniORB

set COIL_ROOT=%RTM_ROOT%\coil
set OpenRTM_DIR=%RTM_ROOT%\cmake

@rem ------------------------------------------------------------
@rem Printing env variables
echo Environment variables:
echo OMNI_ROOT  : %OMNI_ROOT%
echo ARCH       : %ARCH%
echo VC_VERSION : %VC_VERSION%
echo PYTHON_DIR : %PYTHON_DIR%

set PATH_ORG=%PATH%
set PATH=%OMNI_ROOT%\bin\x86_win32;%PATH%;%PYTHON_DIR%;

if %ARCH% == x86       set DLL_ARCH=
if %ARCH% == x86_64    set DLL_ARCH=_x64


if %VC_VERSION% LEQ 10 (
	@set WindowsSdkDir=
	@for /F "tokens=1,2*" %%i in ('reg query "HKLM\SOFTWARE\Microsoft\Microsoft SDKs\Windows" /v "CurrentInstallFolder"') DO (
		if "%%i"=="CurrentInstallFolder" (
			set "WindowsSdkDir=%%k"
		)
	)
)

@set PATH="c:\WINDOWS\Microsoft.NET\Framework\v4.0.30319";%PATH%

@rem ============================================================
@rem make work dir 
@rem ============================================================
echo work dir : work
if not exist "work" (
	mkdir work
)
cd work
if exist "CMakeCache.txt" (
    del CMakeCache.txt
    echo delete CMakeCache.txt
)

@rem ============================================================
@rem  switching to x86 or x86_64
@rem ============================================================
echo ARCH %ARCH%
if %ARCH% == x86       goto cmake_x86
if %ARCH% == x86_64    goto cmake_x86_64
goto END


@rem ============================================================
@rem start to cmake 32bit 
@rem ============================================================
:cmake_x86
set VC_NAME="Visual Studio %VC_VERSION%"
if %VC_VERSION% == 9  (
   cmake .. -G "Visual Studio 9 2008"
   goto x86
) else (
   cmake .. -G %VC_NAME%
   goto x86
   )
goto END

@rem ============================================================
@rem  Compiling 32bit binaries
@rem ============================================================
:x86
echo Compiling 32bit binaries
echo Setting up Visual C++ environment.
if %VC_VERSION% == 9  (
   @rem call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
   call C:\"Program Files (x86)"\"Microsoft Visual Studio 9.0"\VC\vcvarsall.bat x86
   goto VCBUILDx86
   )
if %VC_VERSION% == 10 (
   call C:\"Program Files (x86)"\"Microsoft Visual Studio 10.0"\VC\vcvarsall.bat x86
   @rem call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat%" x86
   set VCTOOLSET=4.0
   set PLATFORMTOOL=
   goto MSBUILDx86
   )
if %VC_VERSION% == 11 (
   @rem call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" x86
   call C:\"Program Files (x86)"\"Microsoft Visual Studio 11.0"\VC\vcvarsall.bat x86
   set VCTOOLSET=4.0
   set PLATFORMTOOL=/p:PlatformToolset=v110
   goto MSBUILDx86
   )
if %VC_VERSION% == 12 (
   @rem call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86
   call C:\"Program Files (x86)"\"Microsoft Visual Studio 12.0"\VC\vcvarsall.bat x86
   set VCTOOLSET=12.0
   set PLATFORMTOOL=/p:PlatformToolset=v120
   goto MSBUILDx86
   )
if %VC_VERSION% == 14 (
   call C:\"Program Files (x86)"\"Microsoft Visual Studio 14.0"\VC\vcvarsall.bat x86
   set VCTOOLSET=14.0
   set PLATFORMTOOL=/p:PlatformToolset=v140
   goto MSBUILDx86
   )

@rem ------------------------------------------------------------
@rem Build (VC2008 x86)
@rem ------------------------------------------------------------
:VCBUILDx86
echo Visual Studio Dir: %VSINSTALLDIR%
echo LIB: %LIB%

vcbuild /M2 /rebuild components\DirectShowCam\BaseClasses\BaseClasses.sln "release|win32"
vcbuild /M2 /rebuild components\ImageCalibration\idl\InterfaceDataTypes_TGT.vcproj "release|win32"
vcbuild /M2 /build ImageProcessing_opencv.sln "release|win32"
goto MAKE_ZIP

@rem ------------------------------------------------------------
@rem Build (VC2010- x86)
@rem ------------------------------------------------------------
:MSBUILDx86
echo Visual Studio Dir: %VSINSTALLDIR%
echo LIB: %LIB%
set OPT=/M:4 /toolsversion:%VCTOOLSET% %PLATFORMTOOL% /p:platform=Win32
set SLN=ImageProcessing_opencv.sln
set LOG=/fileLogger /flp:logfile=debug.log /v:diag 

if %VC_VERSION% == 10  (
	msbuild /t:rebuild /p:configuration=release %OPT% components\DirectShowCam\BaseClasses\BaseClasses.sln
)

msbuild /t:build /p:configuration=release %OPT% %SLN%

goto MAKE_ZIP


@rem ============================================================
@rem start to cmake 64bit 
@rem ============================================================
:cmake_x86_64
set VC_NAME="Visual Studio %VC_VERSION% Win64"
if %VC_VERSION% == 9  (
   echo 64bit compilation on Visual C++ 2008 is not supported. Aborting.
   goto END
   ) else (
   cmake .. -G %VC_NAME%
   goto x86_64
   )
goto END

@rem ============================================================
@rem  Compiling 64bit binaries
@rem ============================================================
:x86_64
echo Compiling 64bit binaries
@rem Setting up Visual C++ environment
if /i %VC_VERSION% == 9  (
   echo 64bit compilation on Visual C++ 2008 is not supported. Aborting.
   goto END
   )
if /i %VC_VERSION% == 10 (
   call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" amd64
   set VCTOOLSET=4.0
   set PLATFORMTOOL=
   goto MSBUILDx64
   )
if /i %VC_VERSION% == 11 (
   call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" amd64
   set VCTOOLSET=4.0
   set PLATFORMTOOL=/p:PlatformToolset=v110
   goto MSBUILDx64
   )
if /i %VC_VERSION% == 12 (
   call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64
   set VCTOOLSET=12.0
   set PLATFORMTOOL=/p:PlatformToolset=v120
   goto MSBUILDx64
   )
if /i %VC_VERSION% == 14 (
   call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
   set VCTOOLSET=14.0
   set PLATFORMTOOL=/p:PlatformToolset=v140
   goto MSBUILDx64
   )

@rem ------------------------------------------------------------
@rem Build (VC2010- x64)
@rem ------------------------------------------------------------
:MSBUILDx64
echo Visual Studio Dir: %VSINSTALLDIR%
echo LIB: %LIB%
set OPT=/M:4 /toolsversion:%VCTOOLSET% %PLATFORMTOOL% /p:platform=x64
set LOG=/fileLogger /flp:logfile=debug.log /v:diag 
set SLN=ImageProcessing_opencv.sln

if %VC_VERSION% == 10  (
	msbuild /t:rebuild /p:configuration=release %OPT% components\DirectShowCam\BaseClasses\BaseClasses.sln
)
msbuild /t:build /p:configuration=release %OPT% %SLN%

goto MAKE_ZIP


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
