#!/bin/sh
#
# @brief Archiving omniORB Windows binaries in to a ZIP file
# @author Noriaki Ando <n-ando@aist.go.jp>
#         Copyright 2014 (C) All Right Reserved
#
# This is part of build.bat omniORB build batch file
# for building omniORB for Windows binaries.
#
# The following environment variables should be set.
# ex.
# export OMNIORB_DIR=omniORB-4.2.0
# export PYTHON_DIR=/cygdrive/c/Python33
# export VC_VERSION=9
# export ARCH=x86
#
#==============================

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
echo "OMNIORB_DIR: " ${OMNIORB_DIR}
echo "ZIP_FILE: " ${ZIP_FILE}

rm -rf ${NEW_DIR}
cp -r bin ${NEW_DIR}

rm -rf ${ZIP_FILE}
zip -r ${ZIP_FILE} ${NEW_DIR}
rm -rf ${NEW_DIR}

# end of script
#==============================