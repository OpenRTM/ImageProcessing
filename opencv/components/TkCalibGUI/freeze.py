
from py2exe import freeze

option = {
  "packages"	 : ['PIL'],
  "compressed"	 : 1,
  "optimize"	 : 2,
  }

freeze(
  options = { 
    "includes": ["OpenRTM_aist"],
    "dll_excludes": ["numpy-atlas.dll"] 
  },
  console = [
   {"script"	:	"TkCalibGUIComp.py" }
  ],
  zipfile = 'lib\libs.zip'
)
			 
