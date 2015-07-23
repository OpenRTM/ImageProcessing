#!/bin/sh
#
# @brief Archiving OpenCVRTC (ImageProcessing) binaries in to a ZIP file
# @author Noriaki Ando <n-ando@aist.go.jp>
#         Copyright 2014 (C) All Right Reserved
#
# This is part of ip-build.bat OpenCVRTC build batch file
# for building ImageProcessing for Windows binaries.
#
#==============================

WORK_ROOT=`pwd`
echo "Current dir:"  ${WORK_ROOT}

NEW_DIR=opencv_rtcs
ZIP_FILE=${NEW_DIR}.zip

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

# end of script
#==============================