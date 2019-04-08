#!/usr/bin/env python
# -*- coding: utf-8 -*-
# -*- Python -*-

"""
 @file TkCalibGUIComp.py
 @brief Image Calibration GUI
 @date $Date$

"""
def check_openrtmpy():
    try:
        import OpenRTM_aist
        print("OpenRTM python is installed.")
    except:
        import tkMessageBox
        import Tkinter
        msg = u'OpenRTM-aist Python is not installed. '
        msg += u'Please download it from \n'
        msg += u'http://openrtm.org/openrtm/ja/content/tkcalibgui'
        Tkinter.Tk().withdraw()
        tkMessageBox.showerror(title = "OpenRTM not found",
                  message = msg)

def check_ttk():
    try:
        import ttk
        print("Ttk is installed.")
    except:
        import tkMessageBox
        import Tkinter
        msg = u'Ttk is not installed. '
        msg += u'Please download it from \n'
        msg += u'http://openrtm.org/openrtm/ja/content/tkcalibgui'
        Tkinter.Tk().withdraw()
        tkMessageBox.showerror(title = "Ttk not found",
                  message = msg)

def check_pil():
    try:
        from PIL import Image, ImageTk
        print("Python Imaging Library (PIL) is installed.")
    except:
        import tkMessageBox
        import Tkinter
        msg = u'Python Imaging Library (PIL) is not installed. '
        msg += u'Please download it from \n'
        msg += u'http://openrtm.org/openrtm/ja/content/tkcalibgui'
        Tkinter.Tk().withdraw()
        tkMessageBox.showerror(title = "PIL not found",
                  message = msg)

def check_numpy():
    try:
        import numpy
        print("Numpy is installed.")
    except:
        import tkMessageBox
        import Tkinter
        msg = u'Numpy is not installed. '
        msg += u'Please download it from \n'
        msg += u'http://openrtm.org/openrtm/ja/content/tkcalibgui'
        Tkinter.Tk().withdraw()
        tkMessageBox.showerror(title = "Numpy not found",
                  message = msg)
                  
def check_rtctree():
    try:
        import rtctree
        print("rtctree is installed.")
    except:
        import tkMessageBox
        import Tkinter
        msg = u'rtctree is not installed. '
        msg += u'Please download it from \n'
        msg += u'http://openrtm.org/openrtm/ja/content/tkcalibgui'
        Tkinter.Tk().withdraw()
        tkMessageBox.showerror(title = "rtctree not found",
                  message = msg)
        return
		
# check execution condition
check_openrtmpy()
check_pil()
check_ttk()
check_numpy()
check_rtctree()

import os
import sys
import socket
import getopt
import re
import time
sys.path.append(".")

# Import RTM module
import RTC
import OpenRTM_aist
from omniORB import CORBA

import tkcalibgui
import CalibrationService_idl

#from rtshell import rtdel
import rtutil

# Import Service implementation class
# <rtc-template block="service_impl">

# </rtc-template>

# Import Service stub modules
# <rtc-template block="consumer_import">
import ImageCalibService, ImageCalibService__POA

# </rtc-template>

# This module's spesification
# <rtc-template block="module_spec">
tkcalibgui_spec = ["implementation_id", "TkCalibGUI", 
		 "type_name",         "TkCalibGUI", 
		 "description",       "Image Calibration GUI", 
		 "version",           "1.2.0", 
		 "vendor",            "AIST", 
		 "category",          "Category", 
		 "activity_type",     "STATIC", 
		 "max_instance",      "1", 
		 "language",          "Python", 
		 "lang_type",         "SCRIPT",
		 ""]
