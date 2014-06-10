#!/usr/bin/env python
# -*- coding: utf-8 -*-
# -*- Python -*-

"""
 @file tkcalibgui.py
 @brief calibration gui
 @date $Date$


"""
from Tkinter import *
import ttk
from PIL import Image, ImageTk
import time
import sys
import os.path
#from cv2.cv import *
import numpy
from io import BytesIO
from StringIO import StringIO
from threading import Thread

import rtutil

WIN_DEFAULT_CAMERA = {
				"DirectShowCamComp" : ":m_FIN_OUTPORT",
				"OpenCVCameraComp"  : ":out",
				"PGRCameraComp"     : ":outputImage",
				"MFCameraComp"      : ":out"
				}
LINUX_DEFAULT_CAMERA = {
				"OpenCVCameraComp"  : ":out",
				"PGRCameraComp"     : ":outputImage"
				}
GUI_COMP = "TkCalibGUI"
CALIB_COMP = "ImageCalibration"

class TkCalibGUI(Frame):
	def __init__(self, width=640, height=480, master=None):
		Frame.__init__(self, master)
		self.img_w = width
		self.img_h = height
		self.window_w = 1050
		self.window_h = 550
		self.frame_w = self.window_w - self.img_w - 50
		self.prev_time = 0.0
		self.curr_time = 0.0
		self.fps = 0.0
		self.pic_num = 5  # 保存画像枚数
		self.buttons = [] # 保存・確認・取消ボタン
		self.save_btn_state = [] # 保存ボタンの状態保存用
		self.view_flg = True	# カメラ画像を表示する
		self.comp_all_found = False

		self.camera_select_flg = False
		self.exit_btn_click = False
			
		self.result_mes = StringVar()
		self.result_mes.set("")
	
		# for image processing
		self.flip_tb = False	# Flip the image vertically (top to bottom).
		self.flip_lr = False	# Flip image horizontally (left to right).
		self.invert = False		# Invert (negate) the image.
		self.print_fps = True
		
		self.option_add('*font', '* 11')
		self.option_add('*LabelFrame.font', '* 11')
		self.option_add('*Button.font', '* 11')
		
		self.init(master)

	def init(self, master):
		self.check_lang()
		self.check_camera_comp()
		# Top level frame
		self.master.minsize(self.window_w, self.window_h)
		self.master.title(self.msg1)		
		self.fr = Frame(master, width=self.frame_w).pack()
		self.set_top_message()
		# left pane
		self.create_left()
		# right top pane
		self.create_right()
		return

	def check_lang(self):
		import locale
		loc = locale.setlocale(locale.LC_ALL, "")
		if "ja_JP" or "Japanese_Japan" in loc:
			self.msg1 = "カメラキャリブレーション"
			self.msg2 = "＊チェッカーパターンを異なる姿勢で{0}枚撮影（保存ボタン押下）して下さい" .format(self.pic_num)
			self.msg3 = "保存"
			self.msg4 = "確認"
			self.msg5 = "取消し"
			self.msg6 = "カメラ映像表示"
			self.msg7 = "結果表示"
			self.msg8 = "結果保存" 
			self.msg9 = "枚目"
			self.msg10 = "カメラコンポーネント"
			self.msg11 = "RTSystemEditorで手動にてコンポーネントを接続して下さい"
			self.err_msg1 = "保存画像のいずれかが不鮮明で、結果を算出できません"
			self.err_msg2 = u"コンポーネントが見つかりません"
			self.err_msg3 = "自動接続できませんでした"
		else:
			self.msg1 = "Camera Calibration"
			self.msg2 = "* Please press the save button in {0} different poses the checkered pattern." .format(self.pic_num)
			self.msg3 = "Save"
			self.msg4 = "Check"
			self.msg5 = "Cancel"
			self.msg6 = "View Camera Image"
			self.msg7 = "View Results"
			self.msg8 = "Save Results"
			self.msg9 = "piece"
			self.msg10 = "Camera component"
			self.msg11 = "Connect the component manually in RTSystemEditor."
			self.err_msg1 = "One of the stored image is blurry, you can not calculate the result."
			self.err_msg2 = "Component not found."
			self.err_msg3 = "Could not automatically connect."
		return

	# check build camera component
	def check_camera_comp(self):
		camera_list = self.get_avail_camera_list()
		self.select_camera_list = self.get_comp_path(camera_list)
		
	def get_comp_path(self, comp_name):
		import platform
		os_name = platform.system()
		path_list = self.get_search_path()
		comp_path = {}
		for i in range(len(comp_name)):
			for j in range(len(path_list)):
				path = path_list[j] + comp_name[i]
				if os_name == "Windows":
					path += ".exe"
				if os.path.isfile(path):
					print "find path = {0}".format(path)
					base, ext = os.path.splitext(comp_name[i])
					# basename : path
					comp_path[base] = path_list[j]
					break
		if len(comp_path) == 0:
			comp_path[self.err_msg2] = ""
		return comp_path

	def create_left(self):
		self.frameL_base = Frame(self.fr, width=self.frame_w)
		self.frameL_base.pack(side = LEFT, padx=3, fill=Y)
		logo = Image.open(BytesIO(RTMLogo().get_image()))
		self.cam_img = ImageTk.PhotoImage(logo)
		self.cam_img_label = Label(self.frameL_base, image=self.cam_img)
		self.cam_img_label.pack(side = TOP)
		self.create_flip_button(self.frameL_base)
				
	def create_right(self):
		self.frameR_base = Frame(self.fr, width=self.frame_w)
		self.frameR_base.pack(side = LEFT, padx=3, fill=Y)
		self.create_camera_combo(self.frameR_base)
		
	# left pane
	def create_flip_button(self, frame):
		fr = LabelFrame(frame, text="Flip image",
						relief=GROOVE, bd=2)
		fr.pack(fill=Y, pady=3, ipadx=3, ipady=3)		
		b0 = Button(fr, text="Flip (top to bottom)",
						state = DISABLED, width=20,
						command = self.flip_top_bottom)
		b0.grid(column=1, row=1, padx=3)
		b1 = Button(fr, text="Flip (left to right)",
						state = DISABLED, width=20, 
						command = self.flip_left_right)
		b1.grid(column=2, row=1, padx=3)
		self.flip_button = {'tb': b0, 'lr': b1}
		
	def flip_top_bottom(self):
		tmp = self.flip_tb
		self.flip_tb = not tmp
		
	def flip_left_right(self):
		tmp = self.flip_lr
		self.flip_lr = not tmp

	# right pane (1)
	def create_camera_combo(self, frame):
		self.fix1 = LabelFrame(frame, text="Select Camera",
							 relief=GROOVE, bd=2)
		self.fix1.pack(fill=X, ipadx=3, ipady=3)	
		lb = Label(self.fix1, text=self.msg10)
		lb.grid(column=0, row=0, padx=3)
		self.box = ttk.Combobox(self.fix1, values=self.select_camera_list.keys(), state='readonly',
												width=30, height=2)
		self.box.grid(column=1, row=0, padx=3, pady=3)
		self.box.bind("<<ComboboxSelected>>", self.change_camera)
		self.comp_btn = Button(self.fix1, text="OK", state = DISABLED,
						command = self.camera_selected)
		self.comp_btn.grid(column = 2, row = 0)	

	# [callback] camera select button		
	def camera_selected(self):
		self.comp_btn.config(state = DISABLED)
		if self.comp_all_found:
			# automatically connect the components
			ret = self.connect_components_auto()
			if not ret:
				self.register_me()
				self.comp_all_found = False
		else:
			# manually connect the components
			self.register_me()		

		# camera selected
		self.camera_select_flg = True
		
		# right pane (2)
		fr = Frame(self.frameR_base, width=self.frame_w)
		fr.pack(ipadx=3, ipady=3, fill=X)		
		self.create_state_transition_button(fr)
		# right pane (3)
		self.frameR_var = Frame(self.frameR_base, width=self.frame_w)
		self.frameR_var.pack(fill=X)
		self.create_capture_button(self.frameR_var)
		# right pane (4)
		self.create_result_view(self.frameR_var)		
		self.on_update(self.tree)
		
	# right top pane (2)
	def create_state_transition_button(self, frame):
		import platform
		os_name = platform.system()
		
		fr = LabelFrame(frame, text="Component", labelanchor=NW,
							 relief=GROOVE, bd=2)
		fr.pack(fill=X, ipadx=3, ipady=3)	
		
		if self.comp_all_found:
			# all
			act_state = "All Activate"
			deact_state = "All Deactivate"
			exit_state = "All Exit"
			act_button = NORMAL
		else:
			# myself
			act_state = "Activate"
			deact_state = "Deactivate"
			exit_state = "Exit"
			act_button = DISABLED

		if os_name == "Windows":
			btn_w = 16
		else:
			btn_w = 13
		b0 = Button(fr, text=act_state,
						state = act_button, width=btn_w,
						command = self.activate_comp)
		b0.grid(column=1, row=1, padx=3)
		b1 = Button(fr, text=deact_state,
						state = DISABLED, width=btn_w, 
						command = self.deactivate_comp)
		b1.grid(column=2, row=1, padx=3)
		b2 = Button(fr, text=exit_state,
						state = NORMAL, width=btn_w,
						command = self.exit_comp)
		b2.grid(column=3, row=1, padx=3)
		self.comp_button = {'activate':   b0,
								 'deactivate': b1, 'exit': b2}

	# right top pane (3)
	def create_capture_button(self, frame):
		import platform
		os_name = platform.system()
		self.fr_cap = LabelFrame(frame, text="Capture", labelanchor=NW,
							 relief=GROOVE, bd=2)
		self.fr_cap.pack(fill=X, ipady=3)
		
		if os_name == "Windows":
			btn_w = 10
		else:
			btn_w = 9
		for i in range(self.pic_num):
			l0 = Label(self.fr_cap, padx = 25, 
						text="{0:d} {1}: ".format(i+1, self.msg9))
			l0.grid(column = 0, row = i)
			b0 = Button(self.fr_cap, text=self.msg3,
						state = DISABLED, width = btn_w, 
						command = self.save_cmd(i))
			b0.grid(column=1, row=i, padx=2)
			b1 = Button(self.fr_cap, text=self.msg4,
						state = DISABLED, width = btn_w,
						command = self.check_cmd(i))
			b1.grid(column=2, row=i, padx=2)
			b2 = Button(self.fr_cap, text=self.msg5,
						state = DISABLED, width = btn_w, 
						command = self.cancel_cmd(i))
			b2.grid(column=3, row=i, padx=2)
			self.buttons.append({'label': l0, 'save':   b0,
								 'check': b1, 'cancel': b2})

			# After saving all the images, enable "View Results" button.
			self.save_btn_state.append(0)

		self.view_cam_btn = Button(self.fr_cap, text=self.msg6, state = DISABLED,
					  width=34, command=self.camera_on)
		self.view_cam_btn.grid(column = 1, row = self.pic_num+1, columnspan = 3)
	
	# right top pane (4)	
	def create_result_view(self, frame):
		self.fr_result = LabelFrame(frame, text=self.msg7,
							 relief=GROOVE, bd=2, width=self.frame_w)
		self.fr_result.pack(padx=3, ipadx=3, ipady=3, fill=X)
		l = Label(self.fr_result, width=50, height=10, relief=SUNKEN, anchor=NW,
				  bg="white", textvariable = self.result_mes, justify=LEFT)
		l.pack() # width/height 文字数・行数指定
		self.result_btn = Button(self.fr_result, text=self.msg7,
					 width=8, state = DISABLED,
					 command=self.get_result)
		self.result_btn.pack(pady = 3)

	def set_top_message(self):
		self.variable_mes = StringVar()
		Label(self.fr, textvariable=self.variable_mes, fg="blue", font=20).pack(pady=10)
		self.variable_mes.set(self.msg2)

	def get_avail_camera_list(self):
		import platform
		os_name = platform.system()
		if os_name == "Windows":
			self.default_camera = WIN_DEFAULT_CAMERA
		else:
			self.default_camera = LINUX_DEFAULT_CAMERA
		camera_list = self.default_camera.keys()
		return camera_list
					
	def get_search_path(self):
		import platform
		os_name = platform.system()
		ret_path  = []
		curdir = os.getcwd() + "/"
		ret_path.append(curdir)
		if os_name == "Windows":
			env = os.environ.get("RTM_ROOT")
			if env != None:
				path = env + "examples\\C++\\opencv-rtcs\\"
				ret_path.append(path.replace('\\', '/'))
		else:
			tmp = (
					"/usr/share/openrtm-1.1/components/c++/opencv-rtcs/",
					"/usr/bin/", "/usr/X11R6/bin/",
					"/usr/local/bin/", "/bin/",
					"/opt/bin/", "/opt/local/bin/",
					"/sbin/", "/usr/sbin/")
			for e in tmp:
				ret_path.append(e)
		return ret_path

	def camera_on(self):
		self.CameraEnable(True)
		
	def CameraEnable(self, enable):
		if enable:
			self.view_flg = True
		else:
			self.view_flg = False       		

	# [callback] camera select combobox	
	def change_camera(self, event):
		camera = event.widget.get()	
		if camera.find("Comp") != -1:
			self.camera_comp = camera
			self.comp_all_found = True			
		else:
			# camera not found
			self.set_warning_msg()
			
		if self.camera_select_flg:
			self.comp_btn.config(state = DISABLED)
		else:
			self.comp_btn.config(state = NORMAL)
		
	def set_warning_msg(self):
		Label(self.fix1, text=self.msg11, fg="red").grid(column = 0, row = 1, columnspan = 3)	
		
	def set_calib_not_found_msg(self):
		Label(self.fix1, text=CALIB_COMP+self.err_msg2, fg="red").grid(column = 0, row = 2, columnspan = 3)	
							
	def connect_components_auto(self):
		import platform
		import subprocess
		os_name = platform.system()
		# Remove the last four characters of "Comp".
		camera_comp = self.camera_comp[:-4]
		camera_rtc = self.rt_dir + camera_comp + "0.rtc"
		camera_path = self.select_camera_list[self.camera_comp]

		calib_rtc = self.rt_dir + CALIB_COMP + "0.rtc"
		tk_rtc = self.rt_dir + GUI_COMP + "0.rtc"

		# External components start
		name = []
		if os_name == "Windows":
			# camera
			comp_name = camera_path + self.camera_comp + ".exe"
			print "windows: comp_name = {0}".format(comp_name)
			subprocess.Popen([comp_name, "-o", self.comp_option])
			camera_outport = self.default_camera[self.camera_comp]
		
			# ImageCalibration
			comp_name = "{0}Comp".format(CALIB_COMP)
			name.append(comp_name)
			lists = self.get_comp_path(name)
			if len(lists) == 0:
				self.set_warning_msg()
				self.set_calib_not_found_msg()
				return False
			else:
				# Environment variable exists
				val = lists.get(comp_name, "")
				if val == "":
					self.set_warning_msg()
					self.set_calib_not_found_msg()
					return False
				else:
					calib_path = val
			comp_name = calib_path + comp_name
			subprocess.Popen([comp_name, "-o", self.comp_option])
		else:
			# camera
			comp_name = camera_path + self.camera_comp
			subprocess.Popen([comp_name, "-o", self.comp_option])			
			camera_outport = self.default_camera[self.camera_comp]
		
			# ImageCalibration
			comp_name = "{0}Comp".format(CALIB_COMP)			
			name.append(comp_name)
			lists = self.get_comp_path(name)
			val = lists.get(comp_name, "")
			if val == "":
				self.set_warning_msg()
				self.set_calib_not_found_msg()
				return False
			else:
				calib_path = val
			comp_name = calib_path + comp_name
			subprocess.Popen([comp_name, "-o", self.comp_option])
	
		# Component startup check
		self.comp_startup_check(tk_rtc)
		self.comp_startup_check(camera_rtc)
		tree = self.comp_startup_check(calib_rtc)
		
		ret1, res = rtutil.connectPorts(str(camera_rtc)+str(camera_outport), str(calib_rtc)+":original_image", tree)
		ret2, res = rtutil.connectPorts(str(calib_rtc)+":checker_image", str(tk_rtc)+":checker_image", tree)
		ret3, res = rtutil.connectPorts(str(calib_rtc)+":CameraCalibrationService", str(tk_rtc)+":CameraCalibrationService", tree)

		if ret1 < 0 or ret2 < 0 or ret3 < 0:
			Label(self.fix1, text=self.err_msg3, fg="red").grid(column = 0, row = 1, columnspan = 3)
			Label(self.fix1, text=self.msg11, fg="red").grid(column = 0, row = 2, columnspan = 3)
			return False
		
		self.set_comp_name(camera_rtc, calib_rtc, tk_rtc, tree)
		return True

	def register_me(self):
		tk_rtc = self.rt_dir + GUI_COMP + "0.rtc"
		self.tree = self.comp_startup_check(tk_rtc)
		self.rtc_list = [tk_rtc]
		
	def comp_startup_check(self, name):
		import rtctree.tree
		import rtctree.path
		judge = " not found."	
		# generate rtctree
		path, port = rtctree.path.parse_path(name)
		path = path[:-1]
		filter = [path]
		for cnt in range(0, 10):
			tree = rtctree.tree.RTCTree(paths=path, orb=self.orb, filter=filter)
			tree1,comp1, port1 = rtutil.getComponent(str(name), tree=tree)
			if tree1 and comp1:
				judge = " found."
				break
			time.sleep(1)
		print name + judge	
		return tree	

	# [callback] activate button	
	def activate_comp(self):
		self.comp_button['activate'].config(state = DISABLED)
		self.comp_button['deactivate'].config(state = DISABLED)
		self.comp_button['exit'].config(state = DISABLED)
		self.flip_button['tb'].config(state = NORMAL)
		self.flip_button['lr'].config(state = NORMAL)
		self.CameraEnable(True)

		# Start a thread to observe the transition state of the component.
		self.comp_transition_thread = CompTransitionThread('Activate a component.', self.rtc_list, self.tree)
		self.comp_transition_thread.start()
		self.check_activate()
		
	def check_activate(self):
		if not self.comp_transition_thread.running:
			self.comp_button['deactivate'].config(state = NORMAL)
			self.comp_button['exit'].config(state = NORMAL)
		else:
			self.master.after(100, self.check_activate)
	
	# [callback] deactivate button
	def deactivate_comp(self):
		self.comp_button['activate'].config(state = DISABLED)
		self.comp_button['deactivate'].config(state = DISABLED)
		self.comp_button['exit'].config(state = DISABLED)
		self.flip_button['tb'].config(state = DISABLED)
		self.flip_button['lr'].config(state = DISABLED)
		self.CameraEnable(False)
		
		# Start a thread to observe the transition state of the component.
		self.comp_transition_thread = CompTransitionThread('Deactivate a component.', self.rtc_list, self.tree)
		self.comp_transition_thread.start()
		self.check_deactivate()
				
	def check_deactivate(self):
		if not self.comp_transition_thread.running:
			self.comp_button['activate'].config(state = NORMAL)
			self.comp_button['exit'].config(state = NORMAL)
		else:
			self.master.after(100, self.check_deactivate)
	
	# [callback] exit button
	def exit_comp(self):
		if self.exit_btn_click:
			# exit_comp already completed.
			return
			
		# Start a thread to observe the transition state of the component.
		self.comp_transition_thread = CompTransitionThread('Exit', self.rtc_list, self.tree)
		self.comp_transition_thread.start()
		self.exit_state = True
		self.check_exit()
		
		self.exit_btn_click = True

	def check_exit(self):
		if not self.comp_transition_thread.running:
			self.exit_state = False
		else:
			self.master.after(10, self.check_exit)

	# [callback] save button
	def save_cmd(self, n):
		return lambda : self.save_img(n)

	def save_img(self, num):
		self.buttons[num]['save'].config(state = DISABLED)
		self.buttons[num]['check'].config(state = NORMAL)
		self.buttons[num]['cancel'].config(state = NORMAL)
		self.provider.captureCalibImage(num)
		self.save_btn_state[num] = 1
		if not 0 in self.save_btn_state:
			self.result_btn.config(state = NORMAL)
			
	# [callback] check button
	def check_cmd(self, n):
		return lambda : self.check_img(n)

	def check_img(self, num):
		cameraImage = self.provider.getCalibImage(num)
		width = cameraImage.width
		height = cameraImage.height
		image = cameraImage.pixels
		pilImage = Image.frombuffer("L", (width, height), image, "raw", "L", 0, 1)
		self.cam_img.paste(pilImage)
		self.view_cam_btn.config(state = NORMAL)
		self.CameraEnable(False)

	# [callback] cancel button
	def cancel_cmd(self, n):
		return lambda : self.cancel_img(n)

	def cancel_img(self, num):
		cameraImage = self.provider.removeCalibImage(num)
		self.buttons[num]['save'].config(state = NORMAL)
		self.buttons[num]['check'].config(state = DISABLED)
		self.buttons[num]['cancel'].config(state = DISABLED)
		self.save_btn_state[num] = 0
		self.result_mes.set("")
		self.result_btn.config(state = DISABLED)
		self.CameraEnable(True)
					
	# [callback] get result button
	def get_result(self):
		self.cameraInfo = self.provider.getCalibParameter()
		if self.cameraInfo.focalLength.x == 0.0:
			self.result_mes.set(self.err_msg1)
			return

		param_str = []
		param_str.append("cameraInfo\n")
		param_str.append("focalLength.x = %f\n" % self.cameraInfo.focalLength.x)
		param_str.append("focalLength.y = %f\n" % self.cameraInfo.focalLength.y)
		param_str.append("principalPoint.x = %f\n" % self.cameraInfo.principalPoint.x)
		param_str.append("principalPoint.y = %f\n" % self.cameraInfo.principalPoint.y)
		param_str.append("Radial distortion(k1) = %f\n" % self.cameraInfo.k1)
		param_str.append("Radial distortion(k2) = %f\n" % self.cameraInfo.k2)
		param_str.append("Tangential distortion(p1) = %f\n" % self.cameraInfo.p1)
		param_str.append("Tangential distortion(p2) = %f" % self.cameraInfo.p2)
		mes_str = param_str[0]
		for i in range(1, len(param_str)):
			mes_str +=  param_str[i]
		self.result_mes.set(mes_str)
		self.result_btn.config(state = DISABLED)

	def calc_fps(self):
		self.prev_time = self.curr_time
		self.curr_time = time.time()
		self.fps = (self.fps * 0.9) + (0.1 / (self.curr_time - self.prev_time))
		return self.fps

	def set_comp_name(self, camera, calib, gui, tree):
		self.camera_rtc = camera
		self.calib_rtc = calib
		self.tk_rtc = gui
		self.tree = tree
		self.rtc_list = [self.camera_rtc, self.calib_rtc, self.tk_rtc]

	def redraw_frameR(self):
		self.fr_cap.destroy()
		self.fr_result.destroy()
		self.buttons = []
		self.create_capture_button(self.frameR_var)
		self.create_result_view(self.frameR_var)
						
