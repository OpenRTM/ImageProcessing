#!/bin/sh
#
# @brief Archiving OpenCVRTC (ImageProcessing) binaries in to a ZIP file
# @author Noriaki Ando <n-ando@aist.go.jp>
#         Copyright 2014 (C) All Right Reserved
#
# This is part of ip-build.bat OpenCVRTC build batch file
# for building ImageProcessing for Windows binaries.
#
# The following environment variables should be set.
# ex.
# export PYTHON_DIR=/cygdrive/c/Python27
# export VC_VERSION=9
# export ARCH=x86
#
#==============================

WORK_ROOT=`pwd`
echo "Current dir:"  ${WORK_ROOT}

if test "x$ARCH" = "xx86" ; then
	WIN_ARCH=win32
elif test "x$ARCH" = "xx86_64" ; then
	WIN_ARCH=win64
else
	echo "Unknown architecture: " $ARCH
	exit -1
fi

if test "x$PYTHON_DIR" = "x" ; then
	echo "PYTHON_DIR is not defined. "
	exit -1
else
	PY_VER=`echo $PYTHON_DIR | sed 's/.*[Pp][Yy][Tt][Hh][Oo][Nn]\([0-9][0-9]\).*/\1/'`
fi

NEW_DIR=$ZIP_DIR-${WIN_ARCH}-vc${VC_VERSION}
ZIP_FILE=$ZIP_DIR-${WIN_ARCH}-vc${VC_VERSION}-py${PY_VER}.zip

echo "NEW_DIR: " ${NEW_DIR}
echo "ZIP_FILE: " ${ZIP_FILE}

rm -rf ${NEW_DIR}
mkdir ${NEW_DIR}

cd ${NEW_DIR}
cp ${WORK_ROOT}/bin/*.conf .
cp ${WORK_ROOT}/bin/*.dll .
cp ${WORK_ROOT}/bin/*.exe .
cp ${WORK_ROOT}/bin/dist/TkCalibGUIComp.exe .
cp ${WORK_ROOT}/bin/dist/python27.dll .
cp -r ${WORK_ROOT}/bin/dist/lib .
cd ${WORK_ROOT}
chmod -R 755 ${NEW_DIR}

rm -rf ${ZIP_FILE}
zip -r ${ZIP_FILE} ${NEW_DIR}
rm -rf ${NEW_DIR}

# end of script
#==============================