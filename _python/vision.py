import cv2 as cv
import numpy as np

DEBUG_MODE_RECTANGLES = 0x1
DEBUG_MODE_POINTS = 0x2

class Vision:
    def __init__(self, imgSubPath: str, method=cv.TM_CCOEFF_NORMED) -> None:
        self.__imgSub = cv.imread(imgSubPath, cv.IMREAD_UNCHANGED)
        self.__w = self.imgSub.shape[1]
        self.__h = self.imgSub.shape[0]
        self.__method = method

    def find(self, img, threshold = 0.5, debugMode = 0):
        LINE_COLOR = (0, 255, 0)
        LINE_THICKNESS = 1
        LINE_TYPE = cv.LINE_4
        MARKER_COLOR = (255, 0, 255)
        MARKER_TYPE = cv.MARKER_CROSS

        # Validate parameters
        if img is None or self.__imgSub is None:
            exit(1)
        threshold = max(min(threshold, 1.), 0.)

        # Find points
        result = cv.matchTemplate(img, self.__imgSub, self.__method)
        locations = np.where(result >= threshold)
        locations = list(zip(*locations[::-1]))
        minVal, maxVal, minLoc, maxLoc = cv.minMaxLoc(result)
        print(f'maxVal: ', maxVal, sep='', end='\r', flush=True)

        # Remove overlapping rectangles by groping them
        rectangles = []
        for x, y in locations:
            rect = [x, y, self.__w, self.__h]
            rectangles.append(rect)
            rectangles.append(rect) # If rectangle is single duplication will save it from removing
        rectangles, _ = cv.groupRectangles(rectangles, 1, 0.5)

        # Create a list with discovered points
        points = []
        if len(rectangles):
            for x, y, w, h in rectangles:
                center = (x + int(w * 0.5), y + int(h * 0.5))
                points.append(center)

                if debugMode & DEBUG_MODE_RECTANGLES:
                    cv.rectangle(img, (x, y), (x + w, y + h), LINE_COLOR, LINE_THICKNESS, LINE_TYPE)

                if debugMode & DEBUG_MODE_POINTS:
                    cv.drawMarker(img, center, MARKER_COLOR, MARKER_TYPE)

        if debugMode:
            cv.imshow('Result', img)

        return points

