
from lib.effect import Effect, Solid, Blink, Fade, Blend
from lib.color import Color


class Style:
    def __init__(self, effect: Effect, color: Color, color_alt: Color = None):
        if effect is None:
            raise ValueError("A Style must have an effect.")
        if color is None:
            raise ValueError("A Style must have a color.")
    
        match effect:
            case type(Solid):
                if color_alt is not None:
                    print(f"WARNING: Solid Effect Style cannot have a 'color_alt' argument. (ignoring)")
            case type(Blink):
                if color_alt is None:
                    print(f"WARNING: Blink Effect Style missing 'color_alt' argument. Defaulting to: color_alt = None (off)")
                if color_alt.rgb == color.rgb:
                    print(f"WARNING: Blink Effect Style has equivalent arguments for 'color' and 'color_alt'. Defaulting to: color_alt = None (off)")
            case type(Blend):
                if color_alt is None:
                    raise TypeError("Blend Effect Style must have a 'color_alt' argument.")
                if color_alt.rgb == color.rgb:
                    print(f"WARNING: Blend Effect Style has equivalent arguments for 'color' and 'color_alt'. Defaulting to: color_alt = None (off)")
            case type(Fade):
                pass
            case _:
                pass

        self.effect = effect
        self.color = color
        self.color_alt = color_alt

        self.name = f'{effect.__class__.__name__.upper()}_{color.rgb}'
        if color_alt is not None:
            self.name += f'_{color_alt.rgb}'
        self.name += f'_{id(self)}'

