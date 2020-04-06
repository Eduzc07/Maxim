#!/usr/bin/python
import jetson.utils
#import numpy as np
#import cv2
import time
#import math
import datetime

# create display window
#frame_width = 640
#frame_height = 480

frame_width = 1280
frame_height = 720

# create camera device
camera = jetson.utils.gstCamera(frame_width, frame_height, "/dev/video0")

# open the camera for streaming
camera.Open()

time.sleep(5)
# Converting datetime object to string
dateTimeObj = datetime.datetime.now()
timestampStr = dateTimeObj.strftime("%H%M%S_%d%m%Y")
nameImage = "images/%s.jpg"%(timestampStr)
image, width, height = camera.CaptureRGBA(zeroCopy=1)
jetson.utils.cudaDeviceSynchronize()
jetson.utils.saveImageRGBA(nameImage, image, width, height)

# close the camera
camera.Close()
