####################################################################################
#
# https://github.com/FrameworkComputer/qmk_firmware/tree/v0.2.9
# https://docs.qmk.fm/
# https://jinja.palletsprojects.com/en/3.0.x/templates/
#
# - Generate compile_commands.json:
#     qmk generate-compilation-database -kb framework/ansi -km custom
#
# - Build and flash firmware:
#     qmk clean
#     qmk compile -kb framework/ansi -km custom
#     qmk flash -kb framework/ansi -km custom
#
#     o Important: Changes made in VIA may override some changes in keymap.c.
#          * The EEPROM needs erased so the flash can take full effect.
#          * Keycode can be assigned in keymap.c and in VIA (with ANY key mapping).
#
# - Print debugging:
#     uprintf("message\n");
#     qmk console
#
# - RGB -> HSV converter:
#     ~/git/qmk_firmware/rgb2hsv.ipynb
#
####################################################################################
#
# *** TODO ***
#
# - Backlight timeout:
#     https:#docs.qmk.fm/custom_quantum_functions#example-void-housekeeping-task-user-void-implementation
#
# - "breathe" fading indicator
#     Can we make a generic way to assign indicators to a key?
#
# - Bidirectional communications to link KB and macropad controls?
#     https:#docs.qmk.fm/features/rawhid
#
####################################################################################


from jinja2 import Environment, FileSystemLoader, select_autoescape

# from lib.key import Key
from lib.effect import Effect, Solid, Blink, Fade, Blend
from lib.color import Color
from lib.style import Style
from lib.layer import Layer

#
# Keymap {
#     Layer {
#         Key {
#             location: (int, int)
#             leds = list(LED)
#                 LED {
#                     index: int
#                     Style {
#                         Effect {}
#                         Color {}
#                         Color {} (optional)
#                         
#                     }
#                 }
#         }
#         Key {

#         }
#     }
#     Layer {
        
#     }
#     Layer {
        
#     }
# }

# def generate_keys() -> dict [str, Key]:
#     return {
#         # 'q': Key()
#     }
# def generate_keygroups(keys) -> dict [str, List[key]]:
#     return {
#         # 'erase': [keys['delete'], keys['backspace']]
#     }

def generate_effects() -> dict[str, Effect]:
    return {
        'solid': Solid(1.0),
        'blink': Blink(250, 1.0),
        'fade':  Fade(100, 0.1, 0.9),
    }

def generate_colors() -> dict[str, Color]:
    return {
        'blue':          Color('0000FF'),
        'red':           Color('FF0000'),
        'yellow':        Color('FFFF00'),
        'white':         Color('FFFFFF'),
        'fuchsia':       Color('FF00FF'),
        'aqua':          Color('22FFFF'),
        'blue_med':      Color('3333FF'),
        'orange_dark':   Color('FF9911'),
        'crimson':       Color('FF3344'),
        'lime':          Color('00FF00'),
        'slateblue_med': Color('6666FF'),
        'violet':        Color('FF66DD'),
        'tomato':        Color('FF6633'),
    }

def generate_styles() -> dict[str, Style]:
    effects = generate_effects()
    colors = generate_colors()
    return {
        'solid_fuchsia':       Style(effects['solid'], colors['fuchsia']),
        'solid_violet':        Style(effects['solid'], colors['violet']),
        'solid_red':           Style(effects['solid'], colors['red']),
        'solid_crimson':       Style(effects['solid'], colors['crimson']),
        'solid_tomato':        Style(effects['solid'], colors['tomato']),
        'solid_orange_dark':   Style(effects['solid'], colors['orange_dark']),
        'solid_green':         Style(effects['solid'], colors['green']),
        'solid_blue':          Style(effects['solid'], colors['blue']),
        'solid_blue_med':      Style(effects['solid'], colors['blue_med']),
        'solid_slateblue_med': Style(effects['solid'], colors['slateblue_med']),
        'solid_aqua':          Style(effects['solid'], colors['aqua']),
        'solid_yellow':        Style(effects['solid'], colors['yellow']),

        'blink_blue_red':      Style(effects['blink'], colors['blue'], colors['red']),

        'fade_red':            Style(effects['fade'], colors['red']),
    }

