#!/usr/bin/python
import jetson.inference
import jetson.utils

import argparse
import ctypes
import cv2
import numpy as np

filename = "./images/12042020/12042020_130744.jpg"

# load an image (into shared CPU/GPU memory)
img, width, height = jetson.utils.loadImageRGBA(filename)

#argValues= ['./detectnet-console.py', '--network=ssd-mobilenet-v2', 'images/peds_0.jpg', 'output_ped0.jpg']

#--------------------------------------------------------------
# Classification
#--------------------------------------------------------------
# load the recognition network
#net = jetson.inference.imageNet("googlenet")
# classify the image
#class_idx, confidence = net.Classify(img, width, height)
# find the object description
#class_desc = net.GetClassDesc(class_idx)
# print out the result
#print("image is recognized as '{:s}' (class #{:d}) with {:f}% confidence".format(class_desc, class_idx, confidence * 100))

#--------------------------------------------------------------
# Object Detection
#--------------------------------------------------------------
# load the object detection network
#net = jetson.inference.detectNet("ssd-mobilenet-v2", threshold=0.5)
#net = jetson.inference.detectNet("ssd-inception-v2", 0.5)
# detect objects in the image (with overlay)
#detections = net.Detect(img, width, height, "box,labels,conf")
# print the detections
# print("detected {:d} objects in image".format(len(detections)))
#
# for detection in detections:
# 	print(detection)
#
# # print out timing info
# net.PrintProfilerTimes()

#--------------------------------------------------------------
# Object Detection
#--------------------------------------------------------------
# load the segmentation network
# net = jetson.inference.segNet("fcn-resnet18-voc")
net = jetson.inference.segNet("fcn-resnet18-cityscapes")

# allocate the output image for the overlay/mask
img_output = jetson.utils.cudaAllocMapped(width * height * 4 * ctypes.sizeof(ctypes.c_float))

# process the segmentation network
net.Process(img, width, height, "void")

#net.Overlay(img_output, width, height, "linear")
net.Mask(img_output, width, height, "linear")

jetson.utils.cudaDeviceSynchronize()
# save the output image with the bounding box overlays
jetson.utils.saveImageRGBA('./images/12042020/output_segNet.jpg', img_output, width, height)

imgMat = cv2.imread(filename, cv2.IMREAD_COLOR)

#Convert and save Image
arr = jetson.utils.cudaToNumpy(img_output, width, height, 4)
img_data = cv2.cvtColor(arr, cv2.COLOR_RGBA2RGB).astype(np.uint8)
outputMat = cv2.cvtColor(img_data, cv2.COLOR_RGB2BGR)

dst = cv2.addWeighted(imgMat, 0.2, outputMat, 0.7, 0)
cv2.imwrite('./images/12042020/output_segNet_Mat.jpg', dst)

print("\n\n\n\n")
print("===================================================")
print("=====================Result========================")
print("===================================================")
# print out timing info
net.PrintProfilerTimes()

# free CUDA image memory
del img
del img_output
