#!/usr/bin/python
import jetson.utils
import numpy as np
import cv2
import time
import math

# create display window
#display = jetson.utils.glDisplay()
frame_width = 640
frame_height = 480

# create camera device
camera = jetson.utils.gstCamera(frame_width, frame_height, "/dev/video0")

# open the camera for streaming
camera.Open()
X_data = []

# Start time
start = time.time()

num_frames = 180
frames = 0
while(frames < num_frames):
    frames += 1
    image, width, height = camera.CaptureRGBA(zeroCopy=1)
    jetson.utils.cudaDeviceSynchronize()
    arr = jetson.utils.cudaToNumpy(image, width, height, 4)
    img_data = cv2.cvtColor(arr, cv2.COLOR_RGBA2RGB).astype(np.uint8)
    X_data.append(img_data)

# while display.IsOpen():
#     image, width, height = camera.CaptureRGBA(zeroCopy=1)
#     jetson.utils.cudaDeviceSynchronize()
#     arr = jetson.utils.cudaToNumpy(image, width, height, 4)
#     img_data = cv2.cvtColor(arr, cv2.COLOR_RGBA2RGB).astype(np.uint8)
#     X_data.append(img_data)
#     display.RenderOnce(image, width, height)
#     display.SetTitle("{:s} | {:d}x{:d} | {:.0f} FPS".format("Camera Viewer", width, height, display.GetFPS()))

# End time
end = time.time()
# Time elapsed
seconds = end - start
print "Time taken : {:.2f} seconds".format(seconds)
# Calculate frames per second
fps  = math.floor(len(X_data) / seconds);
print "Estimated frames per second : {:.0f}".format(fps);

# close the camera
camera.Close()

# Define the codec and create VideoWriter object.The output is stored in 'outpy.avi' file.
out = cv2.VideoWriter('outpy.avi',
                      cv2.VideoWriter_fourcc('M','J','P','G'),
                      fps,
                      (frame_width, frame_height))
#out = cv2.VideoWriter('outpy.avi', -1, 5, (frame_width, frame_height))

print(len(X_data))
print("Saving Video")
j = 0;
for xImg in X_data:
    #arr = jetson.utils.cudaToNumpy(xImg, frame_width, frame_height, 4)
    # img_data = cv2.cvtColor(xImg, cv2.COLOR_RGBA2RGB).astype(np.uint8)
    # img_data = cv2.cvtColor(img_data, cv2.COLOR_RGB2BGR)
    img_data = cv2.cvtColor(xImg, cv2.COLOR_RGB2BGR)
    out.write(img_data)
    #print('img_data %d'%(j))
    #if (j%10==0):
        #cv2.imshow('img_data', xImg)
        #cv2.waitKey(0)
    #if 0xFF == ord('q'):
        #break

    #j += 1

out.release()
#jetson.utils.saveImageRGBA("test_0.jpg", X_data[0], width, height)
#jetson.utils.saveImageRGBA("test_90.jpg", X_data[90], width, height)
#jetson.utils.saveImageRGBA("test_ff.jpg", X_data[len(X_data)-1], width, height)


#filename = 'savedImage.jpg'
#cv2.imwrite(filename, X_data[5])

# capture frames until user exits
#while display.IsOpen():
#display.RenderOnce(image, width, height)
#display.SetTitle("{:s} | {:d}x{:d} | {:.0f} FPS".format("Camera Viewer", width, height, display.GetFPS()))
