import cv2 as cv
import numpy as np

LINE_COLOR = (0, 255, 0)
LINE_THICKNESS = 1
LINE_TYPE = cv.LINE_4
MARKER_COLOR = (255, 0, 255)
MARKER_TYPE = cv.MARKER_CROSS

DEBUG_MODE_RECTANGLES = 0x1
DEBUG_MODE_POINTS = 0x2

def findClickPositions(mainImgPath, subImgSubPath, threshold = 0.5, debugMode: np.uint = 0):
    # Read images
    img = cv.imread(mainImgPath, cv.IMREAD_UNCHANGED)
    imgSub = cv.imread(subImgSubPath, cv.IMREAD_UNCHANGED)

    # Validate parameters
    if img is None or imgSub is None:
        exit(1)
    threshold = max(min(threshold, 1.), 0.)

    wImgSub = imgSub.shape[1]
    hImgSub = imgSub.shape[0]

    # Find points
    result = cv.matchTemplate(img, imgSub, cv.TM_CCOEFF_NORMED)
    locations = np.where(result >= threshold)
    locations = list(zip(*locations[::-1]))

    # Remove overlapping rectangles by groping them
    rectangles = []
    for x, y in locations:
        rect = [x, y, wImgSub, hImgSub]
        rectangles.append(rect)
        rectangles.append(rect) # If rectangle is single duplication will save it from removing
    rectangles, _ = cv.groupRectangles(rectangles, 1, 0.5)

    # Create a list with discovered points
    if len(rectangles):
        points = []
        for x, y, w, h in rectangles:
            center = (x + int(w * 0.5), y + int(h * 0.5))
            points.append(center)

            if debugMode & DEBUG_MODE_RECTANGLES:
                cv.rectangle(img, (x, y), (x + w, y + h), LINE_COLOR, LINE_THICKNESS, LINE_TYPE)

            if debugMode & DEBUG_MODE_POINTS:
                cv.drawMarker(img, center, MARKER_COLOR, MARKER_TYPE)

        if debugMode:
            cv.imshow('Result', img)
            cv.waitKey()

    return points


findClickPositions('albion_farm.jpg', 'albion_cabbage.jpg', debugMode=DEBUG_MODE_POINTS)