#------------------------------------------------------------
# public function

	def setProviderPointer(self, provider):
		self.provider = provider
			
	def set_image(self, image, width, height):
		if self.view_flg:
			self.flip_button['tb'].config(state = NORMAL)
			self.flip_button['lr'].config(state = NORMAL)
		else:
			self.flip_button['tb'].config(state = DISABLED)
			self.flip_button['lr'].config(state = DISABLED)
			return

		from PIL import ImageOps,ImageDraw
		# Getting PIL image from buffer, converting RGB->BGR
		self.pilImage = Image.frombuffer("RGB", (width, height),	image, "raw", "BGR")
		# Resize if size is different
		if self.img_w != width or self.img_h != height:
			self.pilImage = ImageOps.fit(self.pilImage, (self.img_w, self.img_h))
		# Flip if the flag is True
		if self.flip_tb:
			self.pilImage = ImageOps.flip(self.pilImage)
		if self.flip_lr:
			self.pilImage = ImageOps.mirror(self.pilImage)
		# Invert color
		if self.invert:
			self.pilImage = ImageOps.invert(self.pilImage)
		# Print FPS
		if self.print_fps:
			draw = ImageDraw.Draw(self.pilImage)
			fps_str = "%2.2f [fps]" % self.calc_fps()
			draw.text((self.img_w - 70, self.img_h - 12), fps_str)
		# Finally paste image to the label
		self.cam_img.paste(self.pilImage)
		
	def redraw_gui(self, num):
		if self.pic_num != num:
			# Remove all saved images
			self.provider.removeCalibImage(-1)
			self.pic_num = num
			print "tkcalibgui.py : redraw_gui : pic_num changed = %d" % self.pic_num
			self.check_lang()
			self.variable_mes.set(self.msg2)
			self.redraw_frameR()
			self.result_mes.set("")

		# Save button enable
		for i in range(self.pic_num):
			self.buttons[i]['save'].config(state = NORMAL)			
		
	def set_options(self, orb, rt_dir, comp_option):
		self.orb = orb
		self.rt_dir = rt_dir
		self.comp_option = comp_option
		self.register_me()

	def set_on_update(self, func):
		self.on_update = func

#============================================================
# Thread class
		
class CompTransitionThread(Thread):
	def __init__(self, msg, rtc_list, tree):
		Thread.__init__(self)
		self.msg = msg
		self.rtc_list = rtc_list
		self.tree = tree
		self.running = False
		
		if self.msg.find("Activate") != -1:
			# Activate
			self.cmd = "rtact"
		else:
			# Deactivate or Exit
			self.cmd = "rtdeact"
		
	def run(self):
		comp_cnt = len(self.rtc_list)
		self.running = True
		for i in range(comp_cnt):
			self.check_comp(self.cmd, self.rtc_list[i])
			
		if self.msg == "Exit":
			for i in range(comp_cnt):
				# rtdis
				self.check_comp('rtdis', self.rtc_list[i])
			
			for i in range(comp_cnt):
				# rtexit
				self.check_comp('rtexit', self.rtc_list[i])
		self.running = False

	def check_comp(self, cmd, rtc):
		for cnt in range(0, 10):
			if cmd == "rtdis":
				ret,res = rtutil.disconnectAll(str(rtc), self.tree)

			elif cmd == "rtexit":
				ret,res = rtutil.exitComponent(str(rtc), self.tree)

			elif cmd == "rtact":
				ret,res = rtutil.activateComponent(str(rtc), tree=self.tree)
				if ret == 1: ret=0
				else: ret=0

			elif cmd == "rtdeact":
				ret,res = rtutil.deactivateComponent(str(rtc), tree=self.tree)
				if ret == 1: ret=0
				else: ret=0
			else:
				pass

			if ret == 0:
				print "{0}:{1} OK!".format(cmd, str(rtc))
				break
			else:
				print "{0}:{1} NG!".format(cmd, str(rtc))
		return ret

