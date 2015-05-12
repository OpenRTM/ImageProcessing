#!/bin/sh

grep -l 'Translate' CPackConfig.cmake | xargs sed -i.bak -e 's/Translate/ImageProcessing/g'
cpack -C CPackConfig.cmake
