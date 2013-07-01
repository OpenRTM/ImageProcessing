@rem
@rem This is a batch file to build OpenCV-rtcs. 
@rem
@rem Using:
@rem   + Please install the following tools beforehand. 
@rem     -- cmake
@rem     -- vc2010
@rem     -- OpenCV
@rem   + Please set path of cmake and vc2010.  
@rem

@rem @set OPENCV_ROOT=C:\Program Files (x86)\OpenRTM-aist\1.1\OpenCV2.3

@set WindowsSdkDir=
@for /F "tokens=1,2*" %%i in ('reg query "HKLM\SOFTWARE\Microsoft\Microsoft SDKs\Windows" /v "CurrentInstallFolder"') DO (
	if "%%i"=="CurrentInstallFolder" (
		set "WindowsSdkDir=%%k"
	)
)

@rem @set PATH="C:\Program Files (x86)\CMake 2.8\bin";"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\BIN";"c:\WINDOWS\Microsoft.NET\Framework\v4.0.30319";"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcpackages";%PATH%
@set PATH="c:\WINDOWS\Microsoft.NET\Framework\v4.0.30319";%PATH%

echo Generating SoLutio files / WorkSpace.
if not exist "work" (
	mkdir work
)
cd work
cmake .. -G "Visual Studio 10"

msbuild /m:2 /t:rebuild /p:configuration=release components\DirectShowCam\BaseClasses\BaseClasses.sln
msbuild /m:2 /t:build /p:configuration=release ImageProcessing_opencv.sln
cd ../
