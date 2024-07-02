import cv2 as cv
import numpy as np
from numpy._typing import NDArray

class Vision:
    def __init__(self, imgSubPath: str, method=cv.TM_CCOEFF_NORMED) -> None:
        self.__imgSub = cv.imread(imgSubPath, cv.IMREAD_UNCHANGED)
        self.__w = self.__imgSub.shape[1]
        self.__h = self.__imgSub.shape[0]
        self.__method = method

    # Returns a list of rectangles (x, y, w, h) if found some matches
    def find(self, img: NDArray, threshold = 0.5):
        threshold = max(min(threshold, 1.), 0.)

        # Find points
        result = cv.matchTemplate(img, self.__imgSub, self.__method)
        locations = np.where(result >= threshold)
        locations = list(zip(*locations[::-1]))
        minVal, maxVal, minLoc, maxLoc = cv.minMaxLoc(result)
        print('maxVal: ', maxVal, sep='', end='\r', flush=True)

        # Remove overlapping rectangles by groping them
        rectangles = []
        for x, y in locations:
            rect = [x, y, self.__w, self.__h]
            rectangles.append(rect)
            rectangles.append(rect) # If rectangle is single duplication will save it from removing
        rectangles, _ = cv.groupRectangles(rectangles, 1, 0.5)

        return rectangles

    # Returns a list of points (cx, cy) from rectangles
    def getPoints(self, rects):
        points = []
        for x, y, w, h in rects:
            center = (x + int(w * 0.5), y + int(h * 0.5))
            points.append(center)

        return points

    @staticmethod
    def drawRects(img, rects, lineColor = (0, 255, 0), lineThickness = 1, lineType = cv.LINE_4):
        for x, y, w, h in rects:
            cv.rectangle(img, (x, y), (x + w, y + h), lineColor, lineThickness, lineType)

    @staticmethod
    def drawCrosshairs(img, points, markerColor = (255, 0, 255), markerType = cv.MARKER_CROSS):
        for p in points:
            cv.drawMarker(img, p, markerColor, markerType)

