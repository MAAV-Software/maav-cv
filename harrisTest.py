import numpy as np
import cv2 as cv

cap = cv.VideoCapture(0)

# Check if the webcam is opened correctly
if not cap.isOpened():
    raise IOError("Cannot open webcam")

dest = '_________'
i = 0
while i < 100:
    ret, frame = cap.read()
    gray = cv.cvtColor(frame,cv.COLOR_BGR2GRAY)
    dst = cv.cornerHarris(gray,2,3,0.04)

    dst = cv.dilate(dst,None)
    frame[dst>0.01*dst.max()]=[0,0,255]

    cv2.imwrite(dest, frame)

    i++

cap.release()
cv.destroyAllWindows()