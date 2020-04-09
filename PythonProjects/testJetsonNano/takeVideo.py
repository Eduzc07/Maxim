#!/usr/bin/python
import time
import datetime

import cv2
import math
import numpy as np
import jetson.utils
import argparse

# parse the command line
parser = argparse.ArgumentParser()
parser.add_argument("--frames", type=int, default=150, help="Desired Frames to save")

opt = parser.parse_args()
print(opt)

#---------------------------------------
# Image Size
#---------------------------------------
width = [1920, 1280, 1024, 640, 800, 1280, 320]
height = [1080, 720, 768, 480, 600, 1024, 240]
frame_width = width[1]
frame_height = height[1]

# create camera device
camera = jetson.utils.gstCamera(frame_width, frame_height, "/dev/video0")

# open the camera for streaming
camera.Open()
print("Opening Camera ...")
time.sleep(5)

#Create array for Images
images_array = []

# Start time
start = time.time()

max_frames = opt.frames
frames = 0

try:
    while(frames < max_frames):
        frames += 1
        image, width, height = camera.CaptureRGBA(zeroCopy=1)
        jetson.utils.cudaDeviceSynchronize()
        arr = jetson.utils.cudaToNumpy(image, width, height, 4)
        img_data = cv2.cvtColor(arr, cv2.COLOR_RGBA2RGB).astype(np.uint8)
        images_array.append(img_data)

except:
    print("\n ---- Error, Closing ---")
    camera.Close()

#---------------------------------------
# End time
#---------------------------------------
end = time.time()
# close the camera
camera.Close()
# Time elapsed
seconds = end - start
print("===================================================")
print "Time taken : {:.2f} seconds".format(seconds)
# Calculate frames per second
fps  = round(len(images_array) / seconds);
print "Estimated frames per second : {:.0f}".format(fps);



dT = datetime.datetime.now()
timestampName = dT.strftime("%d%m%Y_%H%M%S")
folderDate = dT.strftime("%d%m%Y")
#---------------------------------------
# Save Video
#---------------------------------------
# Define the codec and create VideoWriter object.The output is stored in 'outpy.avi' file.
#out = cv2.VideoWriter('/home/edu/Workspace/pythonExamples/videos/%s/%s_video.mp4'%(folderDate, timestampName),
                      #cv2.VideoWriter_fourcc(*'mp4v'),
                      #fps,
                      #(frame_width, frame_height))

out = cv2.VideoWriter('/home/edu/Workspace/pythonExamples/videos/%s/%s_video_%d.avi'%(folderDate, timestampName, fps),
                      cv2.VideoWriter_fourcc('M','J','P','G'),
                      fps,
                      (frame_width, frame_height))

print(". . . Saving Video . . .")
for iImg in images_array:
    #arr = jetson.utils.cudaToNumpy(xImg, frame_width, frame_height, 4)
    # img_data = cv2.cvtColor(xImg, cv2.COLOR_RGBA2RGB).astype(np.uint8)
    # img_data = cv2.cvtColor(img_data, cv2.COLOR_RGB2BGR)
    img_data = cv2.cvtColor(iImg, cv2.COLOR_RGB2BGR)
    out.write(img_data)

print("\n . . . Closing . . .")
print('Saved video in \n /home/edu/Workspace/pythonExamples/videos/%s/%s_video_%d.avi'%(folderDate, timestampName, fps))
out.release()
