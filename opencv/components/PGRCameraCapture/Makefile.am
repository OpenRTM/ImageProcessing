## -*- Makefile -*-

AUTOMAKE_OPTIONS = 1.9

AM_CPPFLAGS=`rtm-config --cflags` `pkg-config --cflags opencv` -I$(top_srcdir)/rtm/idl -I/usr/include/flycapture
AM_LDFLAGS=-`rtm-config --libs` `pkg-config --libs opencv` -L$(top_srcdir)/rtm/idl -lRTCSkel -lflycapture${D}
 
#------------------------------------------------------------
# Targets
#------------------------------------------------------------
# Targets directories
#exampledir = $(pkgdatadir)/examples
exampledir = /usr/share/OpenRTM-aist/Components/OpenCV
socompdir  = $(exampledir)/rtcs
sourcedir  = $(exampledir)/src/PGRCameraCapture

# Loadable RTCs
socomp_LTLIBRARIES = PGRCameraCapture.la

PGRCameraCapture_la_SOURCES = PGRCameraCapture.cpp PGRCameraCapture.h
PGRCameraCapture_la_LDFLAGS = -module

# Standalone RTCs
example_PROGRAMS = PGRCameraCaptureComp
#example_DATA     = PGRCameraCapture.conf

PGRCameraCaptureComp_SOURCES = PGRCameraCaptureComp.cpp 
PGRCameraCaptureComp_LDADD   = PGRCameraCapture.o

#$(top_builddir)/src/lib/coil/libcoil.la 
# Example sources
source_DATA = $(ALL_SRC) $(EXTRA_DIST)

#------------------------------------------------------------
# Extra distributed files
#------------------------------------------------------------
EXTRA_DIST =                  \
	rtc.conf              \
	Makefile.PGRCameraCapture \
	README.PGRCameraCapture

ALL_SRC =                    \
	PGRCameraCaptureComp.cpp \
	PGRCameraCapture.cpp     \
	PGRCameraCapture.h

#------------------------------------------------------------
# File list for deb/ports packages
#------------------------------------------------------------
lst:
	echo $(ALL_SRC) > src.lst
	echo $(EXTRA_DIST) > other.lst

clean-local:
	rm -f \
	*.gch *.bak *.rpo *.sym lib*.*_pure_* *.lst \
	Makefile.old *core *~ *.so *.log

