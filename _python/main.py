import cv2 as cv
from windowcapture import *
from time import time

print(WindowCapture.getVisibleWindows())
winCap = WindowCapture(0x20476)
lastTime = time() + 1.

while True:
    currentTime = time()
    deltaTime = currentTime - lastTime
    print(f'FPS: ', int(1. / deltaTime), sep='', end='\r', flush=True)

    cv.imshow('Compture Vision', winCap.getScreenshot())

    if cv.waitKey(1) == ord('q') or not cv.getWindowProperty('Compture Vision', cv.WND_PROP_VISIBLE):
        break

    lastTime = currentTime

cv.destroyAllWindows()
print('\nDone')

