#!/usr/bin/python
import jetson.utils
import numpy as np
import cv2
import time
#import math
import datetime
from cameraCalib import remapImage

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
#jetson.utils.saveImageRGBA(nameImage, image, width, height)

# close the camera
camera.Close()

#Convert and save Image
arr = jetson.utils.cudaToNumpy(image, width, height, 4)
img_data = cv2.cvtColor(arr, cv2.COLOR_RGBA2RGB).astype(np.uint8)
img_data = cv2.cvtColor(img_data, cv2.COLOR_RGB2BGR)
finalImage = remapImage(img_data)

#nameImage = "images/%s/%s_rm.jpg"%(folderDate, timestampStr)
cv2.imwrite(nameImage, finalImage)
