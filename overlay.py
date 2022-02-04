import cv2 as cv
import numpy as np

lower = np.array([80,40,0])
upper = np.array([150,255,255])

MIN_MATCH_COUNT = 10
to_match = cv.imread('isolatedfront.png')
h,w = cv.cvtColor(to_match, cv.COLOR_BGR2GRAY).shape
hsv_m = cv.cvtColor(to_match, cv.COLOR_BGR2HSV)
mask_m = cv.inRange(hsv_m, lower, upper)
res_m = cv.bitwise_and(to_match, to_match, mask=mask_m)

sift = cv.SIFT_create()
kpm, desm = sift.detectAndCompute(res_m,None)
FLANN_INDEX_KDTREE = 1
index_params = dict(algorithm = FLANN_INDEX_KDTREE, trees = 5)
search_params = dict(checks = 50)
flann = cv.FlannBasedMatcher(index_params, search_params)

cap = cv.VideoCapture("mast-videos/mast_description.m4v") # replace with path to mast video
cap.set(1,1900) # set frame to start from

ran = False
while True:
    k = cv.waitKey(5) & 0xFF
    if k == 27:
        break
    elif not ran or k == 13:
        ran = True
        _, frame = cap.read()
        hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)
        
        mask = cv.inRange(hsv, lower, upper)
        res = cv.bitwise_and(frame, frame, mask=mask)

        kp, des = sift.detectAndCompute(res,None)

        if desm is None or des is None:
            draw_params = dict(matchColor = (0,255,0),
                            singlePointColor = None,
                            matchesMask = None,
                            flags = 2)
            img3 = cv.drawMatches(to_match, kpm, res, kp, [], None, **draw_params)
        else:
            matches = flann.knnMatch(desm,des,k=2)

            good = []
            for m,n in matches:
                if m.distance < 0.7*n.distance:
                    good.append(m)

            if len(good)>MIN_MATCH_COUNT:
                src_pts = np.float32([ kpm[m.queryIdx].pt for m in good ]).reshape(-1,1,2)
                dst_pts = np.float32([ kp[m.trainIdx].pt for m in good ]).reshape(-1,1,2)
                M, mask = cv.findHomography(src_pts, dst_pts, cv.RANSAC,5.0)
                matchesMask = mask.ravel().tolist()
                pts = np.float32([ [0,0],[0,h-1],[w-1,h-1],[w-1,0] ]).reshape(-1,1,2)
                dst = cv.perspectiveTransform(pts,M)
                img2 = cv.polylines(frame,[np.int32(dst)],True,255,3, cv.LINE_AA)
            else:
                print( "Not enough matches are found - {}/{}".format(len(good), MIN_MATCH_COUNT) )
                img2 = frame
                matchesMask = None

            draw_params = dict(matchColor = (0,255,0), # draw matches in green color
                            singlePointColor = None,
                            matchesMask = matchesMask, # draw only inliers
                            flags = 2)
            img3 = cv.drawMatches(to_match, kpm, img2, kp, good, None, **draw_params)
        cv.imshow('flann', img3)
        
cv.destroyAllWindows()
cap.release()