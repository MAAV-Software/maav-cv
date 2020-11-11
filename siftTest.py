import numpy as np
import cv2 as cv

cap = cv2.VideoCapture(0)

# Check if the webcam is opened correctly
if not cap.isOpened():
    raise IOError("Cannot open webcam")

dest = '_________'
i = 0
while i < 100:
    ret, frame = cap.read()
    gray = cv.cvtColor(frame,cv.COLOR_BGR2GRAY)
    
    sift = cv.SIFT_create()
    kp = sift.detect(gray,None)

    frame = cv.drawKeypoints(gray,kp,frame)

    cv2.imwrite(dest, frame)

    i++

cap.release()
cv2.destroyAllWindows()