#adapted from opencv documentation examples
#commented code is general opencv functions, above their corresponding driver function

import numpy as np
import cv2 as cv
import camera-driver

color_queue = []
depth_queue = []

#cap = cv.VideoCapture(0)
pullFrames(color_queue, depth_queue)

# Check if the webcam is opened correctly
#if not cap.isOpened():
#    raise IOError("Cannot open webcam")

dest = '_________'
i = 0
while i < 100:
    #ret, frame = cap.read()
    frame, depth = getImage(color_queue, depth_queue)
    gray = cv.cvtColor(frame,cv.COLOR_BGR2GRAY)
    
    sift = cv.SIFT_create()
    kp = sift.detect(gray,None)

    frame = cv.drawKeypoints(gray,kp,frame)

    #cv.imwrite(dest, frame)

    i++

#cap.release()
cv.destroyAllWindows()