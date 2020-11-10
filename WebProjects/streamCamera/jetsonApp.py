#!/usr/bin/python
from flask import Flask, render_template, Response
import cv2
import numpy as np
import time
import jetson.utils
from cameraCalib import remapImage

app = Flask(__name__)

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

@app.route('/')
def index():
    """Video streaming home page."""
    name = 'Jetson Nano'
    return render_template('index.html', title='Video Streaming', username=name)

def gen():
    """Video streaming generator function."""
    #print("Grabbing Images ...")
    try:
        while True:
            image, width, height = camera.CaptureRGBA(zeroCopy=1)
            jetson.utils.cudaDeviceSynchronize()
            arr = jetson.utils.cudaToNumpy(image, width, height, 4)
            iImg = cv2.cvtColor(arr, cv2.COLOR_RGBA2RGB).astype(np.uint8)
            img_data = cv2.cvtColor(iImg, cv2.COLOR_RGB2BGR)
            image = remapImage(img_data)
            image = cv2.resize(image, None, fx=0.5, fy=0.5, interpolation = cv2.INTER_LINEAR)

            ret, jpeg = cv2.imencode('.jpg', image)
            frame = jpeg.tobytes()
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')

    except cv2.error as e:
        print("\n ---- Error, Closing ---")
        print(e)
        camera.Close()

@app.route('/video_feed')
def video_feed():
    """Video streaming route. Put this in the src attribute of an img tag."""
    return Response(gen(),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/imagepage', methods=['GET', 'POST'])
def done():
   return render_template('image.html', title="Camera")

if __name__ == '__main__':
    #app.run(host='0.0.0.0', port=5010, debug=True)
    app.run(host='0.0.0.0', port=5040)
