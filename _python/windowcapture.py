import numpy as np
import win32gui
import win32ui
import win32con
from numpy._typing import NDArray

class WindowCapture:
    __BORDER_PIXELS = 8
    __TITLEBAR_PIXELS = 30

    # Capture the window by its handle number (or capture whole screen)
    def __init__(self, hwnd = None) -> None:
        if hwnd is None:
            self.__hwnd = win32gui.GetDesktopWindow()
        else:
            self.__hwnd = hwnd
            if not win32gui.IsWindowVisible(self.__hwnd):
                msg = f'(WindowCapture) The window is not visible (hwnd = {hex(self.__hwnd)})'
                raise Exception(msg)

    # Returns a list of dictionaries of window's hexadicimal handle and window's title name
    @staticmethod
    def getVisibleWindows() -> list[dict[str, str]]:
        winList = []
        def winEnumHandler(hwnd, _):
            if win32gui.IsWindowVisible(hwnd):
                winList.append({hex(hwnd): win32gui.GetWindowText(hwnd)})

        win32gui.EnumWindows(winEnumHandler, None)

        return winList

    def getWinSize(self) -> tuple[int, int]:
        rect = win32gui.GetWindowRect(self.__hwnd)

        w, h = rect[2] - rect[0], rect[3] - rect[1]
        w = w - (self.__BORDER_PIXELS * 2)
        h = h - self.__TITLEBAR_PIXELS - self.__BORDER_PIXELS
        self.__offsetX = rect[0] + self.__BORDER_PIXELS
        self.__offsetY = rect[1] + self.__TITLEBAR_PIXELS

        return (rect[2] - rect[0], rect[3] - rect[1])

    def getScreenshot(self) -> NDArray:
        w, h = self.getWinSize()

        wDC = win32gui.GetWindowDC(self.__hwnd)
        dcObj = win32ui.CreateDCFromHandle(wDC)
        cDC = dcObj.CreateCompatibleDC()
        bmp = win32ui.CreateBitmap()
        bmp.CreateCompatibleBitmap(dcObj, w, h)
        cDC.SelectObject(bmp)
        cDC.BitBlt((0, 0), (w, h), dcObj, (self.__BORDER_PIXELS, self.__TITLEBAR_PIXELS), win32con.SRCCOPY)

        bmpStrLike = bmp.GetBitmapBits(True)
        screenshot = np.frombuffer(bmpStrLike, dtype='uint8')
        screenshot.shape = (h, w, 4)
        screenshot = screenshot[..., :3] # Drop alpha
        screenshot = np.ascontiguousarray(screenshot)

        # Free resources
        dcObj.DeleteDC()
        cDC.DeleteDC()
        win32gui.ReleaseDC(self.__hwnd, wDC)
        win32gui.DeleteObject(bmp.GetHandle())

        return screenshot

    def getScreenPosition(self, pos: tuple[int, int]) -> tuple[int, int]:
        return (pos[0] + self.__offsetX, pos[1] + self.__offsetY)

