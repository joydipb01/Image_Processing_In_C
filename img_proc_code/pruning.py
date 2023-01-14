import cv2
import numpy as np
import sys
from plantcv import plantcv as pcv

img = cv2.imread(sys.argv[1], 0)

thresh_img_name=input("Enter name of thresholded image: ")
skel_img_name=input("Enter name of skeletonized image: ")
prune_img_name=input("Enter name of pruned image: ")
prune_size=int(input("Enter n for thinning/dilation: "))

ret,img = cv2.threshold(img, 127, 255, 0)
cv2.imwrite(thresh_img_name,img)

size = np.size(img)
skel = np.zeros(img.shape, np.uint8)
element = cv2.getStructuringElement(cv2.MORPH_CROSS, (3,3))

while True:
    open = cv2.morphologyEx(img, cv2.MORPH_OPEN, element)
    temp = cv2.subtract(img, open)
    eroded = cv2.erode(img, element)
    skel = cv2.bitwise_or(skel,temp)
    img = eroded.copy()
    if cv2.countNonZero(img)==0:
        break
cv2.imwrite(skel_img_name,skel)
pruned_img, seg_img, seg_obj=pcv.morphology.prune(skel_img=skel, size=prune_size)

cv2.imwrite(prune_img_name, pruned_img)