import cv2
import numpy as np
from matplotlib import pyplot as plt

from transform_test import affineTranform

img2 = cv2.imread('mast-images/front1.png',0)
img = img2.copy()
template2 = cv2.imread('mast-images/isolatedfront.png',0)
template = template2.copy()

# All the 6 methods for comparison in a list
methods = ['cv2.TM_CCOEFF', 'cv2.TM_CCOEFF_NORMED', 'cv2.TM_CCORR',
            'cv2.TM_CCORR_NORMED', 'cv2.TM_SQDIFF', 'cv2.TM_SQDIFF_NORMED']
meth = methods[4]
method = eval(meth)
scales = [0.1, 0.2,  0.5, 0.8,  0.9,  1, 1.1, 1.2, 1.5, 2,   3]
slants = [-2,  -1.5, -1,  -0.5, -0.2, 0, 0.2, 0.5, 1,   1.5, 2]
bestTL = (0, 0)
bestBR = (0, 0)
bestRes = 0
bestValue = -1
for sx in scales:
  for sy in scales:
    for hx in slants:
      for hy in slants:
        img = img2.copy()
        iw, ih = img.shape[::-1]
        template = affineTranform(template2, sx, sy, hx, hy)
        tw, th = template.shape[::-1]
        if iw <= tw or ih <= th:
          continue
        res = cv2.matchTemplate(img, template, method)
        min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)
        if bestValue > min_val or bestValue == -1:
          bestTL = min_loc
          bestBR = (bestTL[0] + tw, bestTL[0] + th)
          bestRes = res
          bestValue = min_val
          print(bestValue, bestTL, bestBR, sx, sy, hx, hy)

cv2.rectangle(img, bestTL, bestBR, 255, 10)
#plt.subplot(121)
#plt.imshow(bestRes,cmap = 'gray')
#plt.title('Matching Result'), plt.xticks([]), plt.yticks([])
#plt.subplot(122)
plt.imshow(img,cmap = "gray")
plt.title('Detected Point'), plt.xticks([]), plt.yticks([])
plt.suptitle(meth)
plt.show()