import numpy as np
import cv2 as cv

cap = cv.VideoCapture(0)

# Check if the webcam is opened correctly
if not cap.isOpened():
    raise IOError("Cannot open webcam")

dest = '_________'
i = 0
orb = cv.ORB_create()

while i < 100:

    ret, frame = cap.read()
    gray = cv.cvtColor(frame,cv.COLOR_BGR2GRAY)

    kp = orb.detect(gray,None)
    kp, des = orb.compute(gray, kp)

    frame = cv.drawKeypoints(gray, kp, None, color=(0,255,0), flags=0)

    cv.imwrite(dest, frame)

    i++

cap.release()
cv.destroyAllWindows()