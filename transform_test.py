import cv2
import numpy as np
from matplotlib import pyplot as plt


def affineTranform(img, Sx=1, Sy=1, Hx=0, Hy=0, Tx=0, Ty=0):
  rows, cols = img.shape[::-1]
  rows = (int) ((Sy + abs(Hy)) * rows)
  cols = (int) ((Sx + abs(Hx)) * cols)
  if(Sx < 0):
    Tx = Tx + (Sx * cols - 1)
  if(Sy < 0):
    Ty = Ty + (Sy * rows - 1)
  if(Hx < 0):
    Tx = Tx - (Hx * rows)
  if(Hy < 0):
    Ty = Ty - (Hy * cols)
  M = np.float32([[Sx, Hx, Tx],
                  [Hy, Sy, Ty]])
  img2 = img.copy()
  translated_img = cv2.warpAffine(img, M, (cols, rows), img2, 0, cv2.BORDER_TRANSPARENT, 0)
  return translated_img