#!/usr/bin/env python

'''
RNA to recognize persons

'''

# Python 2/3 compatibility
from __future__ import print_function

import numpy as np
import cv2
import os.path
import os

sv = np.loadtxt('data/data_images/v.data',np.float32)
sw = np.loadtxt('data/data_images/w.data',np.float32)

#names=["Edu","Antonio","Jorge","Patty","JP","America","Amoran"]
names = os.listdir("data/data_images/Persons/")
#names=["Edu","America","Amoran"]
print(names)

def detect(img, cascade):
	rects = cascade.detectMultiScale(img, scaleFactor=1.3, minNeighbors=4, minSize=(30, 30),
									 flags=cv2.CASCADE_SCALE_IMAGE)
	if len(rects) == 0:
		return []
	rects[:,2:] += rects[:,:2]
	return rects

def draw_rects(img, rects, color):
	val = 20
	for x1, y1, x2, y2 in rects:
		cv2.rectangle(img, (x1 - val, y1 - 2*val), (x2+ val, y2+val), color, 2)

def getFiles(mypath):
	onlyfiles = []
	for file in os.listdir(mypath):
		if file.endswith(".jpg"):
			onlyfiles.append(os.path.join(mypath, file))

	return onlyfiles

if __name__ == '__main__':
	import sys, getopt
	print(__doc__)

	args, video_src = getopt.getopt(sys.argv[1:], '', ['cascade=', 'nested-cascade='])
	try:
		video_src = video_src[0]
	except:
		video_src = 0

	args = dict(args)
	cascade_fn = args.get('--cascade', "data/haarcascades/haarcascade_frontalface_alt.xml")
	nested_fn  = args.get('--nested-cascade', "data/haarcascades/haarcascade_eye.xml")

	cascade = cv2.CascadeClassifier(cascade_fn)
	nested = cv2.CascadeClassifier(nested_fn)

	# cam = cv2.imread('cara1.jpg')

	# while True:
	# ret, img = cam.read()

	#############################################################################3
	samples =  np.empty((0, 2500))
	#---------------------------------------------------------------------------
	# Uncomment in order to aks in terminal
	#name = raw_input("Who do you want to see? ")
	# Fix a name and number of  pictures.
	name = "patty"
	#---------------------------------------------------------------------------

	fileName = "data/data_images/Test"
	#fileName = 'data/data_images/Persons/%s'%name
	#list = os.listdir("data/data_images/Persons/%s"%name)
	list = os.listdir(fileName)
	iv = len(list) #image
	if (iv == 0):
		print("Empty folder")
		quit()

	#Get list of all the images
	#onlyfiles = getFiles(fileName)
	onlyfiles = getFiles(fileName)

	for i in onlyfiles:
		img = cv2.imread(i)
		# img = cv2.imread("cara2.jpg")

		if not (type(img) is np.ndarray): 
			print("There is no Image. Run first \"Take_photo.py\"")
			quit()
		
		gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
		gray = cv2.equalizeHist(gray)

		rects = detect(gray, cascade)
		vis = img.copy()

		draw_rects(vis, rects, (0, 150, 0))
		cv2.imshow("Image", vis)

		for x1, y1, x2, y2 in rects:
			crop_img = img[y1:y2, x1:x2]
			gray = cv2.cvtColor(crop_img, cv2.COLOR_BGR2GRAY)
			gray = cv2.equalizeHist(gray)
			#Set threshold and maxValue
			thresh = 120
			maxValue = 255
			#Basic threshold example
			th,dst=cv2.threshold(gray,thresh,maxValue,cv2.THRESH_BINARY)
			resized_image = cv2.resize(dst, (50, 50)) 
			#-----------------------------------------------------
			#RNA
			#-----------------------------------------------------
			
			#############################################################################3
			resized_image = np.asarray(resized_image, dtype='uint8' )
			h,w = resized_image.shape
			sample = resized_image.reshape((1,w*h))
			sample = sample/255 ##value to compare
			
			h,w = sample.shape
			bias = np.ones((h,1))
			x = np.column_stack((sample, bias))

			#Computing output
			inX=x.transpose()
			m=sv.transpose().dot(inX)
			n=1.0/(1.0 + np.exp(-m)) #sigmond 1
			# n = 2.0/(1.0+np.exp(-m)) - 1   #sigmoid 2
			# n = np.exp(-np.asarray(m)*np.asarray(m));                 # Gaussian
			out = sw.transpose().dot(n)
			y = out

			if (np.max(out) < 0.8 ):
				break

			person = np.argmax(y)
			imgName = os.path.basename(os.path.normpath(i))
			print("\'%s\' esta en la foto %s." %(names[person], imgName))

			val = 20
			cv2.rectangle(vis, (x1 - val, y1 - 2*val), (x2 + val, y1), (0, 150, 0), -1)
			cv2.putText(vis,"%s"%names[person], (x1 - val/2, y1 - 15), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (250,255,250),2)
			#############################################################################
			cv2.imshow("Image", vis)

		key = cv2.waitKey(0)
		if key == ord('q') or key == 27:
			break
	cv2.destroyAllWindows()
