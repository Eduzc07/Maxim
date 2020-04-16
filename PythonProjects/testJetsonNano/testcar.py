#!/usr/bin/python
import os
import cv2
import numpy as np
import time
import datetime

print("OpenCv version:" + cv2.__version__)

def CreateFolder():
    #----------------------------
    # Create Main Directory
    #----------------------------
    directory = './images/car/'
    if not os.path.exists(directory):
       os.makedirs(directory)

    dateTimeObj = datetime.datetime.now()
    folderDate = dateTimeObj.strftime("%d%m%Y")

    #----------------------------
    # Create day Directory
    #----------------------------
    directoryDay = './images/car/' + folderDate
    if not os.path.exists(directoryDay):
       os.makedirs(directoryDay)
       dir = directoryDay + '/bbox'
       if not os.path.exists(dir):
          os.makedirs(dir)

    dates = [directoryDay, folderDate]
    return dates

def CreatePositionNotes(dates):
    #----------------------------------------------------
    # Write in File
    #----------------------------------------------------
    #txtFile = "%s/bbox_%s.txt"%(directoryDay, folderDate)
    txtFile = "%s/bbox_%s.txt"%(dates[0], dates[1])
    if not os.path.isfile(txtFile):
        file = open(txtFile, "w")
        L = ["Name Image, X, Y, W , H\n"]
        file.writelines(L)
    else:
        file = open(txtFile, "a")

    return file

def SaveImages(newframe, cnt, file):
    dateTimeObj = datetime.datetime.now()
    folderDate = dateTimeObj.strftime("%d%m%Y")
    timestampStr = dateTimeObj.strftime("%d%m%Y_%H%M%S")
    nameImage = "./images/car/%s/%s.jpg"%(folderDate, timestampStr)
    cv2.imwrite(nameImage, newframe)

    x, y, w, h = cv2.boundingRect(cnt)
    cv2.rectangle(newframe, (x,y), (x+w,y+h),(0, 250, 0), 2)
    nameImage = "./images/car/%s/bbox/%s.jpg"%(folderDate, timestampStr)
    cv2.imwrite(nameImage, newframe)

    line = ["%s.jpg,%d,%d,%d,%d\n"%(timestampStr,x, y, w, h)]
    file.writelines(line)

def imageProcessing(frame, last_image, kernel):
    # convert to gray scale of each frames
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    blur = cv2.blur(gray, (9, 9))

    # nImage = blur - last_image
    #nImage = 125 + cv2.subtract(blur, last_image)
    nImage = cv2.absdiff(blur, last_image)

    # morpho = cv2.morphologyEx(nImage, cv2.MORPH_OPEN, kernel)
    # morpho = cv2.morphologyEx(morpho, cv2.MORPH_OPEN, kernel)
    morphology = cv2.morphologyEx(nImage, cv2.MORPH_GRADIENT, kernel)

    ret, thresh = cv2.threshold(morphology, 20, 255, cv2.THRESH_BINARY)
    contours, hierarchy = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    return [contours, blur]

def main():
    dates = CreateFolder()
    file = CreatePositionNotes(dates)
    [directoryDay, folderDate] = dates

    # capture frames from a video
    #cap = cv2.VideoCapture('./videos/13042020/13042020_075929_video_10.avi')
    #cap = cv2.VideoCapture('/home/edu/jetson_nano_videos/13042020/13042020_075929_video_10.avi')
    cap = cv2.VideoCapture('/home/edu/jetson_nano_videos/13042020/13042020_195028_video_10.avi')

    ret, frame = cap.read()
    height, width = frame.shape[:2]
    last_image = np.zeros((height, width), np.uint8)
    track_image = np.zeros((height, width, 3), np.uint8)
    subs_image = np.zeros((height, width, 3), np.uint8)
    subs_image[:] = (0, 10, 10)

    # Initialize tracker with first frame and bounding box
    pxT = 840
    pyT = 250

    # Kernel
    kernel = np.ones((9, 21), np.uint8)

    # Save 9 Points
    arr_sp = [(0,0) ,(0,0), (0,0), (0,0), (0,0), (0,0), (0,0), (0,0), (0,0)]
    arr_ep = [(50, 400) ,(50, 400), (50, 400), (50, 400), (50, 400), (50, 400), (50, 400), (50, 400), (50, 400)]
    arr_color = [(0, 255, 0), (0, 0, 255), (255, 0, 0), (0, 255, 255), (255, 255, 0), (255, 0, 0), (0, 255, 255), (255, 255, 0), (255, 0, 0)]

    #----------------------------------------------------
    # Loop
    #----------------------------------------------------
    try:
        while True:
            # Start timer
            #---------------------------------------
            timer = cv2.getTickCount()

            # reads frames from a video
            ret, frame = cap.read()
            if not ret:
                break

            drawframe = frame.copy()
            [contours, last_image] = imageProcessing(frame, last_image, kernel)

            # Jump
            if (len(contours) > 50):
                continue

            arrayCnt = []
            for cnt in contours:
                if (cv2.contourArea(cnt) > 4000):
                    arrayCnt.append(cnt)

            for idx, cnt in enumerate(arrayCnt):
                x,y,w,h = cv2.boundingRect(cnt)
                cv2.rectangle(drawframe, (x,y), (x+w,y+h), arr_color[idx], 2)

                M = cv2.moments(cnt)
                cX = int(M["m10"]/M["m00"])
                cY = int(M["m01"]/M["m00"])

                if(cY > pyT):
                    if (cX > pxT and cX < pxT+10):
                        newframe = frame.copy()
                        SaveImages(newframe, cnt, file)

                        cv2.circle(track_image, (cX, cY), 7, (0,  255, 0), -1)
                        cv2.putText(track_image, "%d"%(idx+1), (cX, cY), cv2.FONT_HERSHEY_SIMPLEX, 0.75,  (0,  255, 0), 2)
                    else:
                        color = (0, 255, 255-(idx+1)*50)
                        #cv2.circle(track_image, (cX, cY), 8, color, -1)
                        arr_sp[idx] = (cX, cY)
                        cv2.line(track_image, arr_sp[idx], arr_ep[idx], color, 5)
                        arr_ep[idx] = (cX, cY)

            # End time
            #---------------------------------------
            # Calculate Frames per second (FPS)
            fps = cv2.getTickFrequency() / (cv2.getTickCount() - timer);

            # Display frames in a window
            dst = cv2.addWeighted(drawframe, 0.6, track_image, 0.4, 0)
            cv2.rectangle(dst, (840,250), (880, 620), (200 , 0, 200), 2)
            cv2.putText(dst, "FPS: %d"%fps, (400, 60), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (50,170,50), 2)
            cv2.imshow('Car Detection', dst)

            track_image = cv2.subtract(track_image, subs_image)

            # Wait for Esc key to stop
            if cv2.waitKey(33) == 27:
                break

    except cv2.error as e:
        print(e)
        print("\n ---- Error, Closing ---")

    file.close() #to change file access modes
    # De-allocate any associated memory usage
    cv2.destroyAllWindows()

if __name__ == "__main__":
    # execute only if run as a script
    main()
