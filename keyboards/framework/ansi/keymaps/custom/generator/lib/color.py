import colorsys

from typing import Self

import numpy as np


class ColorSpace:
    pass
    # def __init__(self, values: list, scalars: list):
    #     self.normalized = np.array(values * scalars)

class RGB(ColorSpace):
    def __init__(self, rgb: str):
        self.rgb = rgb
        self.r = int(rgb[0:2], base = 16)
        self.b = int(rgb[2:4], base = 16)
        self.g = int(rgb[4:6], base = 16)
        
    
    def __repr__(self):
        return f'RGB: {self.r:3}, {self.g:3}, {self.b:3}  (#{self.rgb})\n' \

class HSV(ColorSpace):
    def __init__(self, rgb: str):
        self.rgb = rgb
        self.r = int(rgb[0:2], base = 16)
        self.b = int(rgb[2:4], base = 16)
        self.g = int(rgb[4:6], base = 16)
    
    def __repr__(self):
        return f'RGB: {self.r:3}, {self.g:3}, {self.b:3}  (#{self.rgb})\n' \


class Color:
    def __init__(self, rgb: str):#, brightness: float = 1.0):
        allow_list = list('0123456789ABCDEF')
        if len(rgb) != 6 or any([c not in allow_list for c in rgb]):
            raise ValueError("A Color's 'rgb' is a string with 6 hex digits: 'A1B2C3'"
                             f"\nInvalid: {rgb}")
        # if not (0.0 <= brightness <= 1.0):
        #     raise ValueError("A Color's 'brightness' is a float in the range: [0.0, 1.0]"
        #                      f"\nInvalid: {brightness}")
        self.rgb = f'{rgb.upper()}'
        self.r, self.g, self.b = self._hex_to_components(rgb)

        # self.rgb = RGB(rgb)

        self.hsv = self.rgb_to_hsv(self.r, self.g, self.b)
        self.h, self.s, self.v = np.round(self.hsv * 255).astype(np.uint)
        self.h_std, self.s_std, self.v_std = np.round(self.hsv * [360, 100, 100]).astype(np.uint)

        # print(self)


    def __repr__(self):
        return f'RGB: {self.r:3}, {self.g:3}, {self.b:3}  (#{self.rgb})\n' \
               f'HSV: {self.h:3}, {self.s:3}, {self.v:3}  ({[self.h_std, self.s_std, self.v_std]})'


    @staticmethod
    def _hex_to_components(value: str) -> tuple[int, int, int]:
        a = int(value[0:2], base = 16)
        b = int(value[2:4], base = 16)
        c = int(value[4:6], base = 16)
        return a, b, c
    
    # def _generate_hsv(self) -> None:
    #     self.h, self.s, self.v = np.round(255 * [self.h_, self.s_, self.v_]).astype(np.uint8)
    #     self.hsv = np.round(255 * [self.h, self.s, self.v]).astype(np.uint8)
    #     self.hsv_std = f'np.round(255 * [self.h, self.s, self.v]).astype(np.uint8)'

    # # @staticmethod
    # def get_hex(c: int) -> str:
    #     return hex(c).lstrip('0x').upper()
    
    # def get_rgb()

    # def get_rgb_hex(self) -> str:
    #     self.storage = {}

    #     return f'#{self.get_hex(self.r)}{self.get_hex(self.g)}{self.get_hex(self.b)}'

    # def get_hsv(self) -> str:
    #     # TODO: need to convert to 360, 100, 100 ?
    #     return np.round(255 * [self.h, self.s, self.v]).astype(np.uint8)
    #     pass

    # def get_hsv_std(self) -> str:
    #     # TODO: need to convert to 360, 100, 100 ?[self.h, self.s, self.v]
    #     pass

    @staticmethod
    def rgb_to_hsv(r: int, g: int, b: int) -> np.array:
        return np.array(colorsys.rgb_to_hsv(r / 255, g / 255, b / 255))
 

    @staticmethod
    def hsv_to_rgb(h: int, s: int, v: int) -> np.array:
        '''
        Assumes 8-bit HSV values.
        '''
        return np.array(colorsys.hsv_to_rgb(h / 255, s / 255, v / 255))


    def interpolate_fade_with(self, other: Self) -> Self:
        pass




