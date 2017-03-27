
from distutils.core import setup
import py2exe

option = {
  "packages"	 : ['PIL'],
  "compressed"	 : 1,
  "optimize"	 : 2,
  }

setup(
  options = { "py2exe"	:{
    "includes": ["OpenRTM_aist"],
    "dll_excludes": ["numpy-atlas.dll"] }
  },
  console = [
   {"script"	:	"TkCalibGUIComp.py" }
  ],
  zipfile = 'lib\libs.zip'
)
			 
