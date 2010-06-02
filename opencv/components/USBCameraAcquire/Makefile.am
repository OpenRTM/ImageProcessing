## -*- Makefile -*-

AUTOMAKE_OPTIONS = 1.9

AM_CPPFLAGS=`rtm-config --cflags` `pkg-config --cflags opencv` -I$(top_srcdir)/rtm/idl
AM_LDFLAGS=-`rtm-config --libs` `pkg-config --libs opencv` -L$(top_srcdir)/rtm/idl -lRTCSkel
 
#------------------------------------------------------------
# Targets
#------------------------------------------------------------
# Targets directories
#exampledir = $(pkgdatadir)/examples
exampledir = /usr/share/OpenRTM-aist/Components/OpenCV
socompdir  = $(exampledir)/rtcs
sourcedir  = $(exampledir)/src/USBCameraAcquire

# Loadable RTCs
socomp_LTLIBRARIES = USBCameraAcquire.la

USBCameraAcquire_la_SOURCES = USBCameraAcquire.cpp USBCameraAcquire.h
USBCameraAcquire_la_LDFLAGS = -module

# Standalone RTCs
example_PROGRAMS = USBCameraAcquireComp
#example_DATA     = USBCameraAcquire.conf

USBCameraAcquireComp_SOURCES = USBCameraAcquireComp.cpp 
USBCameraAcquireComp_LDADD   = USBCameraAcquire.o

#$(top_builddir)/src/lib/coil/libcoil.la 
# Example sources
source_DATA = $(ALL_SRC) $(EXTRA_DIST)

#------------------------------------------------------------
# Extra distributed files
#------------------------------------------------------------
EXTRA_DIST =                  \
	rtc.conf              \
	Makefile.USBCameraAcquire \
	README.USBCameraAcquire

ALL_SRC =                    \
	USBCameraAcquireComp.cpp \
	USBCameraAcquire.cpp     \
	USBCameraAcquire.h

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

