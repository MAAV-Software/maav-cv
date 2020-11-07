import cv2

cap = cv2.VideoCapture(0)

# Check if the webcam is opened correctly
if not cap.isOpened():
    raise IOError("Cannot open webcam")

dest = '_________'
i = 0
while i < 100:
    ret, frame = cap.read()
    trans = cv2.medianBlur(frame,1)
    cv2.imwrite(dest, trans)

    i++

cap.release()
cv2.destroyAllWindows()