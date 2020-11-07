import os
from sys import exit
import cv2

script_path = os.path.realpath(__file__)
print(script_path)
print(__file__)

# Print helpful information
print("\nDisplaying sample image using OpenCV2. Press any key to exit. \
    \nDO NOT CLOSE THE WINDOW!!! You might need to restart your terminal.\n")

# Get the image from the disk
mast = cv2.imread(cv2.samples.findFile('mast.png'))
if mast is None:
    print("Image couldn't load. :(")
    exit(1)

# Mast is a cv2.Mat object. In C++, this is a cv2::Mat.

# Show the image
cv2.imshow('Mast for Mission 9', mast)

# Wait for a key press
key_wait = cv2.waitKey(0)
# while key_wait != 13:  # 13 is the return / enter key
#     key_wait = cv2.waitKey(0)
