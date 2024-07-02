import cv2 as cv
from windowcapture import WindowCapture
from time import time
from vision import Vision

print(WindowCapture.getVisibleWindows())

winCap = WindowCapture(0x905dc)
lastTime = time() + 1.
vBall = Vision('rl-ball.jpg')

while True:
    if cv.waitKey(1) == ord('q') or not cv.getWindowProperty('Result', cv.WND_PROP_VISIBLE):
        break

    currentTime = time()
    deltaTime = currentTime - lastTime

    outputImg = winCap.getScreenshot()
    rects = vBall.find(outputImg, 0.5)

    vBall.drawRects(outputImg, rects)

    cv.imshow('Matches', outputImg)

    lastTime = currentTime

cv.destroyAllWindows()