class RTMLogo:
	logo = """iVBORw0KGgoAAAANSUhEUgAAAoAAAAHgCAIAAAC6s0uzAAAgAElEQVR4nOzdd7wk
VZn/8ec5p6o6pxvmzgxpCBJkjSvrz4CIKGsWFF2zoqtiDugadldcI4oRdwUVUFlF
MazLGmBZxCyKAUFFRHKcmRs6Vnd1VZ3z/P7oGQSccGfm3q7q7u/75R++eM3cOndu
dX3uOZV4v/VzBAAAAMOlkh4AAADAJEKAAQAAEoAAAwAAJAABBgAASAACDAAAkAAE
GAAAIAEIMAAAQAIQYAAAgAQgwAAAAAlAgAEAABKAAAMAACQAAQYAAEgAAgwAAJAA
BBgAACABCDAAAEACEGAAAIAEIMAAAAAJQIABAAASgAADAAAkAAEGAABIAAIMAACQ
AAQYAAAgAQgwAABAAhBgAACABCDAAAAACUCAAQAAEoAAAwAAJAABBgAASAACDAAA
kAAEGAAAIAEIMAAAQAIQYAAAgAQgwAAAAAlAgAEAABKAAAMAACQAAQYAAEgAAgwA
AJAABBgAACABCDAAAEACEGAAAIAEIMAAAAAJQIABAAASgAADAAAkAAEGAABIAAIM
AACQAAQYAAAgAQgwAABAAhBgAACABCDAAAAACUCAAQAAEoAAAwAAJAABBgAASAAC
DAAAkAAEGAAAIAEIMAAAQAIQYAAAgAQgwAAAAAlAgAEAABKAAAMAACQAAQYAAEgA
AgwAAJAABBgAACABCDAAAEACEGAAAIAEIMAAAAAJQIABAAASgAADAAAkAAEGAABI
AAIMAACQAAQYAAAgAQgwAABAAhBgAACABCDAAAAACUCAAQAAEuAkPYB0qVvbEiGR
pAcCQ6WUqjEXmZMeyOrzDXcMGUOEnRxWj5K8QyVNegI+U3sAAd5iS3qtldnZQFDg
CeIRnReGL2y1FpmJ2WGeYs6NX4y7htuGTdj79OFySJ4M9nFYNQWdPf736s6ukCMF
TSVNauw+UCuB91s/l/QYEiNELZGGtSRCIr3p6ZjIEi2hv5PEIyoq5YlkmYnozd3u
R7pdYnaUmmLO8JYUj+TxY7An9yy3DNuwd9oh9ogSB0TdGAGGVVTQ4hDltVScwv1+
RSSSd6isiYnG71fbPTChARYRS9QSaVl7fa12gOOItS2RRRGT9Nhg+HhrX3NEVaXy
zMT8rFbra0FASnnMNWaPiHl0Dh4iJESB5Z7lftQ/5cD4KdNcj6hlOEJ6YfUxEZGU
tBS1rM8UD/qpuK54isoaDb7LJAbYirREDHPH2h+Xyw/WepNInPSoIFUKRFNKZZmJ
+aGNxlXGCFGV2SVS6c+wCAWW+yJdG71+r/jFc7yA9EJipKSloqXk5B/wG8ozlbEi
vcUkBrgtEohcWCo9ynUb1tZRX9i+ElFNqYxSh9TrTWvrRDVml0intsKD+nYlPHEu
Pmkdz0fUNhwivZAwqWhyVPYpV5FSaPDAxAXYiNwpcl6h8EzPu9naKOnxwEgoMdeY
M0qtW1oqM99k7Syzk8IGb537xs+YCV++jhci7iO9kArCRGVNhrLPv5r6RGVcIz1h
9wEbkaZIhrlPtAn1hWVri9xi7SZjbqpW/1SrrdP6Dmt7InHarteLhC3FJ6C+kDos
RC1DDgdnH0plRS2DKwEnKMCD+paUOrdQOMF1g6THAyOnJXKLtfPG3FAq7afUAVpv
FonS02ArFFjzgEL4+r1RX0ghFqJWTAUdfPQgmnaoNelX409KgAf1LSv14Xz+iY6z
WQTTX9g9DZF5oj9Vqz8qle6r9XxKGmyFAitCdl2G51FfSCm2RK2YZtzg/fvTXlnq
TvR9J5MS4IBorVIfyWaPd93NImHS44GRNpgKt0S+Uyw+OCUNNsJa2eetiV69fsIP
apBybImaMe2V6b96PbcNxUl/dpIzKQHuiTzd8/4hm91kLeoLe06IGiJZ5i8Xiw8d
/FaXYIOtUM+aGSd8xXpeiHDNM6Tclnlwhs1heepN7u+LExHgWMQXWRDxUV9YOZao
IVJhPjuff5TW8wk22JK4yh5ZQX1hZEQih+Wjl6zjnk16KImZiAD3iQ7X+ljH6SY9
EhgzlqguMsN8erF4lOsm1mAjXHPC1+1F/uROJmC0sCXyLTdjig1N6iNiJiLAgciR
jvOsTMZP/EQdjJ3BPHgd88dyuWMcJ5kGW6HAcmAx/YVRElnZkDUPr1Iwob84jn+A
Y5GOyIJIF1c+w+owRA2RvZT6aKHwFNfF73kAyxKJPTBrjq1N7Cr0+Ae4T/QQ1z3J
83BYhNUzaPAG5kc5TmvIk+Ct01/qT+hRDEYUW+K+UF8m9kLoCQiwyH2UOiaT6SDA
sJoGDT7SdR8/5IuiLUlB99++D3cmdB0PYESNf4ANUZ9I8MYFWH0B0QalPlkonOC6
vaFt1QorMsdOkY8ZMIyaWLhriCyFk7j3jn+AAYbGEDVF9lJqvVINa/vDmQQLUSQU
CU/qOh6MsMiaI6vR8bMUIMAAsGcMUcPa12azz/Y8PG8cYCdCsVOOHJBnBBgA9lyX
aI45N8xJMMCI0kwZRRklziS+mtBJegAA48YSNUVOy+VCa78bx5mkxwOQXh7r7y66
p99G2UmcDU7i9wyw2nyiLFGXqD6ESbAiKWopKsnj4wyjxlUcCXcsAjyeMsxVZmbO
Jz0SmBxC1BL5WrH4XM9b9cePK1ab4twTfy8FvdqbAlhhTKSZiEhN4hL0+AfYIzon
CJ7b6RR4En/AkBSfqCeykWjJ2tW9J1iR5Jl7looIMIyayT4qj3+AM8wZZo+ohADD
EAlRW+R7pdKzPK9PtIoFZiY1+N/qbQNg5YnLUtCUZ8lN6L47/t82E+WYc8xapJD0
YGCidIja1i4SLVkbrfYkuMhCJIXx/0TD+PBY/89i5l9voOyELt5MxMc1S3RmELyw
2y0xZsEwVG2RS0qlJ3re6r4IRLO6Lsg99fdScgS7OIwCcVlymrNssy5hBjzGMswV
5iIzE+FSLBgmn4hEGiKrOwPWLFWHai5HlnAtNIyEjNI/aWbefQPlJnT6SxMSYCLK
Mp8RBO/odkvMk/I9w0RxWP2pm3nVdVJ2BLs4pJtkWDLMoZ3k6S9NVIDLzDFznxkN
hmESoogoIrJDmATnmBuGShoNhtQSh0Wzc8GS9/6bKT+5018i0tVSMekxDIki+kEU
bbL2SK0LSkWremEqwFYu0dVxfLW1mkiv6kUITHx7qH7VNn9XpqpDkTB2cUihnNK3
h9lTrpOpzGQ+f+MuExRghznL/LM4bhA9XOsiGgxDYYmek81+KwwXRNxVDbBi8pTa
GKqre+aBBZryKLJoMKSKZJVklf5VS/26S+VJfxbyBAWYtjb4p3HcJnoYGgxDERNN
K/WJIGgQZVb7MvxBg+/oq+sCc3ieZjwK0WBIC8kqybJzwZJ3+u1UcWgiX8Bwd5MV
YNra4J/EcQsNhmGZVuqMIGgNIcC0tcG39dWNgRyal1mXQqxFQ8LEY8prKTve+fPu
p++QKXfCF58HJi7AdM8GH+M4VWZLFCc9KhhjQw0wbWkw39bnW0JzeIHWeWSJLDIM
CdiS3jWe+6XNfGPgfQr1/YtJDDDdrcEB0eXGHOW6a5TSREy0ug9MgIk07ADT1nnw
LYHaGNKdfZl1ZL8saSYmMnhWBwzDlvSu9dzPb1TXB94Zt+qrelRyUN+7TO458Czz
nFLn9vstkY5IiWhe5DPFYsNaX6Sb9PAA9pTDUnXU73z906a5ticH5akZR8+dlWmX
fEMBzg3DqhCXKaskq6is3U/dSR67X9goeUemskjvvUxugIkoy5xlzol8od/vEUXW
VrTebMwXisV4td/hCknwRXwRP+lhDI/DVNSSUeoqn37Z5sCwH4vL0avWy5QnNunh
wVjKK/cLd/JtoVQc9yubRSuZ8ijDhAcB/5WJDvDAIMM1ooD5M71eS2SKeQkBHju+
yC3WXlgq+ZP2w3WZXE0FLZFWP29zM+aIxSMyE/bvAEMhea1/0FD1WHJKCg7SuwMI
8F9kmTPMOZHPhWF/0o7RYy0QIRGy9gOlUtJjSZSryCXxWP28SaEQdnJYDS6Tq2TW
JYXu7gQCfA88mBAT4Ve28eCLtERI5PxyORJ5uufN24lfeHUVuUR4NydA0hBgGFu+
yIK15xYKVebHuG7H2s3WhkmPCgBgAAGGMeSLNEUiaz9WKh3nul2ieWtxgxkApAoC
DGOlO3j5rrWfKBYfrfVapZoivaRHBQDw1xBgGBM9kYZIaO2/5XIvymYLRIbZF8Ez
zgAgnRBgGAeByGZr35rLvS2XG7x5tyVi0nGVb5GImGtKbTQm6bEAQIogwDCqBnXt
iyyJRNY+L5N5VzbbtNYnSlXoyswPazQuj+O1Co8BAoC/QIBh9AiRiIREdZHQ2uMy
mW+Wyx1rl6xN4VOuCszEXGR2cG8bANwNAgwjRogikYZIj+jRWn9/erpv7Z1x3El6
YNs0SO7gPR/ILwDcHQIMI0OIrEhItGTtwxznR9VqmtM7UGQmohyRh+kvANwTAgyj
IRaJiZZE1il1H8f5UaWy0Zh2Oi6z2oES0UNbrcvjeD1OAAPAPSHAMAJikc0ihyiV
JbqpVgus3WhMO+lR7ZQiKig1w1xmdjEDBoB7QoAh1SKRiGje2vs4zu+q1cDaTca0
Uj/xHSgy325MgPVnANgWBBhS6q70Psp1a0RXVyqbjWmOSHqJSBPVmI/z/UujCOvP
APDXEGBInUgkJFqw9ijX9ZX631KJiRasHZWJ70CB+XprQ5FppbD+DAB/DQGGdIlE
FkQe6ThKqc8VCmuUqou0ZaTaS+QQTTG/0fe/h+kvAGwHAgxpEYkEREvW/p3jfLpQ
2E+pusitdvTe3zuo79XGzItMYfo7usRy1OcoYBMSESktTta6GdJu0iODMYEAQ/Lu
Su+jHecAz/vHTGaG+VZrU/VEyeXLMf8mjt/e611hzAzqO1I4CjjqqSgkIrIknlt/
5IkqaBGROBlv4zWlP/zCZkiUFjdr3SxpHEJh92HvgSTFIl2iurUPc5yHet7TPe//
Oc6StUsiI1pfh2ia+X+j6BdxvB7T39HBUaDCntMMNz39ZNVrEwlbYwpTzaf9K/Xa
RERORt94eTRznvXy4uWLf7gwd/O1JqvEzVkvR0on/R3A6OH91s8lPQaYULHIksj9
HecJrvswx3mM4yyKdEf8BYIloj9Y+65u9w/WllHfUcBRoMKu04w2Hf8m1WvUn/0R
6rWJhEQo7FLYo7hPRKQ0eXlyc+S45OUzV347e+PlpjBdu+xct3GnyVXQYNhVCDAk
IBbxiRrWHuw4Z+ZyR7rugrU9opFOLxE5RHsr9ZEgeHu3i+nvSOAocJrNzU98AxM1
//5kCrsU+hQFO/lr2iE3R26O8pXMld9Zf95rld+2ubz1CoRr7mDZEGAYKiPSIWpY
u59S78znp5kHE98o6YHtOYdohvm3xryt1/ujMZj+ph9HPafZWjzmpfUnvZ2sof4y
0nsv2qXiTO533yZj1l5wCoWBzZWJ0WBYFgQYhsSKdIjq1laUOief10RP8rx5a4PR
n/gOFIiuMOZNvd6txtTw7MnUG9R3/nGvaB7zahKivr/1HdO7SLvkZKg061596d7n
vIiURoNhmbCXwDCISFOkL3JJuXxuofBUzzvCcW6ztjMu9XWIZpRqilwbx1Oob+pt
mfs+7hXNo/esvkRkIup3qH5bdPAjbnv5l1TYV702jdiN65AMXS0Vkx4DjLmWyGZr
+0Q/KJUe4boV5iWRPtHI3eC7TZqoyryXUlcZ84ZuN2bOo77pxlEwqG/96FcSM4V7
UN+7iKWoZ+cObh129PQPvyjKiJMh7AmwQ1iChlXUEmmKWGP+MDV1uzEPd5zNI36R
871oomnmPxvzQt/XRDdYW2N2cNhNscFVV4vHvKz+uNeSEIX+Ss5WlUOlWW7eceC7
HxHV1iDAsGO4DxhWS0ukbsxvKpWeyAFKlYnGrL6KaJp5o8hTO50FkSnmKWaNY266
qbC7+YlvaD3mVRQFe7TyvE02pva8rLuvF5AJWjZXWckvDmMH54Bh5bVFbrG2HseX
lMuHOM6+Wt8xFncZ3d2gvk2Rh7VaTZE55izqm3ocBU4zYkcTycrXd8DG1Fm4+iNX
aX8XL6iGyYMZMKykjsiSiBhzSbV6H+ayUputHYNbjO6FiaaZY+bDmk0SwcR3VKiw
u+n4NzWPfi11l1alvgP+EtX29kIy3abNYxIM24UZMKwAIeqI3GLMYhx/rVBoTk8f
4TjMPB43+N7LoL4Z5tmlJRaZRn1HxGD6q/odivu7fL/vLrGGwu7V/3GD7garmHkY
fZgBwx4RIhHxiZasPbdYfEEm07a2IdIftRcILgcTTTHXmJmZFxaKStWYFeo7Ijjs
bXrGm5pP/GdaunnVN+YvUba0dRJcXfXNwWjCDBh2kwweayWy2dolaz9cKDzX8241
ZqPI+P3aP0jvHNGUUmphgRcXi0pNKYX6jgwRFYbc61C/s7rT3wFryfGuPnted/ur
vi0YWZgBwy4TEUPUIwqsDYhOyeffmct1rL3d2vE72DCRJqox55kzS0vEXNR6SimE
d7RwFLAlFkthdygbFPLrZGIdUmQtHhAN24TdAnaNiPSIOkRE9LJs1kxPn5zJ3GrM
nSJjVl9F5BLNMCuRDNG+jUZOqX2UmkZ9RxDH/YW/f3X92R+jqDekTYqlwDdZUr3m
kLYIowYBhuUSkUikLdIker7rLtZq78vlbjPmDpExu99CEXlEM8wxc5/oqE4nX6/7
ItM44zuyOOrrbpN6jWGsPw+IpULlug9fr3vhkLYIowZL0LBzQhSJBEQB0cFaH6z1
vxcKtxsznLW8IVNEFeZ5awOR53U6vzamwLyXUni+FewaEeq1iFihv7AdCDDsyCC9
faKWtQ93nKpSF5RKg9O941ffwcR38KjqV3S7P46iAjNe6wsAqwQBhh0JRTZb+2TP
6xFdXCp1rL3DGD/pUa24u9J7m7XXGPPZfv9ncbxOKQ/pBYBVgwDDtvVF+kR1a5/s
uheMb3qJSBGVmeetvdyYb4bhl/r9AvMaZtQXAFYVAgz31hcJiBrWvsDzNhN9q1S6
05hO0qNaDYooQ1Rmboq8PwgG6V2rVAbpBYDVhwDDPfRFNln7As/rE51TLAYiY1lf
JsoSlZnbRGeH4fXGfKnfn1Mqi/QCwLAgwLBFX6RH1LT2uEzm9ELBEZm3dnXeF5Ow
PFGRmYg+GgQNkU8EQZEZ9QWAIUOAgfoiXZGWyPM9b06pV2Yyocj8OD7MmYiKREWi
fw0Ch+gjvV4J6QWAhCDAEy0U8UVaIs/0vPs7zj+47nqtm9a2xrG+g4lvRamXdzqf
7feRXgBIFgI8uQbT3Ke47mNc91GOc5BSDZGN1tqkB7bi8kQF5qpSz2215rT+LE73
AkAKIMCTaPBEybbIo1z3Pfn8/sx1kY0iJumBrYY80V5KvcD3C8xfjqKKMagvAKQB
AjxZYpGWSFvkYZ73T5nMOqXWMY9renNEZeayUq/vdL60dc05w2gvAKQCAjwpYpG2
SEvkMMf5VC6XV+pwpeoijXGs713pPanTuUXkJ3E8rVQe6QWANEGAJ4IRWRJZr9T3
i0VDdF+llkQ2jWN6s0QV5rJSb+50fm7M76wlkQKzR4T6AkCqIMBjzoo0RVoieeaL
SqV1zOOaXiLKEq1X6p293gVRdIe1RqTA7OAdggCQSgjw2BKihrUtESKar9U2WTvD
PK7pzRBVmctKnd7rfTwIHCKkFwBSDgEeTw1rmyJkbTQ9fZO1WaIC86LI+N1i5BHV
mMtKfTYITu71XKIsc4EI6QWAlEOAx1DD2qa1/tRUk6hHpJk3jWN6XaIac0WpC8Lw
uHa7pJQSyTEr1BcARgECPCYGD65qijStJWtvnpoyRIHIWD7MmYhcorXMPzbm2Ho9
q1SFuUSkmHGlMwCMCgR4HIhIU8QXia29uVbb13Ea1s6P4+leJnKIppiLSv3OmGOb
zYpSZSJGegFg1CDAo82KWKKWSJvot9XqA7RuWHuDMeOXXiLSRFPMQrTZ2sriIilV
07qE8ALAaEKAR5UVMUQtEY85YP52Pn8w843GxEkPbDUoIodoVqlb4/jQRoOUqjpO
BeUFgFGGAI8eKxITtUVyzDnmTxUKJ3hew9pNIuNXX02kiWaVus6YHNGhzWbFcapI
LwCMPgR4xFiRDlGWaE7rN2cyJ2WzTWtvNiZMemArThO5RDNKXR1FLtED6nVSqqIU
6gsA4wEBHhlWJCLqiGiit+dyr83lmtbeYkw/6YGtBk00w/x7a9vWHtFuC1HFcSo4
3QsAYwQBHgFWJCTyRbLMR7juI7V+ZTZ7qzFB0gNbDZrII5pR6po4fnKnU7e2zFzB
M60AYOwgwKkmIn0iX4SJjnHdQ7U+NZ9vWHunteNXX02UIZpW6vIoutraN/h+X2Rv
pTTSCwDjCAFOLyHqEvWITvC8qlIfz+cb1o7rxNchqjH/No5/GsfvC4Kr4rjCXGNG
fQFgXCHAKdUT6Yp0rH17Lvf+QqFh7W3G9JIe1WrQRDmiKaV+H8cn93q/ieMK83ql
XKQXAMYaApw6PZGeSNvaf83nF4jen8uNd3prSv02jr8UhhdH0e+NQXoBYEIgwOnS
E9ls7Sn5/EZj3p3PL1l7m7XjV19FlCeqKXWtMWcEwS/j+KIwrCg1w4z6AsCEQIDT
oifii/jWvimXe1cuVx/fNWdFVGW+09oPdLu3WHtBGFaZ1ynlIb0AMEkQ4OTdld5/
zedzRK/IZO4wxk96VKuBiYpEVaWa1p7c7V4cRVWlkF4AmEwIcJICkY6Ib+1b8/l9
lXqW52kiX2Qsr3MuEVWYI+Zndzqa6HtxjPQCwCRDgBMTiGyy9g3Z7EO0fpTnlYja
Iv0xfX1vmblM9CTfrxBdEIZTSq1hRn0BYJIhwAnoi7REuta+MJN5Sy6XJ2qLLIxp
ektEZea81kc2mz+JommsOQMAEBECPGShSFOka+3zstlXZLNrmR2iRRGb9MBWQ5Go
wpzX+m8bjWnmn2HNGQDgbhDgIbkrvU/wvA/k8wXmaeaWSHsc61sgqiqVV+qRjYZh
viKOp5Vai1uMAADuBgEehkhkXuQo1z27UDAiU8xNkaVxTC8RFYjWa/3YZvN2kRuN
KSq1TikHLzICALgnBHh1xSJ1ka7IoVp/rVAgoub4rjnfNfF9brv9ozguKzWjlEZ6
AQC2BQFeLUakLuKLzDF3pqc3GmOIGiJjeaVVnqjGnNf6+a3WN6NIEU0plUF6AQC2
DwFeeVZkScQXIRGZmakbU7e2R+SPb333UuoVvv+ZMMwSFZmzRAr1BQDYIQR4JVkR
IVoQCURkdjY0ZtHaBpEdx/IyUY6oolSR+Z3d7mf6/RmlckSM9AIALAMCvDKsiCVa
ElHMgbUyM7MYx0vj2F0iUkRZoqJSBZHTguAdvq+VWqNUFukFAFg2BHgFDE73Fph7
1srMjIgsWFsf0/pmiQrMlugrYXhSu62VmtU6j/ICAOwiBHiPGJGYqC6SZf5zpVJm
XrS2Pqbneh2iDFFFqa/0+//YbmutZ7QuIL0AALsFAd5Nd6V3b2YhurRcFqLrrR3L
+4sG6S0p9WdjfhmG/+j7045TRHoBAPYAArw7BmvO+yhVYv5KsfhgrRdFxvXuXo+o
ptRv4jiy9thGw9F6WinUFwBgDyHAuywWaYjMKPWZYvFIrRdFbrQ2TnpUq8El8ojm
lPpuv//UTsdRaspxSsNKb2SkH1NotrGgz0ye5oxDrsbvAQAwqhDgXWBE+kQNkX2Y
P5nPP1ipm6yNkh7VanCJMkRzSn0tDOeUeqrvT2k9nPQOutuPpdOzjz3MyyiO7b0L
7CiOjFx8TVjMqYyDEgPASEKAd0GPaINSR2l9guse5TjzIuNXX5coS7RGqfPDsEr0
7E7HYZ5Samj1nfftow7w5kpqvmP/64WV4pSiv17ZV9Rt2OM+35wtqo0tc+m10VSR
sw4jwwAwQhDg5RqsPB/juh/M5xesHcv6ekRrmL8RRZroxb5viKaUKhIN4f7eyEgv
krovR+zrfO6E8j77aOpJ0Jf5TeavJsCkmEoZvvgVVcrx9TfH//Z//i1188Pro1qB
cy4yDACjAQFelkF9NzDfT+t5axtjd5+RS5RjXsN8QRS9pttti1SZC0RqiOk98iD3
Aev0iUfk9qmpxc0m3v4lbVaoGUgzEKdNe1fUuS+qXHFT+Llf9q68w/7ilnA6r9Bg
AEg/BHjnBvXdW6kP5vOPdZz58aqvS5RnnmX+ShjeYu1Z/b4VWc+sh7XmvNi1D9nb
O/pA5ymHZf7uPl7g28Wu3UF97y621AjECc19Z53Tn1m54sboZV9v/35jhAYDQPoh
wDsXEh2k1Kn5/JFjd97XJZph/m4UXRnHX4+i66ytMleGW9/D17ifeGrxbw90A98u
1nc08d2eQYbd0Dxob+c/ji+++psdNBgA0k8lPYC0G7zQd06px7vuONXXIaoy76fU
ZXH8L0HwgX5/QWQdc2lY9Y2NLHXlkFn3U88o/u2+zlLdNAPZjfreJbK01LEP3c89
4/jiYWvcxa7EZqzWKgBgzCDAOxERHarUP+VyC+NS30F6Nyj1W2Ne6Pvv6HZvN2Y9
c4nZGdY9vrGVxa4cOK3Oelbpofu5Sx0brcRDTAYNPmI/94vPKT1oL903K/A1AQBW
CZagdyIiWqP1Ma57kxn5w7kmKjFPM/8yjv8xCO4Q+U0cTzHXhpjegTCmw9fqLz23
fMgaZ6XqOzBo8OEb3GpGL/hxRisHC9EAkEoI8I7EIk2RBWv7oz/91URTzDdae4Lv
95h/HUVTSq0benppy/TXbpjWh+zjLi7uzknfHYsshR37wScVb2qYG5fMTIEchQYD
QOpgCXpHYqK/UerL+Xx7lK98VltP9/aInuv7P4njm4xZp1QhifoSUWTofmudbzy/
FPnLvdp5V7X7cv/1+rsvKe1bVQu+/PWztAAAEocA70hE5DLf13VbIxvgCvN+ShWY
D2o2n9BqDdKbTyi9RBRbWfBtN5J91rmtYFi/Oq8AACAASURBVLX+VWNLC77df8r9
wUnV2YJa9GUbj/MAAEgUArxdg9t/B+vPo3j6t8K8Qak1zHP1+t81m7dau1FkOrn0
DsSWDp11fvem6aizWtPfAWNpwbfry+qqN04VMrSABgNAyiDA22WIDlfq9+XyKE5/
K8xrlKrW6wc2m02iRZG1zNmk60tExlJobGZGNVZt+vuXbQkt+FLN88J7Z2oFhf4C
QKogwNsVE/WYi1qPVoDLg4mv1nvX6yFRT2QdcyYF6SUiY2XRt/O+UNcOJ4dGaMG3
5Kg76/GCb+1I/SgBYLwhwNslRIPj9WoulK6kQXrntN7QbOYXF5dE5pjdYT1YYzmM
pYNmVOMTc2FveCG0QpFv5d/XhqHM+4IGA0BKIMDjgIkqzHNKHd5o8Pz8krVV5hlm
PYTXGO2KWKgREDE1hhhgIqp3xfSsfHZ94Nv5DhoMAKmAAI8w3nqL0QzRGuZHNZvX
i6zTeo1SLrNKWX2JiIREiISGHEAhWvAlbsTyRTQYANICAR5JTKSJKsxlojLzs32f
Fxd/acxaZo+ZiVKX3hRY7Eq8OZaz1/YNzXdwUTQAJAwBHj1MVCTKElmRN/Z62cXF
H8TxWq3nlHLTN+lNlcWumLa1H12Tdag+3GVwAIB7QYBHCRO5RFVmj/mUIFhbr389
DOe0XqtUBuldnkVfKJL6B9b4HYsnZAFAgvAs6NEwSG+BORK52ZhvRtFZQbBG6xy6
u4uEaLFrmWSmpjZ37GwBrw0GgGRgBjwCmChP5BBdacxZUfSQZvMjQbBGKdR391ih
Sk7d9LaZg2f1fMdGeG0wACQBAU67DFGVuch8Vhg+ttl8f7c7q/Uc6rsHrNBSV3Ie
/eCk6v3WuZvRYABIAgKcXhmiGnON+VtR9PkwfNfW9OaR3j02aPBUTv3PieWH7ONs
7tgQDQaA4cI54DTKEOWYZ5Q6vdfzmF/V6eSUmh399Com12FyydMUJv2CCyO01JX1
ZfW151Ve8rXOZbeE0/nR/ucFgNGCGXDqZIn2UuqzQXBWELzB90/udme1XjP69SUi
rSgy9O0f9Wq5VOx4RmixK3tP67c8Ottp2zDGJBgAhicVx0G4uxrzh3q9f+52X9/t
zoxLege0onZgX3NBm8oqk47FF2Op3bXrS/qow7xNHTQYAIYHAU6dolLv6PVqWs8q
VRiX9A4o5kqW+xGfe6lfzaZl3+sbOmxOn/PM8uMPRoMBYHjSchCEe8mN6eMkHU2t
vv34T3tUUdnUTIKXerKhpj7xtBIaDABDgwDDUCnmcpbn23LOJd1Kyhp8wLT6xPGl
Yw/2NqPBALD6EGAYNldTO7Rv+27njB/0UtfgmjrrmaWnHZ7pIcAAsMoQYBg2xVzK
cGjkHRf6H7yoW8qkZa190OC9pvVeFd3ARdEAsMoQYEiAVlzKcBDLHxdiVdG5dEyC
ichY6nXl5EdlH3tYBieDAWBVIcCQDK24lOWLru7/x0WdckWnZCGaiPxQ9q7o855X
OeogbxOekAUAqwYBhsRkHepbesdF/ie/71dS02AjtNSTWpb++wXlh+/rbW7jSdEA
sCoQYEjM4GSwYnrbd/1P/7CbngZboXpP8h5ffFL5gXs5m/C2BgBYBQgwJGnQYE/T
6y5on/OTbqWiU/KErEGDXeZfvXXmYft6kU16QAAwdhBgSNigwQWPX/rF9icv9XNu
Wi6KHrwxiYy9rRHPt22MSTAArCgEGJK3ZR6cp/VlnUnNncFEJERBV25878wBM+rO
to2NCCoMACsEAYZUYOZSRp1wVuPzP+xWimk5GUxEzUD6bbn+A2v2rTqbfBtbNBgA
VgYCDGlRzPB0TZ94buuLlweVfIpuDm70pL9gbvzgzN5VtXnQ4KSHBABjAAGGFClm
eLqmXvCFxvm/DcpZzrlJD2irRiD9BXPjB9ZsqKol3xISDAB7DAGGdClm1FRNPfvs
xld+GxRcdlOzhw4a/OdPrA1CwkXRALDnUnN4A9iqlFFTNf2cs5vXbI7Tc1E0ETUC
oYV4vxl1Zwt3BgPAnkKAIY1KGdYFdd2SzXmUT81CNBEFXbnpA2sesJdzZwtPqQSA
PYIAQ0pVs+q4Tzc+/6t+KU0ng5uB9JfMb9869f/28za28LYGANh9CDCk1NaLopvn
XN4v51PU4EYgfV9+9rrq0ffx7mzbPhoMALsFAYb0GjT4pV9sfvayfrmo09PgZk/C
UC55WfX4+3mNHgIMALsDAYZUGzT45ec1P3xJt1xIy+4qRI2eiMhZzywHbRtgEgwA
uy4tRzSA7Rk0+C1fb1A5RZPguxp83EOym1o2iNBgANg1CDCMgKLHlNOnf6tVrur0
XBQdC5U8de6zSy86IrepjQYDwK5BgGE0zBTU67/Zee+3/VJqGixCzUAyDn/sKYUT
j8igwQCwSxBgGA15l6eL+pSL/Xde0E5Pg61QM5CCxx96UumlD8U8GAB2QWoeeA+w
Q8xU8IiIT/1+z1h63/ElaphulPSwtja4kuVTn1AgorN/0ZsrqWyaHuAFAOmEAMPI
YOaCJ0T84R/26j351DNK3XQ8lPmuBn/0KcViRs76RR8BBoCdwhI0jBJmznuUc+nH
N4RUUwUvLZ0bNLic4wOnXB8XRQPAMiDAMGIUc97jW1ty8ueaxaoqeEkPaCsr1O7Z
5zwwc+KRedyYBAA7hSVoGD2uIlfRGZcHEfHpzy5Rw/hh0mMiIqIgpnKGP/m0ApH9
3C/6cxWVddIyRweAtEGAYfQwc94VIj7r5z1F9PHUNNgKtfpSzvCnn1GKDH3x1/21
ZZVBgwFgW7AEDSOJmfMuFTJ85s97J5/fLqbsxiRj+T//ofLMB2Q2tvC2BgDYNgQY
RtWgwaUMf/T7nZd9oVXMpGWiKUStQAIjXz2p+uIjcggwAGwTAgwjjJnzLhcLerbE
XNPpuSh60GDqyq2NuN60/VgEFQaAe0KAYbQxU97lD1zsv+ncZrFExTQ1uNuXS06e
etqDshvbtm/QYAC4BwQYRl7O5TVV/bHvd9/6Tb+QV0WPUxLhdl+6S/a/X1N7yuHu
oo95MADcAwIM4yDn8mxVf+gS/5TvtgtZKqSrweZ/XjP9uEMy9R4aDAB/gQDDmMi7
PFvR777If9dFvqsok5o77AYN/u7rqn9/aKYVWoMCAwARIcAwTvIez1b0v32rc96V
/VImLZNg2roW/T9vmZrN6SAiJBgACAGGMZP3OFfWSx1LxMWUNZiWzCFrdL1nAixE
AwACDOOn6PHJF7TffUmHiFLVYL8nF7+xdsL9sgu4IAsAEGAYP1sWoi/0P/QDX6Wp
wZ2++HV7/knVZz0gt6mNJ2QBTDoEGMbQXQ1+/w98R6WswUv2yy8vP/9vM5uaeGMS
wERDgGE8DRr8ngv9f77IN0I6NXt6JxR/yf7nS6uveFh+sWuTHg4AJCY1hyWAlTZo
8Icv6kSxpOqi6E4ovYY988Wl2Lc9TIIBJhUCDOMs77FbVKf9KPAcLqXmbQ1E1O6L
dOxJR+c3N0wXDQaYSAgwjLlaTp36f+1X/lc7l01Rg4WoG9IZJ5Tf9rjiPBoMMJFS
87gggNWRc3lNRZ/5k14Y27NfWKWWbfdTUTs/FGZ6/+PzmuR9/+fPVnQ+NW+SAIAh
wAwYxl/O5bmKOucX/eee3crXVEremCREnb70Yjrl2MIpTyjMN003TMVvBgAwHAgw
TISsy3MV9eVfd5/67/VCIRUBpq0Njiz9y2OKHz6u1AwQYIAJggDDpMg6vKaiv3VV
SFM6VSeDO30RlodvcMKO8TEJBpgYCDBMkIxDXoaeftpiPn0Nvt+c+97jygsNNBhg
UiDAMEGYuJZX37o6fMLH66lqcGSImd5ydO4DTy0sNHAyGGAiIMAwQZgpo3m6oC69
vv+Ej6doHixEfihRTG8+qvC+p+CCLICJgADDZBk0uJZVl14fPTF9De7H9PbHFE55
IhoMMP4QYJg4zJRxuJZTF14T/r9Tl/LZVASYtja4G9K7ji38y7GF+SYe0AEwzhBg
mESDefBUQZczQtO6nI5JMA0uyAqlE9B7nlH+wFNK3RCvDQYYWwgwTChmyjh0yZ+j
x75vMVfRldTMg4nID8V2zJ1t47dMN0KDAcYTAgyTy1U8V1TfuzZ8wicXswWuZFP0
xiQ/lE88u/zPTyou+oMGI8IA4wYBhsk1OBm8tqwuuiZ6+mebWZfLqWlwN6J23bz3
+NJbjio0e9KNCA0GGDMIMEy6QYO/+bv+s7/YZJJCOp4UTVsb/MFnll79iFy7jwYD
jBsEGIAyDs+V1fm/DE7+jl/IskpLgqkbUadhPvYPpVc/LNs3EpqkBwQAKwcBBiAi
yjpcLitjKYylkqYG+yF1GvYjL6r83d5uNxSLSTDAuECAAbbIuXzmz3ov+2qr009b
g4Xq9qAZ3bfSDQkNBhgPCDDAFlmH58rq3F/33/xtv5WyBrcDe+bzyq9/RD6IpBvi
ZDDAOHCSHgBAimQdniupz/2yR2RPfXKpmuVmIDYFsetGRA3zwWeUHEUf+XGPiAue
cFou2QaA3YEAA9xD1h00uB8Knfr4UjlLvSjpMRHR1ga/7/iSIjr1+4MGExIMMLqw
BA1wb4MGf+mnwR82RuWsSs9CdDeidsO85/jSe5+Y9yMrlIK5OQDsLgQYYBuyLudL
6mu/6y91pZpL0cngQYPf+vRy4IsfEs4FA4wuBBhg28pZPvvy4JXfaHcCqabpgqwt
54OPKy41jY9XFgKMLAQYYNsyDq8tqa9eFfzj11uBkWo2Rdc8tXz7T48v/sczK4t1
0+mjwQAjCQEG2K5Bg//r9+FzvtgRlmpqnhTdi6nVNK86OnfeieXFrk16OACwOxBg
gB0ZNPg71wRHn9kMDanUfGJ6EXXa9jlHFqiHAAOMpNQcTgDSKuPwupL62R/DYo5r
qZkEE1EQCfmWSNp9NBhg9CDAADvnOZwtqUd8sq6zXM2lJcFWiIQued30UhengQFG
DwIMsCxTef75zdF937fklXVKGixEnb4cc5iHVWiAUYQAAyyLq3ltSV27GG/4182Z
aV3NpqLB/ViaHUskzQANBhgxCDDAcrma1xbVbXV7wFsXMjOpaLAVquTUn06ZbWAV
GmDUIMAAu8DRvLakbmnG5ddtyuRTEeBOX/atKgoQYIARgwAD7BqteE1Bucw069RS
cDLYWjFChOdCA4waBBhg1zCTo6kd2jWvuNOr6TQ0GABGEQIMsMsU85qiqndl/Vs2
ewWeyqfo5mAAGBUI8HYxkWIm/BvBtjiK50pq3rcHv3vBdXkqn6InRQPASEBctssh
yol0jSnjyArb4iieK6obluz9P7xkhcsZ7CdwT0qT0kkPAtILAd4uTfQHa+/bag0z
wIMPq0byR4Sjea6ofnd7fNQZS9l8Mq8sdDQ7ioiwz+wQs2hNyiEe1kGPFXk5Z/P1
JjekDcLIQYC3y2GuMFeYM0PMYY35hjjO89AOErCnXM0zJVXIqMC303nWw+2gYsq7
/ItbY8K1YDskblb3mtSaJ29YPWRFvdZ9Pni8zeWHtEUYNTjO7wgTecxE5Axri1Xm
A1stI4Kj6QjxNP3w+vCYzzY2te3UcBusmNp9e9RHFnEx9o6Jm63+5L+nLjyV3GEF
2MlQcdoQ2WxpSFuEUYMA74hL1BO5IopKw5oEM3OBucyssAo9OlzNs0X1i1vi55zX
vq1pp/Ksh/XByrlcKioiVc7is7wj4ng2z6YwTdnSMFahmSmTy175Xaw/ww7gQ7sj
DtGfrD2p15thHsIk2CMiogzO5o0gT/OaovrJTeFLvta+qW6ncsNoMDPlXT7v8oBT
8Eyu9LNu1tv4J9p47TBWoZ0saW/Dp19t8sVV3xaMLAR4RxzmKeautd+P49WeBHtE
c0r9bxQp/FRGk6d5TUH94LrwZd9oX73Z5N1Vj2LeJcrw886pT+Wwy+ycuNnqby6u
/OablKus+sXJuXL+si9FOZJMYXU3BKMMn9udcIn+YO0p3e5qB7jC/JUgOKHdjkTw
UxlRnsNriurSP/Yv+EOYzw8uTl5FxZw6/Xs+53URd0AtgzheXMpmb7tKbbyGnOwq
bsnNUXF63//8Z5PH2V/YERzqd8JhLjPPKJVh9lZtKxmiqlLvCUMhwgngkeY5XCmq
n98aXbcxns6r1WtwwSMq6tef35zO41O8XNbNlX57aeG6n1G+uoqT4EKt/K33RTmS
DK5/hh3R1RJOUeyEJeqL7K3U/bTuE5lV2MQU8xf6/e9FUYbZQX1HnFJ01R3xVXdG
D93X2aeq+zHZlX5RQt6lUk1/8Nvt790czxYQ4GVTmtioqNudu48UpslEK78JN0dr
Dtz/vU+IqiXS7sp/fRgj+OjuXIboJmvf1O1+J45nV3oenCGaYq5qfXa/3xQZ2v1O
sHo8zbNF9eMb41d90//z/KpcFF3Kq5O/2nnb/3RmMP3dRTaTL/7hMm/hJqrtRdnS
Cs+D3RzV1pe/+paoQDK0+51gZGEGvHOKOUO0KPKLON5X6weu3Dw4Q7RWqTP7/a/2
+1fGsWJ2Mf0dC1px1uU/bo6vuCN69AGZ9RUVRCvzvsCCS1N5Reudx5+2OF3WeQ/P
oN5FSgsb1e9kr74knDtIquspClfmZY5ujmrrq994x8wlZ5hCmbRD+NnADiHAy6KY
va0N3qD1A7QOiOyefc1Bfc/q99/f7/8sjpk5Q4Sj6dgYNPhP8+byW6NjD8msLa1A
gwseF2v6JV9uX3pFcNWmuJLD5QK7Q5TOLN6Sv/GqzO1Xtg9+NJVmKO7vaYO9HNX2
mvraP039+HOmUBI3h/rCTiHAy3VXg38Wx/fV+gjHIaJ4dzM8qO85/f67u92+SI05
w4z6jhmtOOfytQvmh9dHz31wtlrVLlFodvNIX/CoWNVvPL955mXB1ZtM0VOOwi6z
W5QWx7OZTGbh5sKNP28dfiwVarvfYDdHhRrN7F/7yuumfvZFky2ivrBMvN/6uaTH
MEqMSENkH6WqSr0jl3u01vMi8a58BY+ozFxT6gv9/tt6vZ61JWa8fWGMRUbmO/Zh
G7xOX370ykrG4aWu7NKRPu9SKaeoqt7yn+1PXdbNu5x3sViyEkzk+Ev92YNue/23
KVeibp167V3LsJuj6rqp817nLd2Wvek35LqoLywfArzLjEhI1BCZZf5mqXSE1osi
TZGdnhV2iSrMNaXOC4IPBkFLpC6C+k6CyEhoaKFjD5lV17xrDblEXRv0pBns5Fif
c6mcU1RRbzq3ednN8R83x47ivEdYeV4xJnb8xWD94Y6/dPO/XUFxTN0GBa2d/0U3
S7kyVdbXzn9z7UdninLFy4t2UV9YPgR4NxmRushapXpE3ysWD3W2XL8ciTREGltn
OJqoylzZWtmLwvCkbjcmqltbZnbw5sFJEhnZ3LH3W+e0+7YRyPwZa8kX6tleV1r9
e5Q461AlpyjHVFDvPq/15SuDO1qWhAoeOxr1XWkmZhvrTjNcdx+ncceNn2pR3ydr
qNeiXpP6nS1/TLuUq1CusuVJll6+ct7ry1d+W3cWiUm83PBedAjjAgHefUYkJqqL
zDFHRIOF6LpIND1Nd1tivNXa+zQaZeYMkSFqilSYHcJhdBLFVmJDsaUl3x6yxpn3
7fqyuuq0Werdcyrs8ucv7LzmvztrCqoeWGupkGFHYZ9ZTTZma3S7Ea7ZV3ebZE1c
XXfbqddSz9/yB5yM97PP733uq6yXJyKbyetek8RaL09Ko76wGxDgPWVEDJHdeuKo
LrJGqfrdZsA55pa1ZWYm0kQa6Z14QmSsGEuhkXZfpguqfs8A51wSodhQ0WOtSCO9
Q2MNW0PWsFju+6ZQUb0ty9HiZJiZo3AQYGIlSpNSSC/sNgR4hRkRe89LozWRQnRh
W0TIili596OymEgpUozdJjkiJJbE8t1Ws4QVigsrCE9eWmGaeZVfswLjgxk7TFox
E2sivdJPEQX4C/wqBwAAkAAEGAAAIAEIMAAAQAIQYAAAgAQgwAAAAAlAgAEAABKA
AAMAACQAAQYAAEgAAgwAAJAABBgAACABCDAAAEACEGAAAIAEIMAAAAAJQIABAAAS
gAADAAAkAAEGAABIAAIMAACQAAQYAAAgAQgwAABAAhBgAACABCDAAAAACUCAAQAA
EoAAAwAAJAABBgAASICT9AAAIKVERESIiJmZOenhACTJWjv4Pyv4ccAMGAC2QcT2
w66QKKXCsJf0cACSZK3xu/VcrtwLOkSyUl8WAQaAexOx/X4vjqPjnnTyk5/4+igK
kh4RQGKsNb7f8Nz86af91nH0YFloRazYErSIGBtbExtrdvXvauVo7Sil93xeLyLW
xmbXh6GV1tpRytnmGESsMbExsRW7jC/laO1ove1/W2vtYITL+VIrhYm11kptd1TL
JCLGxNbuzk95T2jlDMa/h3vI4IdobLycP6yUGvwomYf0e+ou7WarysQhMf/9417+
94992fn/9Z4oiojIWjP411vmAYiZ7/poL3/Tu72P7ephZFe/HSJSrJTWWi33O9q9
o+KOj0W7YU8Ozitit4/wW/YEY2TlJp27IQj8SmnmrSd/nYh9v1MpZ1bqK69kgMWY
/fd/UKezNDiILOdvae1o7ZZL09f++efZbGlFhmFMdMD+f9vuLO7SMErFqZtuusrL
aKJt7CLGxJlMbmpq7263ZXe4EzvaKZVm/nzd5YVCdZt/wJgoX6hWSjO9oH3XSYXV
ppRmVrfcenWxUNnBLwc7Za1RSu+7z+G+3zAmGtrnuZCv/OnPv8pls9rxHO1o7e7e
1wmCzqGHPKLdXohNtON9QyldyFfm52+J4r7jDCnAxsTZTGFmdr9ut2lMvOM9bZVH
Ev7N4Uc//piTGs3NUdQf/McoCubWHOC4Xr/fMybacbcUcyZTiMLewtLtmUx++ZsW
scy0/4YHdvy6MZExO/9H2L3DSBgF69ce5LjZMOwZExmzoxIrVlo72Wyh67eWGnd4
Xm6Z34u1Zt99/qbj13flWDR9w01XZLdzLNoNIiJxvO9+9xscnFfka+6SLZ/fXE5r
d5c+v2HY23uv+8amH0X9FZx37qog6Lz6FWd5Xq7bba7sKFbyIqxSafrE55/W7TX3
3/eBfrexnL9SLE5de+1ltan1bz/lqJUaRi5XOfEFp7X9+oEbHuT7Ox+GVk6pNH3L
7Ve//0PHi2x379x/w4NOfP6HK+XZbrdlZdsHhcFR4Iabr3znu4/ZwRbve+gjnnPC
u/L5Sq/XlqHMdRTrRuPO0894qaPd62/+XS6bcxxXa3c3Sjw3t+HFz/9QHId7rz+0
22utxmj/Wj5X/pd3H+3oTCaTveoPl+XzWUe7g29hV7/US174kcWlWw87+JHtzuL2
/oxSOp+vNBobP/v5111/wxV7NvZdc8ABDz7uKW/23OzM1N5B3x/mpu/OmKgXdJqt
zYX8PX6PPPqoF23Y7/77rD+UWYVhb3vzEmbOeAUj9sKL/+M7F35yV7deq6x78fNP
C/r+fvv8Tbfb3PEfHhTrT9deVq7M7uph5DGPPvE+Bx3RbM4fuOFBxhq7naURZs5k
CmHQ3bx48xVXXnzhxZ9a/ibK5dmXnXj6+rUHtTv17X39u9ztWPS0lT0y5Iu1l73o
4+vXHdzpLC1zBWglt56rvO2dj8xmCp6b+f01Py8VS8v/8D758a966BHH94JOgudB
+v1uEHR8vzFVW7+yX5n3Wz+3Il/Iiu0HfqvdLZfyp73vF4tLty3nb01N7fXiV90/
61C5lM9miyuxBG17QafR6GUzdPqHr6g3Nu70rxQLtRtvuvLTn3+tZpXLlba53mhM
1O7UD9jvAScc//Zadd32joyl4vS1f778k59+Tbmcz23nN/E4Dtvt+v0OP+ppT3pj
vlAdzuUtzCqbLRTy1fmFm8/98tuLxalGfdOtt/8xny87jqvVcjNsrQkCv9HsHbj/
gW990zfqjTtXddh3yWTy2UwxlyuK8KkffXqxWLM2vuZPlxWLU86uNDgIOn7X7wT0
pc9cs2nzjdv7Y9lscWnxtvO/8Z5b77imWKjt4br98hkTB0Fnsdl/1EMf94Lnvr/V
XhjOdnegWpn73+995sL/PbNWmwujIAj8Zjs+/kkvffRRz7fxdhfzHccz1l566TmX
/vjLlXLFdbPL36K1Nvj/7d1nnBRV2jfgUzl3mkQGAcWAYUVB5THnCKICRkRYXMWE
rjliQDCA6KKuCdE1Y0BF1xzBsLJrAEVMICITO1Q6lev90C6vy1T39Mw00wPe14+P
3U11T1X966T72Lqq4rqa6uuveTudXlf89bKc/OnHz+fc8xeGRoIg8ZxU4m3EcSzL
1i3Lxy668YqnYrFqz3MKfZ3A91954+633n0hHmN4TmKYkjohgyAwcY5j+Emnzu7T
d4c2myW/3YsemkpRtCDEytcFHZimSpHM5Alz+vUfWkqzpLw4TuJ5SeAVz3dvv+sk
npNWrlwqK21fvLZtZrLaSSdcOmLEKGx2UXOliH59h55+Zt9EvKpc94Sy9a2RBCkI
Sjwux2I1XGn9MwihwPcO2+9YUaQ5vtTLpi0ERdKyTO2z97gSO0gFQXnwkQvDICiU
vgghimJkOfnNqs/fW/J4kd4nUYzNvfscWeZ5Ti70GppmFSX56bL3Pvvi1XbdmDoj
DAOMtXR6nSjGLzh7wblnPnj8sZfvvOOBqXgPVW2xbbPE3k6SpEQxFo/zilLdZQeP
ELJtM6c21jf8mMvV54//jJNvG9h/50wmbVmG57slfg7Py4l4SuFR8U5RjhXefGf+
D6u/7sr0RQhRFC1JibhMKnKKLu0W35VYho8pVXXVsecWP6jrLSwnFuojJQnqpzWf
v/TmE5Iktfc8IUlSFGLxmBhTqtkS3ssw/N/u+wvLoES8RmjPQzzL8jGlOpVMIoQY
hiv0RoIgeF7+Zd03b7/7Ql1NQpFTJaZv/rsIvJLT1Icfv7SUdzEMf88DZ7ueV5bW
yAYEQQpCzHbNBU9eVqhhsEnZtpHLfhz20wAAIABJREFUNdY3/KhrLRectWDSqXMG
DxymaWnPa+PK5TgxmVDmPzkrCPwtciFchWdBm1g9c9I8x/ZcxypLF38YBn7gHX7w
X8Yfdy3GWilvYVmB40SeE4vPtaEphucIjpNomi30Go4TOQ7xnFj8XKFplucQx0l0
F97cEUJBGGCsZbL19Q0/9KgbNGXiXUcfdcGeI45NJnq2ObbdTVi2kVMbGxp/QgR5
8vib9hh+xKCtdtX1TOkZXAqa5jhO5DiqK9N3c8GyQkKmPvv3q37gkWT0JROGQV1N
/2FDh3XspArCwPVs17VL+bP6vnfA/icTBHLdjtxDsKWP2HUky/CFDpWiGN3Irlj5
YTzGlB69G4QojMVi++59cimDr77vHXzgRJ5vx5B5iUiSzN/lSnmm2WRCyzayufog
9E89+ebtttlD00vKYAIhjm3jprqZqnAAOw7OZOv33++knKZ1vos/DEPbwZ7rHDfq
smyuAdZOFBIEPsZqOr1uQL+dTh1/06ijpiWTm00Go3yXGlZFIXbaiTcfN/qywYOG
6Xq6vBkMiuAF+bmXH3Rdm+OkyEaw57s9agcdsO8ETbfavMO2FvieKMT/b+Q4y9Lb
fDHG2vGjrjzm8Asc1+7AXFnLcscee7msVEWe/Pnmbzbz6xtvP8NxUns/PAwD2zJ4
TjnmyAtxKd/FUscddw1CyLbNCs452qTCMMRY4zl53AnTd9pxf7tyEx26g8o/4Btm
btyYqxNKzaJX5pY4tzBSGIaOg7FhHHn42ZnNP30ZhmdZgWU79bjqONhxrEI/hR94
GGuOg7cePHz00Re98NKtmUyDKMbatWgkEk2zLCuwDE8UaCGVwnNtx7EcF0feicIw
sGzDcbAip44fdflzi2at+uFTWUrRdAcnSG9eGIZnWZ5lhDLNk40Q+L7j4sjfk2X4
hKy/+fb9Rx56HkmSraMr/4SUTPXadae9v/vxXzQdL/3/DcPAsoxksuchB04pMk6/
geta2Vz96GP++uyiOxgac231P/3ve23TQbqe4Vg58jQjSVo3Mks+Xhhrf/M3DEPL
NsMw2Gfk+Fyu0fPsUo4nm204aL9Ji1+7iyDJLm720RTDsALL8p2/A3iu47jYcazI
Udv8iFhMTo3cY+yn/3qH45wi3YptIkmKZXiGFTbpte+6tuPgsv85Kh/ArmtlsvXH
HHnRowvnSiLuTAabWDtx/LX/t8cJuVxjGY+wImQpseAflyYSPTvxGaFlaZMmzMlk
620HF4xh39P19HZDRnKs8OyiWblcE8t29vITeOW1t+63sU514rqyLP3oIy+IxWos
rNsOjqw+E4SBidWqVO/Roy554aVbVn33R8lgSYw//vR1khjfdGuUPc8etNWwgw+c
JAgxitr4lBAE5dU3nznmyIs4kogc6/E8p65mq13/dPi/v/iAY4XS77BB4HO8dOjB
U3JqU6FZURtxXTubrR9/wqXPvTirXUueLNs4ZP/RsXhd5GwygiAEQWluXvv+ksU1
NcnSPzYvDAOCIE4cf8PuuxyRU5tKfJeJtcMPOTMRr37y2es5VijXSqRScLy0ZOnC
lvQvnZ/eYVn6kUecE4/VYEuz7YiLN5/B1alee+1x6PIV73YmgGmaWb/+u08+e1EU
o1d+loVlaWNGXRpTqjmunLeXygcwQsh1rZzWdOLoqc+/PC+RQKXP4fo92zE1Ex16
wOR1678r5WGzmxOE2Lsfvx5vd6fX/8AYiVLCxNoZp96WzTbYjhkZw/kM3n7bvV97
4+/r67+naaaTj8A8L3/86XMt6Ra245cVsizkuGaI0GnjZ/C8bFm67ZitXxYEvonV
mlSfMUde8tziW1Z992l7p0ZvjkQx/uYHLyoC6kQXQxs8D6384eP6xh9++GlZ654Y
huHiMvX4U1efNHY6QZCtGzphGGCs9uuz3R67H7xi5Qcl3mHzqxg4Xtp7z/GNJTR/
NzBM9YhDz3no8VkcZ/Kl9RW7rq2q7ohdR0tCLHJ+MklShp556Z9zFYVh2pkQYRha
lu659j57jqtv+LHEJwmEkOfZ2VzDgftNvP/ha1nG5PmC0znLjuekz796feWqr4SO
N4J+Y1nIdg0CkScce5nAy5HlGz3fTSZ67bzDAUs/fo3lnPb+whvQFNvUvPb19xd1
8m5ZHMYoCIOqVG+SLGdlnm4RwAghjHOHHjxZSdQ8+fR1HQhg2zYzGX3yqddmcw1b
QPoihPIPoYl4xxeJhWEoic77Hz5u2TZJhARJjT/uukJD477vaXr6oP0nNad/0bQW
UYx3JoNpmhUEJabYPN/xa8KTnOXfvKeqJkkSpqmeNeluZBCRI0b5DK6q7jP22Kuf
f+n273/81xYfwAzD8RSKx6upTvcWFhIEvq62vPP+IwRJRjaJBDH2wcdvTJ4wJz+5
r/ULXM+pruo3cNCwj//1RomN4CAIWJY/dexNqtbcrkF9z7NzuYZzJs9a8I9LSwxg
yzaOOPSUqupelm207n8mCELgFROrny37oLYjzd8wCIMzTp9bejt+A89zsmrjnyfe
8MDDV1eliA6MPXcMRTECr8QUrlARodJ5krPimw9U1cjl6idNmEMQROtfOAwDbGn9
+g7db+9j//35Kx0OYJKkOE4UmE7dLdskS87nX76OUEjTbBk7ortLLWjXtXUjt9tO
h6kmsu2Ihk5xhqmde9YdI/c4zmhr2f4fB0EQDMOJYjymJD797OUlHy18/qVbE/G6
Qv1LlmUMHrz75Il3ppK9KlIuZyM0zQq8UpVKLl/x3n+++PBv951ZlepZqIMxn8HV
1X2TyR66bnRg4g/YyG/3NUHhOSlyNjhDs3GFumPeBIbmIntK892MQ7fdZ++Rx5Sy
3j0MQ2xptmX8aZdDSlzC8HuGmTtov9ON0uZ+uK6VU92tBw8X+ZgflfQEQWFLm//I
xYrCtLeDNN/81XVn+LCjTTOiWA1FMZKUiMVqJDEe+duaRnaP3UdfMPUuw2x76lY3
RNOswMtVqeQXy/8tiHFBKFQUwU2leg/Zenfb6e6tpnyjQhBiLMNvgQGMEHJdO0TB
lNOuSme1dmWwZRsaRkN32E/Tmtv7sLnF2xDDPCe9+8FjTyy8rlCnVhB4up4Z0Hen
IPRNnOsmM6JpmuV5uSqV/HrlR3PumlCV7F0sg031gH1O3XXn/baMXpDuTxTiny9f
Fo/VCgVOKtezE/Hamqo+qma5bV2bYRiQJDVt6qOq1hIZisV5nqMbmQChUuZOW7Y5
5pjJW/XbMbL5ixAhCorASV+v+qJQeBRnO/iKv/5DVZtaf5F8vbzPv3xjxq2jv1j+
NkNHzO3yfDenNu6840EaRpa1uc4Tpmk2ptC3zB1HEmShGvue57ieU0K90S1TNwrg
MAwcxxqx2+hzptxe+vOvZRnptD7tzNtcO+IKJ0ma52Sek7uyakQ3RBAEy/IUSa1f
/10yUVdoLWAQeLqRPuuMe6uSvbpJAOfRNBuPJb/+dulNt40WxYJTah0H19UOjMVq
XQjgLkHTTEyhrpi+jyhF/1HyY7p7jTh+/33GuE6xxml+dYqm48GDd49sNdI0q8ip
RLyHLCULVTG0be2aix82zDYSy3GtnOrWVg9gWCEy6UmCdDxr1h3jYjLTgeEMbGmq
ibYdspeJIzrkGIb7ZNlLz74wc83aX3zfVZToskqe51iWduWFD5p4s2wE55Ek9c2q
b7us6P1mp6sDmGUEnpd5Xo4cYnQ927KMutpBOROVuD4MY/2yi+bvOHR/yzY2Gucn
SUqRkz/+/J/HF17XrrmRWySCIFmW//mXFfc/dB5XeGjWsvT+A3YKQ9StAhghRFGM
IifW/bKyKtWn0CBfGAaWpetGxjQ96IXuGpKY+HH1mupUn0I9K65rCWKM42VVw0Ub
waHnuzOnv65pLZE1k1mGf+vd+Rdc+qdl/yk4XmgY6lb9d9EtVHzRrWObY8dMHbrD
AXZ08xeJYiwmV6/9Za0gxIp8TiSMtZYMnnHVM6ra3Hooh6KYmFLtu45lm6mksOil
25f9+5VCXdwm1gb0H6rhktr03ZPt2NMveZSk6S10VXNndXUAc7z4wILz1637RpZT
hTI4nqg7Z/LMdEZvM4MtS1cxSiZ7GqbauvOZopgfV39+57zzTCPLsX/0AEYIURRD
EpRuZpKJnoWWewWBb5pZ3UibWO1Wz60EQVA0g1B4yRXDBbHgbdF2zJPG3jB8t4Nh
MKJNJlbz/zpT84Gi6JhM/eX8gVKBRSD5AdEjDj77gP1OKLQWLgxD01SzelBbMyBy
hw+aZuOxWoKkVBOxrKAo1ZGNYM93Pd+ZefXzZuFGsONYWdUVeIkkycintPyMoYsu
H6bIbAdKoRmmOXvGmz3qBkXOrGZodulHzzz34ixBkHlONrFO0XRMqY78j3zfDQJ0
5bSHWtLG5pjB2NI1jKpr+llYK+Mm9puC5zmq1tKSbsjlGp0urCHR1bOgeU5ybHPG
nClXX7yguqpv62ZWEPiObQ7b5ZALz43f88DZRWYAWpbekjYun3avLCZbn+skSSly
StWanQCJQqxQzbw/lHxH9IqvP5h335STTpheaF4MxtqsG5deN+Mw33O71SAFgQiO
l+qbV4tCLJOJ3gfCcXBNdS1N057nINRF00e7nmmqlo9+qe/sbg3PPvLLhCl9Olkf
WBTjv9anRTFeaAsW17V4Tgo8V1Uxw/CR7Vds2Q/f/bmupyO7XhiGf+X1eYv/eXdS
oRY8dnlIENtts2dU73GIsabEq3QLiViLHL51HHzS2PNH7jHWNNXIVBDFeCrZW9Xs
2prqtr76xkysGjaKyUlNb2n9RSiKicVqGJb3fFeUEiRBCoJ0/8PTJp522zYDd2/d
XA7D0LLUgVvtfMO1z8645biuXJLUedjSW9LGtRcvoBnecTKRr6EoRhBiohDj2E5U
7PFdVWvGLlrza0OHPwQhNHznEdPO/cfrbz/w4uI5XVaws6sDmCBIUYgjhBLxGkVO
5XINrfce9zzHxFoQBjkj5HmjUH8jtozpVzzVs+c2mt6y0TJEkiAVOVXf+OMtc8aK
LOpMPaYtDEUxBEnatsFxYqH9+LClVVX1tR2LRESrAgyVRBAESVIkSZFEwcMKwzAI
giAIKruD96Zmmrm/zXpbFBNEJwo1mDgXU2oMC6U6dzAURckikuWkICiRszfCMLRs
45QTb6QZ5l/LFrcO4HxusYyYyUTcQ2maTcRqOE6iKFKWkqraLPOxmFKT9ta1Di3f
d0UhfufNb1wx/eDWAew4VlZ1XMf1fLfQRAGOFcef3i8RZzuwEs808b23L/EDP3Ib
Y5pml3y88OHHro4pMkmQCCGOE7FlyGJSUaq97K9RX8czzRxF0BpGgqAXmuzWfVi2
YZq6gVGA0DUXz+/XdwfdyBTqYqFp5qNPFz70yFWxWMe/l+c5u+82avhuRxfZAqft
D/Ed3/cQIsIwcBynyx7dK7AOmCAIkUNX33DEjdf8U1GqNW3j+EQIeZ6z3TZ7XzHt
/nn3TWldjy1/PasmshzdNLOtH4RJil7f8MPVN42rTvAUWfkVNd0HQRAkSZIEVeSh
JAzDMPTDINiEVR46iiBISYyLUlzgFWy1e6XKFsMwcwRBqFpLZxZEhGFoGJlyPKcQ
JEmecLJy9x1fFZo+6Tg4DJHne65rb7TVTxiGhmE9+dAPaq4psnIhw3CvvH7vU8/N
TiVjJEnxvHz7384+68+zhgzeI7LVaJpZx3VbN4LDMHRcfPK4Cw/ef3I2F71RqSQm
aqr7MjSSCs/1ixSGIcaqYSPP92zHbN22pigmHquJKVUsQ2wYESMIUuClWbMnnX/W
HVtttUvk8j/Pd6ur+t06/eXrZx5V+maLHWDbRktmXXPObs51qin517Pn7j7sKF3P
+KGnG5lCaxopionHauNKDcuizgwRep7j+x5BEJ3pOg7DMAxDhuX9wO/KR/fKFOLg
eY4kyGlXHnbLdS/ynOj5G191vu+ZOGMa2ZwRcpyx0Wln20ZONa69+JH+/XZUtY1r
vJEkpSgpAiGJR4IQM4zo3o8/LJpiBUHiWJFjxcjaUt0ZQZC5XNMpk6rnzFxWOIBD
FAZhGIRhuEXuoIIQ+u8do7OD9GXZYJUgCElKOi6Ox+pMU40cxEUIWbZh2wa2fEF0
fr/2xjRz2EWmoUUOmtI0G5OrFCUpiXS+Y5DjRFHUY0qVLFd5OSei1Rj4qWTPe27/
4OIr9/592LuuraqOrqVNM+e6kc1fQpISR45LVcXZ9lY7wpaWyVnzbn2PZQVd33hw
h6JoRa769+evzrvvwkTif54/OE6SRCOmVMlSKus1tJ6AFoaBYWRMnNUw4nlN4JVN
dFZbtnHFxYtkMUZ1opirbZvY0hqb13ieG4ZhoaFfimIUuerzr964657zNvpBOiAM
g/JM8uryqWKVCWACIUFQZAsLoiLycU1vbj1e4nnOzjsffPHUu/7+4LkEQWzoiA7D
AFvG5Rf9Y+CAXTKZ9Ru9kSQpWUo1Na297LpjqpNb5g5WnUTTzPtLXta03KTTb9/s
ApgkSUmMEwQhCDGU+TXyNY6DUYg81/d9tzM1ZkHpSJJ0HOukibX33LkCW1pkl6Pj
4HPOfIDnLlj2n1dp6rdyQkEQGIb99PzV2VxD5LsYhn/z3fnzn7ilR/VvG9Tndyi6
8fbJf506d6sBEa3GMAx0vUU3sthC2NJEIYZ+26zFHHvcucccPq2xeXXkt5DEOMvJ
Mo8KTSgrJAwD08R/u30Jywg5tan1F6Fp9uuV78++58K6lLxRay//da695dTLL7i3
d6/tHCeivegHXp/e29105TOzbj8hX6KrXYdXIts2bdvMZqNnV5QLQZA0zYhifOWq
j+bOOy+V3PgH+UOpWB8jQRCiIJ5/yf7NLatFId66ko7ve6ravPNOB006fbZl6flH
9TAMMNZdD6las6o1tyqhTohCvCX9Sz59O7aCfotHUYwgELwgV3Rn0E3rnL88sOeI
UZU+ij8QgiBlKZlMVRMIFYoHx8GZTL3j2raDN5SZxFhFJGpO/2IY0c1fnhU5Tkgp
9O8r1PKcFJcImmZ5TibJqPnDgd+zx6DbbnrNNHF+Mr/r2rbrYsvI5uojm78EQcpy
cuzpfTm2/Qt/sR4EKJOp1/R0604FiqR5TqYpNi4RPB/Rh8zzUlxCDM2JQizykTEI
fE1r6dN7yCUXPoVNs1stT2gXiqIlMU4S5Nq1K+b87exUUo78Qf44KjnIJwhKdVK8
dPoYgiQKzcJX1SZsqyhE2NJ938NYD1Fw/l/uGbrdvn6rJQQURZMkden00ZC+RXie
QxBIEiS7hOqAmyfi1rnj31+6qCt3kuliJEn+tudjh/7RdEdmGBVHEKSut0y7bFdJ
Shbqv7Vt4+zJ9+y68yGug8MwDAJfN+y/z/mKoZjI5i/L8J99vvjvD09nW63j53lp
5tyz1vz8ZeQugWEYaHpa05tphrRsPQwD2zEP3n/iuGOvdArUAxGFmGVpVXFaFBPt
ioQg8A0T3zT9tbrarSKncNMM99PPX86886wipdF5Xr759onLv3mPY6XIP00Q+KrW
ohsZRCLczZYItokkKZpmOU5U5Oqf13099ZL9br/rtHz6luPjCYqkGYbr4OXA8BTF
bLpdxYqr8CwbQVBkHmWzjbKUinySdT1nxLDRE0+9jaZZ3cgwLHfGKbdts80emr5x
85ckaVlJZbINMo8gfYvwfHe3Px018bQ5Hai5vVlgWUEQYqLAbMH7EopioiVdj7Fu
WUYH/mVzTbKUYlmBbP8610JIkpTEpCJVa2pToQvQca1Mtp7lBD8IfN81sRZPSBmt
0TBzrQcLaYpBJIUtnPjf5m9evhGsGxmW4QolVr8+O1x+0bOWhV3PIQmCoelsrjFy
8jNJUrKcmnrR7mFItLdBZlm6LEmmntGNTOvmL0NzPCdZWItLRJFdInhOSiXl2Xdf
kFMbC42b+L679aDdL5j6sMBvBrM3NoQux0mJeF0m02hZRmPj6pm3nVGXklLJujKl
L6JpJkRENtfUsWvBMFXLNhU5RdMsscm2Nil48F38/7UmitJl1x511cUL+vTZTtM2
Xhjj+66qNe8+7Kh0Zv0jT99+/Jhzdxx6YDoTMV9fkuI/r11+46zTqlJ/3BGFUvie
i7GGsVaoKkJ3FoahH3gEIiKLJf1xiGJ8+i0nkx19gvYQuurCBwkCbb/tyDI+g5Mk
mcn+OmP2mJnTP7QsPbI5aNvGlNPvymbGfv/TMl1377z1bYpidCPd6oUEywo/fP/p
/Mdu6lEdXXdF4OXZ91w446pn4/Ga1kV78rXBdSNjYERR2p7Dx4wZdVlT05rojxKU
xqafEorCsFy7AtgPPMO0rrpkYY+6gWqrTX8JgiRIcsnHTz/0+KyYiApNT9uAIdDa
dV9vv+3IyB0eg8DX9fTggcOOH3PN/fOncaxQ3m4MkqQokqaotrfbC8PA9z0/8IrU
y2NZYd2vKxmaYxjOdfC1s05kCCTwqCollXdBM02zy5YtfvCJmR0LMx+hQf37//n0
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
W4ULU9uOWfrNuQiel7CpTp95VFPzz4WuTce1XBvvOfz44489v75ZzeYanAInPMbG
rGue5zgp8rlnc1e2AA6CQDcyspyYfuVrRYqP25Zx3tnzt+q3raa3tPlnJgiSZUWW
YWmKq63pv/ifdy1aPI+mWLqEdSa+72laS0NDun+/radMuqvIcx+2VIbm19U3Njan
DSPbOjs9z1XV5vrGzLBd/m/scdcUyXLTVO+e+0ladZtbGguUobdzamNz1jL0jB8E
XRDAkhivqe7Xv++O/fvtaOLc5LO3//PUAQ8suCBEIUUykhinKabEu14Q+Jqebmhs
qa3tff7Uh9tV8aBjeF6uSvXu22f7/PH36Ln15Kk7XHbNSJIggzBQ5CqaZtvV8DXN
nKa7d8/9pHsu/vE8J6dmhu92wPhx13Zm5LX7cF07p2YP3G/00UdNK/5IYdnG3Jvf
bMk5zS0NHXs8MsxcU0tjWvPm3fFp8dIftmNOPOXWnXbcU9Uy+cn2kTac8FVVvc89
88EAhUXaYbZtzLxxSRigpubmyNtIB4RhoBuZxqZmikY3XPNO8R3MwjAwzRyByCsv
fTVAfmNTs25kwjD474dk6uvXMjTb1s35oZ49+jQ0tmh62u9ogWvHsSzb0TGybVyk
K8BxLcsx/m+PsX+/4+PDDjl7fXMum2v4/VJDbOkt6QYNI5JicNSmHVsAon+vTpVr
2SAIgphSfe/cr01smrjgj0UgQhAUUZSuuu7AH9d8UcpYheNahpE7eez1fug/+/wM
SUqW0sPj+96ggcNuueG1XM4KQg9jPSw8b00UY0SIqquk0SfWEQTa6PM9zx0+7OhL
pj2s65brObZtFvoogiBkKRkGgaIIR48VpFYL+FzXPvSgsyZPmGGY2HGsIo3psiAQ
CglizCk9eQZRJOI4hkAEL8gkQbZ35zWEUBD4vXttc+etS7I5Kwj8LugUEnj5zPMG
NzQ2EiQiEBJFNvB9SUoQJNmx+XeGkX3hSQtbtm5kKno9h4aRs22DIMjJ5w3r999r
0POcEbuNumDqfMPULMsocsZ2Dde1DTPLs+JTz9347gePJtpf2sl17YP2nXjGhNkm
ztm2WeT7kAQpy0kUIse1JpzZQ2z/jFbDzC1eiLM5myRDvWgEEojgeUkUlFvvGPef
L98s1BEdBH7vXkPuvPXDUk54giAkMY5CVFUljBlfF7a6jXRAGAY0wz+94Od02gpR
UOS++vvDEIUYgchUih87oZ/nWgghlhOfnL+6ucVEKCxURx39dnOWSZJOxPkLLtnn
53VfUx2a1mRZ+o3XvjtowJ883zGxVmT+PPFbK4tnGEHg2aefnfnMopkbhiqwpd8x
c1ld7UA/8E0zF7ndclfCWDWMbE1N/7Gn9OzRo7osY17lDGDDzNq2a9pt3DYIhAQW
cRwjiSVtpBWGoeNavucgAtEUV2IpUd/3TFOzHQfbKGxrzjiBEIGQJCCWZSUx0TqA
saViy7OdUj9KFhHL8pEBjC0NW77tIlSumexFD0biEcsyohAnCIQQkdexTwsC3zBV
2y7pJy0LgUEcR0lijCAotOHoO3FfM4ysZduGVelkQwgh9Nyj61zbGj950O8DGGPV
tHzb7RZHiBDad89DzvvLQ48+ecW77z8aj9e29+2ua2NLxTiwvba/EYEQmb92OKFj
AWzbloFLOjkJhHgGCQIt8LEiAdyuEz5/7YsC4qJuIx0QhoFh5mzbMa12XHG/HQaP
OI7N34JK/5D8ewUOcSwrSkqHAxhbBsYov8SulN8NIcTRSBBInld+H8C2bRgm8kv7
nC4w75b3hmyzx5FjuNramrJMmilbAKP2Tw/LR0J7P7nk07qDLZzWn9/hplIZP6qT
yjQrpDKHX/p50qZu04sV6kbWtt3HHm46+fSa6qrfrsFuc3i/MbF60P5nTJowe/6j
f33jrQeKlGQqpIzXTiX+r47/Qco1D6uTp0T+MDr2IR2+7spyzJ3/nLLDWG3KWi8/
kz31jCpFTpVlFnA5AxgAUKIwDHG+M5AgOtDa6xquaxtmznNDmiEkMd7e8pAAbGFM
U803xQUhVp6l3hDAAAAAQNfbDJZ+AgAAAFseCGAAAACgAiCAAQAAgAqAAAYAAAAq
AAIYAAAAqAAIYAAAAKACIIABAACACoAABgAAACoAAhgAAACoAAhgAAAAoAIggAEA
AIAKgAAGAAAAKgACGAAAAKgACGAAAACgAiCAAQAAgAqAAAYAAAAqAAIYAAAAqAAI
YAAAAKACIIABAACACoAABgAAACoAAhgAAACoAAhgAAAAoAIggAEAAIAKgAAGAAAA
KgACGAAAAKgACGAAAACgAiCAAQAAgAqAAAYAAAAqAAIYAAAAqAAIYAAAAKACIIAB
AACACoAABgAAACoAAhgAAACoAAhgAAAAoAIggAEAAIAKgAAGAAAAKgACGAAAAKgA
CGAAAACgAiCAAQAAgAqAAAYAAAAqAAIYAAAAqAAIYAAAAKACIIABAACACoAABgAA
ACoAAhgAAACoAAhgAAAAoAIggAEAAIAKgAAGAAAAKgACGAAAAKgACGAAAACgAiCA
AQAAgAqAAAYAAAAqAAIYAAAAqAAIYAAAAKACIIABAACACoAABgAAACoAAhgAAACo
AAhgAAAAoAIggAEAAIAKgAAGAAAAKgACGAAAAKgACGAAAACgAiCAAQAAgAqAAAYA
AAAqAAIYAAAAqAAIYAAAAKACIIABAACACoAABgAAACoAAhgAAACoAAhgAAAAoAIg
gAEAAIAKgAAGAAAAKgACGAAAAKiA/wdXr6Z02sdfbwAAAABJRU5ErkJggg=="""
	def __init__(self):
		import binascii
		self.buff = ''
		for l in self.logo.split("\n"):
			self.buff += binascii.a2b_base64(l)	
	def get_image(self):
		return self.buff
		


if __name__ == "__main__":
	RTMLogo()
#	f = TkCalibGUI()
#	f.mainloop()
