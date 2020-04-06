#!/usr/bin/python
import time
import datetime

import cv2
import numpy as np
import jetson.utils
import argparse

# parse the command line
parser = argparse.ArgumentParser()

parser.add_argument("--fps", type=int, default=30, help="desired Frames per Second")
parser.add_argument("--hours", type=float, default=1.0, help="hour to save")

opt = parser.parse_args()
print(opt)

# create display window
#frame_width = 640
#frame_height = 480
width = [1280 640]
height = [720 480]
frame_width = width[0]
frame_height = height[0]

# create camera device
camera = jetson.utils.gstCamera(frame_width, frame_height, "/dev/video0")

dT = datetime.datetime.now()
timestampStr = dT.strftime("%H%M_%d%m%Y")

#cv2.VideoWriter_fourcc('M','J','P','G'),

fps = opt.fps
# Define the codec and create VideoWriter object.The output is stored in 'outpy.avi' file.
out = cv2.VideoWriter('videos/timeLapsed_%s.mp4'%(timestampStr),
                      cv2.VideoWriter_fourcc(*'mp4v'),
                      fps,
                      (frame_width, frame_height))

# open the camera for streaming
camera.Open()

#Create array for Images
#images_array = []

time.sleep(5)

max_frames = round(300 * opt.hours)
num_frames = 120 #100 -> 10 seg // 5 Frames per Minutes
frames = 0
count = 0

try:
    while(count < max_frames):
        frames += 1
        image, width, height = camera.CaptureRGBA(zeroCopy=1)

        if (frames == num_frames):
            frames = 0
            count += 1
            # Converting datetime object to string
            dateTimeObj = datetime.datetime.now()
            timestampStr = dateTimeObj.strftime("%H:%M:%S_%d/%m/%Y")
            print "Frame saved: %s - %s"%(count, timestampStr)
            jetson.utils.cudaDeviceSynchronize()
            arr = jetson.utils.cudaToNumpy(image, width, height, 4)
            img_data = cv2.cvtColor(arr, cv2.COLOR_RGBA2RGB).astype(np.uint8)
            img_data = cv2.cvtColor(img_data, cv2.COLOR_RGB2BGR)
            out.write(img_data)
            #images_array.append(img_data)
except:
    print("\n ---- Closing ---")
    camera.Close()
    out.release()

# close the camera
camera.Close()
out.release()