def generate_layers(keys, styles) -> dict[str, Layer]:
# "escape",      "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12",                    "delete",
# "grave",        "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",   "0",             "minus", "equal", "backspace",
# "tab",               "q", "w", "e", "r", "t", "y", "u", "i", "o", "p",      "brace_left", "brace_right",  "backslash",
# "caps_lock",          "a", "s", "d", "f", "g", "h", "j", "k", "l",                "semicolon", "quote",       "enter",
# "shift_left",          "z", "x", "c", "v", "b", "n", "m",                  "comma", "period", "slash",  "shift_right",
# "ctrl_left", "fn", "super", "alt_left",       "space",      "alt_right", "ctrl_right",  "left", "up", "down", "right"
    return {
        Layer('base', {
            styles['solid_fuchsia']: {
                keys['q']: 'KC_Q',    keys['a']: 'KC_A',    keys['z']: 'KC_Z',
                keys['w']: 'KC_W',    keys['s']: 'KC_S',    keys['x']: 'KC_X',
                keys['e']: 'KC_E',    keys['d']: 'KC_D',    keys['c']: 'KC_C',
                keys['r']: 'KC_R',    keys['f']: 'KC_F',    keys['v']: 'KC_V',
                keys['t']: 'KC_T',    keys['g']: 'KC_G',    keys['b']: 'KC_B',
                keys['y']: 'KC_Y',    keys['h']: 'KC_H',    keys['n']: 'KC_N',
                keys['u']: 'KC_U',    keys['j']: 'KC_J',    keys['m']: 'KC_M',
                keys['i']: 'KC_I',    keys['k']: 'KC_K',
                keys['o']: 'KC_O',    keys['l']: 'KC_L',
                keys['p']: 'KC_P',
            },
            styles['solid_orange_dark']: {
                keys['grave']:       'KC_GRV',
                keys['minus']:       'KC_MINS',
                keys['equal']:       'KC_EQL',
                keys['brace_left']:  'KC_LBRC',
                keys['brace_right']: 'KC_RBRC',
                keys['backslash']:   'KC_BSLS',
                keys['semicolon']:   'KC_SCLN',
                keys['quote']:       'KC_QUOT',
                keys['comma']:       'KC_COMM',
                keys['period']:      'KC_DOT',
                keys['slash']:       'KC_SLSH',
            },
            styles['solid_aqua']: {
                keys['1']: 'KC_1',
                keys['2']: 'KC_2',
                keys['3']: 'KC_3',
                keys['4']: 'KC_4',
                keys['5']: 'KC_5',
                keys['6']: 'KC_6',
                keys['7']: 'KC_7',
                keys['8']: 'KC_8',
                keys['9']: 'KC_9',
                keys['0']: 'KC_0',
            },
            styles['solid_blue_med']: {
                keys['enter']: 'KC_ENT',
                keys['f1']:    'KC_F1',
                keys['f2']:    'KC_F2',
                keys['f3']:    'KC_F3',
                keys['f4']:    'KC_F4',
                keys['f5']:    'KC_F5',
                keys['f6']:    'KC_F6',
                keys['f7']:    'KC_F7',
                keys['f8']:    'KC_F8',
                keys['f9']:    'KC_F9',
                keys['f10']:   'KC_F10',
                keys['f11']:   'KC_F11',
                keys['f12']:   'KC_F12',
            },
            styles['solid_white']: {
                keys['space']: 'KC_SPC',
            },
            styles['solid_crimson']: {
                keys['delete']:    'KC_DEL',
                keys['backspace']: 'KC_BSPC',
            },
            styles['solid_red']: {
                keys['escape']:     'KC_ESC',
                keys['ctrl_left']:  'KC_LCTL',
                keys['ctrl_right']: 'KC_RCTL',
            },
            styles['solid_green']: {
                keys['shift_left']:  'KC_LSFT',
                keys['shift_right']: 'KC_RSFT',
            },
            styles['solid_blue']: {
                keys['alt_left']:  'KC_LALT',
                keys['alt_right']: 'KC_RALT',
            },
            styles['solid_yellow']: {
                keys['tab']: 'KC_TAB'
            },
            styles['blink_blue_red']: {
                keys['caps_lock']: `'KC_CAPS'
            },
            styles['solid_tomato']: {
                keys['fn']: 'MO(_FN)',
            },
            styles['solid_violet']: {
                keys['super']: 'KC_LGUI',
            },
            styles['solid_slateblue_med']: {
                keys['left']:  'KC_LEFT',
                keys['up']:    'KC_UP',
                keys['down']:  'KC_DOWN',
                keys['right']: 'KC_RGHT',
            },
        }),
        Layer ('fn', {
            styles['']: {

            },
        }),
        Layer ('fn_lock', {
            styles['']: {

            },
        }),
        Layer ('fm', {
            styles['']: {

            },
        }),
            # [
            #     '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______',
            #     '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______',
            #     '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______',
            #     '_______',      '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______',       '_______',
            #     '_______',            '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______', '_______',            '_______',
            #     '_______', '_______', '_______', '_______',                 '_______',                  '_______', '_______', '_______', '_______', '_______', '_______'
            # ]
    }


def main():

    loader = FileSystemLoader('template')
    env = Environment(
        loader = loader,
        autoescape = select_autoescape()
    )
    # env.compile_templates('build')
    template = env.get_template("keymap.c.jinja")

    # keys = generate_keys()
    keys = []
    styles = generate_styles()
    layers = generate_layers(keys, styles)

    # for layer in layers:
    #     for layer_style in layer:
    #         for key in keys:
    #             if key in layer_style:
    #                 pass

    print(layers)

    # for style in styles.values():
    #     print(style.name)

    # print(template.render(variables = None))

    exit(0)




if __name__ == '__main__':
    main()

