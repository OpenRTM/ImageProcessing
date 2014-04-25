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

set OPENCV_RTC_ROOT=%~dp0
set BUILD_DIR=%OPENCV_RTC_ROOT%\work
set PYTHON_DIR=c:\python27;
set OpenCV_DIR=C:\distribution\OpenCV2.4.8-win32
set Baseclasses_DIR=C:\distribution\baseclasses

set RTM_ROOT=C:\distribution\OpenRTM-aist-rv2566
set COIL_ROOT=%RTM_ROOT%\coil
set OpenRTM_Dir=%RTM_ROOT%\cmake

set OMNI_ROOT=C:\distribution\omniORB-4.1.7-win32-vc10
set ARCH=x86
set VC_VERSION=10

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

@rem ------------------------------------------------------------
@rem Copying Config.cmake
@rem ------------------------------------------------------------
@rem %PYTHON_DIR%\python build\cmakeconfgen.py rtm_config.vsprops
@rem move OpenRTMConfig.cmake cmake

@rem ------------------------------------------------------------
@rem Copying Config.cmake
@rem ------------------------------------------------------------

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
) else (
	del /s /q work
	mkdir work
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
cd %BUILD_DIR%
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

@rem ------------------------------------------------------------
@rem Build (VC2008 x86)
@rem ------------------------------------------------------------
:VCBUILDx86
echo Visual Studio Dir: %VSINSTALLDIR%
echo LIB: %LIB%

vcbuild /M2 /rebuild components\DirectShowCam\BaseClasses\BaseClasses.sln "release|win32"
vcbuild /M2 /rebuild components\ImageCalibration\idl\InterfaceDataTypes_TGT.vcproj "release|win32"
vcbuild /M2 /build ImageProcessing_opencv.sln "release|win32"
goto END

@rem ------------------------------------------------------------
@rem Build (VC2010- x86)
@rem ------------------------------------------------------------
:MSBUILDx86
echo Visual Studio Dir: %VSINSTALLDIR%
echo LIB: %LIB%
set OPT=/M:4 /toolsversion:%VCTOOLSET% %PLATFORMTOOL% /p:platform=Win32
set SLN=ImageProcessing_opencv.sln
set LOG=/fileLogger /flp:logfile=debug.log /v:diag 

cd %BUILD_DIR%

if %VC_VERSION% == 10  (
	msbuild /m:2 /t:rebuild /p:configuration=release %OPT% components\DirectShowCam\BaseClasses\BaseClasses.sln
)

msbuild /t:build /p:configuration=release %OPT% %SLN%

goto END

@rem ============================================================
@rem start to cmake 64bit 
@rem ============================================================
:cmake_x86_64
@rem cd %OPENCV_RTC_ROOT%
cd %BUILD_DIR%
@rem cd %RTM_ROOT%
set VC_NAME="Visual Studio %VC_VERSION% Win64"
if %VC_VERSION% == 9  (
   echo 64bit compilation on Visual C++ 2008 is not supported. Aborting.
   goto END
   )else(
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
echo Visual Studio Dir: %VSINSTALLDIR%
echo LIB: %LIB%

@rem ------------------------------------------------------------
@rem Build (VC2008 x64)
@rem ------------------------------------------------------------
:VCBUILDx64
echo Visual Studio Dir: %VSINSTALLDIR%
echo LIB: %LIB%
set SLN=ImageProcessing_opencv.sln

vcbuild /M2 /build %SLN%
goto END

@rem ------------------------------------------------------------
@rem Build (VC2010- x64)
@rem ------------------------------------------------------------
:MSBUILDx64
echo Visual Studio Dir: %VSINSTALLDIR%
echo LIB: %LIB%
set OPT=/M:4 /toolsversion:%VCTOOLSET% %PLATFORMTOOL% /p:platform=x64
set LOG=/fileLogger /flp:logfile=debug.log /v:diag 
set SLN=ImageProcessing_opencv.sln

msbuild /t:clean /p:configuration=release   %OPT% %SLN%
msbuild /t:build /p:configuration=release %OPT% %SLN%

goto END

:END
cd %OPENCV_RTC_ROOT%
set PATH=%PATH_ORG%
