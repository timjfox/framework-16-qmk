


class Effect:
    pass

class Solid(Effect):
    def __init__(self, brightness: float):
        if not (0.0 <= brightness <= 1.0):
            raise ValueError("A Solid Effect's 'brightness' is a float in the range: [0.0, 1.0]"
                            f"\nInvalid: {brightness}")
        
        self.brightness = brightness


class Blink(Effect):
    def __init__(self, milliseconds: int, brightness: float, brightness_alt: float = None):
        if not (0.0 <= brightness <= 1.0):
            raise ValueError("A Blink Effect's 'brightness' is a float in the range: [0.0, 1.0]"
                            f"\nInvalid: {brightness}")
        if brightness_alt is not None and not (0.0 <= brightness_alt <= 1.0):
            raise ValueError("A Blink Effect's 'brightness_alt' is a float in the range: [0.0, 1.0]"
                            f"\nInvalid: {brightness_alt}")
        
        self.time = milliseconds
        self.brightness = brightness
        self.brightness_alt = brightness_alt


class Fade(Effect):
    def __init__(self, milliseconds: int, brightness_min: float, brightness_max: float):
        if not (0.0 <= brightness_min <= 1.0):
            raise ValueError("A Fade Effect's 'brightness_min' is a float in the range: [0.0, 1.0]"
                            f"\nInvalid: {brightness_min}")
        if not (0.0 <= brightness_max <= 1.0):
            raise ValueError("A Fade Effect's 'brightness_max' is a float in the range: [0.0, 1.0]"
                            f"\nInvalid: {brightness_max}")
        
        self.time = milliseconds
        self.brightness_min = brightness_min
        self.brightness_max = brightness_max


class Blend(Effect):
    def __init__(self, milliseconds: int, brightness: float, brightness_alt: float = None):
        if not (0.0 <= brightness <= 1.0):
            raise ValueError("A Blend Effect's 'brightness' is a float in the range: [0.0, 1.0]"
                            f"\nInvalid: {brightness}")
        if brightness_alt is not None and not (0.0 <= brightness_alt <= 1.0):
            raise ValueError("A Blend Effect's 'brightness_alt' is a float in the range: [0.0, 1.0]"
                            f"\nInvalid: {brightness_alt}")
        
        self.time = milliseconds
        self.brightness = brightness
        self.brightness_alt = brightness_alt

