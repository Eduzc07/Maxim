#!/usr/bin/env python3
'''
Face recognition:
Program to take fotos in real time and generate data to train.

USAGE:
    example2.py 

Keys
----
ESC  - exit
save - Space bar
'''

# Python 2/3 compatibility
from __future__ import print_function

from datetime import datetime
import time
import numpy as np
import cv2
import os

#---------------------------------------------------------------------------
# Uncomment in order to aks in terminal
#name = raw_input("What's your name? ")
#n_images = raw_input("How many pictures? ")
# Fix a name and number of  pictures.
name = "Edu"
n_images = 5
#---------------------------------------------------------------------------


def nothing(x):
    pass

def detect(img, cascade):
    rects = cascade.detectMultiScale(img, scaleFactor=1.3, minNeighbors=4, minSize=(30, 30),
                                     flags=cv2.CASCADE_SCALE_IMAGE)
    if len(rects) == 0:
        return []
    rects[:,2:] += rects[:,:2]
    width, height = rects.shape[:2]
    return rects

def draw_rects(img, rects, color):
    for x1, y1, x2, y2 in rects:
        cv2.rectangle(img, (x1, y1), (x2, y2), color, 2)


def main():
    if not os.path.exists('data/data_images/Persons/%s'%(name)):
        os.makedirs("data/data_images/Persons/%s"%(name))

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

    cam = cv2.VideoCapture(0)

    n = 0;

    #Read previous Data
    currentFile = 'data/data_images/Persons/%s/generalsamples.data'%name
    if os.path.isfile(currentFile):
        samples = np.loadtxt(currentFile, np.float32)
    else:
        samples =  np.empty((0,2500))
    

    isClose = False
    isPhoto = True

    cv2.namedWindow('image')
    while n<int(n_images):
        ret, img = cam.read()

        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        gray = cv2.equalizeHist(gray)

        rects = detect(gray, cascade)
        vis = img.copy()
        draw_rects(vis, rects,(0,255,0))

        trigger = 5 - time.localtime().tm_sec%6
        height, width = vis.shape[:2]
        cv2.putText(vis,"...%d..."%trigger, (width/2 - 60, height/2), cv2.FONT_HERSHEY_SIMPLEX, 2, (50,255,50), 2)

        if (trigger == 5):
            isClose = False

        if (trigger == 0 and not isClose):
            isPhoto = True

        key = cv2.waitKey(2)        
        for x1,y1,x2,y2 in rects:
            crop_img = img[y1:y2, x1:x2]
            #--------------------------------------------------------
            gray = cv2.cvtColor(crop_img, cv2.COLOR_BGR2GRAY)
            gray=cv2.equalizeHist(gray)
            #Basic threshold example
            #Set threshold and maxValue
            thresh=127
            maxValue=255
            th,dst=cv2.threshold(gray,thresh,maxValue,cv2.THRESH_BINARY);
            #--------------------------------------------------------
            
            resized_image = cv2.resize(dst,(50,50))
            cv2.imshow("cropped", resized_image)

            if (key == 32 or isPhoto):
                imageName = datetime.now().strftime('%Y%m%d_%H:%M:%S.jpg')
                cv2.imwrite("data/data_images/Persons/%s/%s.jpg" %(name, imageName), img)
                print("%s has been saved!" % imageName)

                resized_image = np.asarray(resized_image, dtype='uint8' )
                sample = resized_image.reshape((1,2500))
                sample = sample/255
                samples = np.append(samples,sample,0)
                n=n+1;
                isClose = True
                isPhoto = False

        cv2.imshow('image', vis)
        
        if key == 27:
            break
    
    print("Data has been saved!!. Run \"training.py\"")    
    np.savetxt('data/data_images/Persons/%s/generalsamples.data'%name, samples)
    cv2.destroyAllWindows()


if __name__ == '__main__':
    import sys, getopt
    print(__doc__)
    main()   
