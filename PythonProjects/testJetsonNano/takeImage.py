#!/usr/bin/python
import jetson.utils
#import numpy as np
#import cv2
import time
#import math
import datetime

# create display window
width = [1920, 1280, 1024, 640, 800, 1280, 320]
height = [1080, 720, 768, 480, 600, 1024, 240]
frame_width = width[1]
frame_height = height[1]

# create camera device
camera = jetson.utils.gstCamera(frame_width, frame_height, "/dev/video0")

# open the camera for streaming
camera.Open()

time.sleep(5)
# Converting datetime object to string
dateTimeObj = datetime.datetime.now()
timestampStr = dateTimeObj.strftime("%d%m%Y_%H%M%S")
folderDate = dateTimeObj.strftime("%d%m%Y")
nameImage = "images/%s/%s.jpg"%(folderDate, timestampStr)
image, width, height = camera.CaptureRGBA(zeroCopy=1)
jetson.utils.cudaDeviceSynchronize()
jetson.utils.saveImageRGBA(nameImage, image, width, height)

# close the camera
camera.Close()
