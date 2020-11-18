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

start = cv.getTickCount()

i = 0
while i < 100:
    #ret, frame = cap.read()
    frame, depth = getImage(color_queue, depth_queue)

    gray = cv.cvtColor(frame,cv.COLOR_BGR2GRAY)
    dst = cv.cornerHarris(gray,2,3,0.04)

    dst = cv.dilate(dst,None)
    frame[dst>0.01*dst.max()]=[0,0,255]

    #cv.imwrite(dest, frame)

    i++

end = cv.getTickCount()
time = (end - start)/cv.getTickFrequency()

#cap.release()
cv.destroyAllWindows()
print('Time: ' + time)