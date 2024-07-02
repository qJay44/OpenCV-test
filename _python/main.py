import cv2 as cv
from windowcapture import WindowCapture
from time import time
from vision import *

print(WindowCapture.getVisibleWindows())

winCap = WindowCapture(0x905dc)
lastTime = time() + 1.
v = Vision('rl-ball.jpg')

while True:
    currentTime = time()
    deltaTime = currentTime - lastTime

    v.find(winCap.getScreenshot(), 0.3, DEBUG_MODE_RECTANGLES)

    if cv.waitKey(1) == ord('q') or not cv.getWindowProperty('Result', cv.WND_PROP_VISIBLE):
        break

    lastTime = currentTime

cv.destroyAllWindows()

