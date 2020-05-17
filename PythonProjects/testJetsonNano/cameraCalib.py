#!/usr/bin/python
'''
Module to Remap image 1280x720
using Camera Matrix and Distortion distortion coefficients
- *xml file is required
'''
import cv2

def remapImage(src, width = 1700):
    #Load Parameters
    fs = cv2.FileStorage("/home/edu/Workspace/pythonExamples/parameters/out_camera_data_1280x720.xml", cv2.FILE_STORAGE_READ)
    #fs = cv2.FileStorage("./parameters/out_camera_data_1280x720.xml", cv2.FILE_STORAGE_READ)

    mtx = fs.getNode("camera_matrix")
    dist = fs.getNode("distortion_coefficients")

    #Convert data in Matrix
    mtx = mtx.mat()
    dist = dist.mat()

    #get Size Image
    h, w = src.shape[:2]
    newcameramtx, roi = cv2.getOptimalNewCameraMatrix(mtx, dist,(w,h),1,(w,h))

    #undistort
    mapx, mapy = cv2.initUndistortRectifyMap(mtx, dist, None, newcameramtx,(w, h), 5)
    dst = cv2.remap(src, mapx, mapy, cv2.INTER_LINEAR)

    # crop the image
    x,y,w,h = roi
    dst = dst[y:y+h, x:x+w]

    #Resize Image to get around 1768x720
    ndst = cv2.resize(dst, None, fx=1.409, fy=1.409, interpolation = cv2.INTER_LINEAR)
    hr, wr = ndst.shape[:2]
    offset = (wr - width) / 2
    #Cut only to keep 1700x720
    #newRoi = [34, 0, 1700, 720]
    #newRoi = [524, 0, 720, 720]
    newRoi = [offset, 0, width, 720]

    x, y, w, h = newRoi
    finalImage = ndst[y:y+h, x:x+w]
    return finalImage

# if __name__ == "__main__":
#     img = cv2.imread('./images/08042020/08042020_093202.jpg', cv2.IMREAD_COLOR)
#     newImage = remapImage(img);
#     cv2.imshow('image', newImage)
#     cv2.waitKey(0)
#     cv2.destroyAllWindows()
