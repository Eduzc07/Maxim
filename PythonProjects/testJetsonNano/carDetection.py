#!/usr/bin/python
import os
import cv2
import numpy as np
import time
import datetime
import jetson.utils
from cameraCalib import remapImage
import logging
import sys

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

def CreateLogFile(dates):
    #----------------------------------------------------
    # Write Log in File
    #----------------------------------------------------
    # Create A logger
    logging.basicConfig(filename='%s/car_%s.log'%(dates[0], dates[1]),
                        level=logging.DEBUG,
                        stream=sys.stdout,
                        filemode='a',
                        format='[%(asctime)s.%(msecs)03d][%(levelname)s]: %(message)s',
                        datefmt='%d/%m/%Y %H:%M:%S')

    logger = logging.getLogger('Car Detection')
    logger.info('###### Start ######')
    return logger

def CreateVideo(dates):
        #----------------------------------------------------
        # Write images into a video
        #----------------------------------------------------
    frame_width_video = 1700
    frame_height_video = 720
    fps = 6
    # Define the codec and create VideoWriter object.The output is stored in 'outpy.avi' file.
    out = cv2.VideoWriter('%s/video_%s.mp4'%(dates[0], dates[1]),
                          cv2.VideoWriter_fourcc(*'mp4v'),
                          fps,
                          (frame_width_video, frame_height_video))
    return out

def SaveImages(newframe, cnt, file, log, fps, nMobiles):
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

    #timesImage = dateTimeObj.strftime("%d%m%Y %H:%M:%S")
    #line = "[%s] %d) Mobile Detected --> FPS: %d"%(timesImage, nMobiles, fps)
    #print(line)
    line = "(%d) Mobile Detected --> FPS: %d"%(nMobiles, fps)
    log.info(line)

def imageProcessing(frame, last_image, kernel):
    # convert to gray scale of each frames
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    blur = cv2.blur(gray, (9, 9))
    nImage = cv2.absdiff(blur, last_image)
    morphology = cv2.morphologyEx(nImage, cv2.MORPH_GRADIENT, kernel)
    ret, thresh = cv2.threshold(morphology, 20, 255, cv2.THRESH_BINARY)
    contours, hierarchy = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    return [contours, blur]

def main():
    #---------------------------------------
    # Create files to write
    #---------------------------------------
    dates = CreateFolder()
    file = CreatePositionNotes(dates)
    log = CreateLogFile(dates)
    #video = CreateVideo(dates)

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

    # run Autoexposure
    n = 0
    while(n < 50):
        image, width, height = camera.CaptureRGBA(zeroCopy=1)
        n+=1

    print("Opening Camera ...")
    time.sleep(5)

    width, height = [1700, 720]
    # Create images
    last_image = np.zeros((height, width), np.uint8)
    track_image = np.zeros((height, width, 3), np.uint8)
    subs_image = np.zeros((height, width, 3), np.uint8)
    subs_image[:] = (0, 10, 10)

    # Initialize tracker with first frame and bounding box
    pxT = 1040
    pyT = 250
    fps = 0

    # Kernel
    kernel = np.ones((9, 21), np.uint8)

    # Save 9 Points
    #arr_sp = [(0,0) ,(0,0), (0,0), (0,0), (0,0), (0,0), (0,0), (0,0), (0,0)]
    #arr_ep = [(50, 400) ,(50, 400), (50, 400), (50, 400), (50, 400), (50, 400), (50, 400), (50, 400), (50, 400)]
    #arr_color = [(0, 255, 0), (0, 0, 255), (255, 0, 0), (0, 255, 255), (255, 255, 0), (255, 0, 0), (0, 255, 255), (255, 255, 0), (255, 0, 0)]

    nMobiles = 0
    #Create array for Images for save a Video
    #images_array = []
    imgcnt = 0

    print("Running Loop ...")
    #----------------------------------------------------
    # Loop
    #----------------------------------------------------
    try:
        while True:
            #---------------------------------------
            # Start timer
            #---------------------------------------
            timer = cv2.getTickCount()

            # reads frames from camera
            frame, width, height = camera.CaptureRGBA(zeroCopy=1)
            jetson.utils.cudaDeviceSynchronize()
            arr = jetson.utils.cudaToNumpy(frame, width, height, 4)
            img_data = cv2.cvtColor(arr, cv2.COLOR_RGBA2RGB).astype(np.uint8)
            img_data = cv2.cvtColor(img_data, cv2.COLOR_RGB2BGR)
            image = remapImage(img_data)

            if (image.size == 0):
                break

            # Save Copy
            drawframe = image.copy()
            [contours, last_image] = imageProcessing(image, last_image, kernel)

            # Jump
            if (len(contours) > 50):
                continue

            arrayCnt = []
            for cnt in contours:
                # if (cv2.contourArea(cnt) < 4000):
                #     continue
                #
                # if (imgcnt <= 1800):
                #     video.write(image)
                #     imgcnt += 1
                # else:
                #     print('Video finished!')

                if (cv2.contourArea(cnt) > 4000):
                    arrayCnt.append(cnt)
                    #video.write(image)

            for idx, cnt in enumerate(arrayCnt):
                #x, y, w, h = cv2.boundingRect(cnt)
                #cv2.rectangle(drawframe, (x,y), (x+w,y+h), arr_color[idx], 2)
                M = cv2.moments(cnt)
                cX = int(M["m10"]/M["m00"])
                cY = int(M["m01"]/M["m00"])

                if (cY > pyT):
                    if (cX > pxT and cX < pxT+30):
                        newframe = image.copy()
                        nMobiles += 1
                        SaveImages(newframe, cnt, file, log, fps, nMobiles)

                    #     cv2.circle(track_image, (cX, cY), 7, (0,  255, 0), -1)
                    #     cv2.putText(track_image, "%d"%(idx+1), (cX, cY), cv2.FONT_HERSHEY_SIMPLEX, 0.75,  (0,  255, 0), 2)
                    # else:
                    #     color = (0, 255, 255-(idx+1)*50)
                    #     cv2.circle(track_image, (cX, cY), 8, color, -1)
                    #     if (cX > pxT):
                    #         (cX, cY) = (50, 400)
                    #     arr_sp[idx] = (cX, cY)
                    #     cv2.line(track_image, arr_sp[idx], arr_ep[idx], color, 5)
                    #     arr_ep[idx] = (cX, cY)

            # End time
            #---------------------------------------
            # Calculate Frames per second (FPS)
            fps = cv2.getTickFrequency() / (cv2.getTickCount() - timer);

            # Display frames in a window
#            dst = cv2.addWeighted(drawframe, 0.6, track_image, 0.4, 0)
            #cv2.rectangle(dst, (840,250), (880, 620), (200 , 0, 200), 2)
            #cv2.putText(dst, "FPS: %d"%fps, (400, 60), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (50,170,50), 2)
            #cv2.imshow('Car Detection', dst)

            track_image = cv2.subtract(track_image, subs_image)

            # Wait for Esc key to stop
            if cv2.waitKey(33) == 27:
                break

    except cv2.error as e:
        logger.error(e)
        logger.error("---- Error, Closing ---")

    file.close() #to change file access modes
    # close the camera
    camera.Close()
    # De-allocate any associated memory usage
    cv2.destroyAllWindows()

    #video.release()

if __name__ == "__main__":
    # execute only if run as a script
    main()