# </rtc-template>

                                    
##
# @class TkCalibGUI
# @brief Image Calibration GUI
# 
# 
class TkCalibGUI(OpenRTM_aist.DataFlowComponentBase):
	
	##
	# @brief constructor
	# @param manager Maneger Object
	# 
	def __init__(self, manager):
		OpenRTM_aist.DataFlowComponentBase.__init__(self, manager)

		self._d_img_check = RTC.CameraImage(RTC.Time(0,0),0,0,0,"",0.0,[])
		"""
		"""
		self._checker_imageIn = OpenRTM_aist.InPort("checker_image", self._d_img_check)

		"""
		"""
		self._CameraCalibrationServicePort = OpenRTM_aist.CorbaPort("CameraCalibrationService")

		"""
		"""
		self._CalibrationService = OpenRTM_aist.CorbaConsumer(interfaceType=ImageCalibService.CalibrationService)

		# initialize of configuration-data.
		# <rtc-template block="init_conf_param">
		
		# </rtc-template>
		self.tree=None

		 
	##
	#
	# The initialize action (on CREATED->ALIVE transition)
	# formaer rtc_init_entry() 
	# 
	# @return RTC::ReturnCode_t
	# 
	#
	def onInitialize(self):
		# Bind variables and configuration variable
		
		# Set InPort buffers
		self.addInPort("checker_image",self._checker_imageIn)
		
		# Set OutPort buffers
		
		# Set service provider to Ports
		
		# Set service consumers to Ports
		self._CameraCalibrationServicePort.registerConsumer("CalibrationService", "ImageCalibService::CalibrationService", self._CalibrationService)
		
		# Set CORBA Service Ports
		self.addPort(self._CameraCalibrationServicePort)
		
		self.pic_num = 0
		return RTC.RTC_OK
	
	#	##
	#	# 
	#	# The finalize action (on ALIVE->END transition)
	#	# formaer rtc_exiting_entry()
	#	# 
	#	# @return RTC::ReturnCode_t
	#
	#	# 
	def onFinalize(self):
		self.gui.quit()
		return RTC.RTC_OK
	
	#	##
	#	#
	#	# The startup action when ExecutionContext startup
	#	# former rtc_starting_entry()
	#	# 
	#	# @param ec_id target ExecutionContext Id
	#	#
	#	# @return RTC::ReturnCode_t
	#	#
	#	#
	#def onStartup(self, ec_id):
	#
	#	return RTC.RTC_OK
	
	#	##
	#	#
	#	# The shutdown action when ExecutionContext stop
	#	# former rtc_stopping_entry()
	#	#
	#	# @param ec_id target ExecutionContext Id
	#	#
	#	# @return RTC::ReturnCode_t
	#	#
	#	#
	#def onShutdown(self, ec_id):
	#
	#	return RTC.RTC_OK
	
	#	##
	#	#
	#	# The activated action (Active state entry action)
	#	# former rtc_active_entry()
	#	#
	#	# @param ec_id target ExecutionContext Id
	#	# 
	#	# @return RTC::ReturnCode_t
	#	#
	#	#
	def onActivated(self, ec_id):

		self.gui.setProviderPointer(self._CalibrationService._ptr())
		
		return RTC.RTC_OK
	
	#	##
	#	#
	#	# The deactivated action (Active state exit action)
	#	# former rtc_active_exit()
	#	#
	#	# @param ec_id target ExecutionContext Id
	#	#
	#	# @return RTC::ReturnCode_t
	#	#
	#	#
	#def onDeactivated(self, ec_id):
	#
	#	return RTC.RTC_OK
	
	##
	#
	# The execution action that is invoked periodically
	# former rtc_active_do()
	#
	# @param ec_id target ExecutionContext Id
	#
	# @return RTC::ReturnCode_t
	#
	#
	def onExecute(self, ec_id):
		
		try:
			if CORBA.is_nil(self._CalibrationService._ptr()):
#				print("TkCalibGUI : [nil] object reference is not assigned.")
				return RTC.RTC_OK
			elif self._CalibrationService._ptr()._non_existent():
#				print("TkCalibGUI : provider is inactive.")
				return RTC.RTC_OK
		except Exception as e:
			print("TkCalibGUI : ",str(e))
			return RTC.RTC_OK

		if self._checker_imageIn.isNew():
			_image = self._checker_imageIn.read()

			# チェスボード撮影枚数確認
			try:
				num = self._CalibrationService._ptr().getImageNumber()
			except Exception as e:
#				print("TkCalibGUI : getImageNumber() : ",str(e))
				return RTC.RTC_OK
				
			if self.pic_num != num:
				self.pic_num = num
				
				# 撮影枚数変更に伴うGUI再描画
				self.gui.redraw_gui(num)
								
			self.gui.set_image( _image.pixels, _image.width, _image.height)	
		return RTC.RTC_OK
	
	#	##
	#	#
	#	# The aborting action when main logic error occurred.
	#	# former rtc_aborting_entry()
	#	#
	#	# @param ec_id target ExecutionContext Id
	#	#
	#	# @return RTC::ReturnCode_t
	#	#
	#	#
	#def onAborting(self, ec_id):
	#
	#	return RTC.RTC_OK
	
	#	##
	#	#
	#	# The error action in ERROR state
	#	# former rtc_error_do()
	#	#
	#	# @param ec_id target ExecutionContext Id
	#	#
	#	# @return RTC::ReturnCode_t
	#	#
	#	#
	#def onError(self, ec_id):
	#
	#	return RTC.RTC_OK
	
	#	##
	#	#
	#	# The reset action that is invoked resetting
	#	# This is same but different the former rtc_init_entry()
	#	#
	#	# @param ec_id target ExecutionContext Id
	#	#
	#	# @return RTC::ReturnCode_t
	#	#
	#	#
	#def onReset(self, ec_id):
	#
	#	return RTC.RTC_OK
	
	#	##
	#	#
	#	# The state update action that is invoked after onExecute() action
	#	# no corresponding operation exists in OpenRTm-aist-0.2.0
	#	#
	#	# @param ec_id target ExecutionContext Id
	#	#
	#	# @return RTC::ReturnCode_t
	#	#

	#	#
	#def onStateUpdate(self, ec_id):
	#
	#	return RTC.RTC_OK
	
	#	##
	#	#
	#	# The action that is invoked when execution context's rate is changed
	#	# no corresponding operation exists in OpenRTm-aist-0.2.0
	#	#
	#	# @param ec_id target ExecutionContext Id
	#	#
	#	# @return RTC::ReturnCode_t
	#	#
	#	#
	#def onRateChanged(self, ec_id):
	#
	#	return RTC.RTC_OK
	
	def set_gui(self, gui):
		self.gui = gui
		
	def set_tree(self, tree):
		self.tree = tree
		
	def get_tree(self):
		return self.tree		

def get_options(pid, opts):
	o_option = False
	for o, a in opts:
		if o == "-o":
			index = a.find("naming.formats")
			if index != -1:
				# naming.formatsのオプション指定あり
				index = a.find("%p")
				comp_option = a[:index] + str(pid) + a[index+2:]
				o_option = True

	if o_option == False:
		# naming.formatsのオプション指定が無いので追加する
		comp_option = "naming.formats:cvcalib{0}/%n.rtc".format(pid)

	return comp_option
	
def usage():
	print("usage: TkCalibGUIComp.py [-o naming.formats]")

def main():
	calibgui = tkcalibgui.TkCalibGUI()
	
	pid = os.getpid()
	rt_dir = "/localhost/cvcalib{0}/".format(pid)
	try:
		opts, args = getopt.getopt(sys.argv[1:], "o:f:")
	except getopt.GetoptError as err:
		print(usage())
		return

	comp_option = get_options(pid, opts)
	sys.argv += ["-o", comp_option]
	sys.argv += ["-o", "manager.shutdown_on_nortcs:NO"]
	sys.argv += ["-o", "manager.shutdown_auto:NO"]

	mgr = OpenRTM_aist.Manager.init(sys.argv)
	mgr.activateManager()
	orb = mgr.getORB()
	
	# Register component
	profile = OpenRTM_aist.Properties(defaults_str=tkcalibgui_spec)
	mgr.registerFactory(profile,
                            TkCalibGUI,
                            OpenRTM_aist.Delete)
	# Create a component
	comp = mgr.createComponent("TkCalibGUI")
	comp.set_gui(calibgui)
	calibgui.set_on_update(comp.set_tree)
	mgr.runManager(True)
	
	calibgui.set_options(orb, rt_dir, comp_option)
	calibgui.mainloop()
	
	# GUIの×ボタンを押されるとmainloopを抜けてここへ来る
	# コンポーネントの終了処理を確認する
	calibgui.exit_comp()
	
	while True:
		comp_list = mgr.getComponents()
		# is finished?
		if len(comp_list) == 0:
			break
		time.sleep(0.9)
		
	# Delete context
	tree = comp.get_tree()
#	rtdel.main([str(rt_dir)], tree)
	rtutil.delComponentRef(str(rt_dir), tree)

	mgr.shutdown()

if __name__ == "__main__":
	main()

