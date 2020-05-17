#!/usr/bin/env python3
import numpy as np
import cv2 as cv

print("OpenCV version:" + cv.__version__)

cap = cv.VideoCapture(0)

if not cap.isOpened():
    print("Cannot open camera")
    exit()

while True:
    # Capture frame-by-frame
    ret, frame = cap.read()
    # if frame is read correctly ret is True
    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break
    # Our operations on the frame come here
    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    # Display the resulting frame
    cv.imshow('frame', gray)
   
    key = cv.waitKey(1)
    c = chr(key & 255)
    if c in ['q', 'Q', chr(27)]:
        break
    
# When everything done, release the capture
cap.release()
cv.destroyAllWindows()