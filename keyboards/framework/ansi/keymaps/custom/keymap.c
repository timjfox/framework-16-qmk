///////////////////////////////////////////////////////////////////////////////
//
// *** Notes ***
//
// - Generate compile_commands.json:
//     qmk generate-compilation-database -kb framework/ansi -km custom
//
// - Build and flash firmware:
//     qmk clean
//     qmk compile -kb framework/ansi -km custom
//     qmk flash -kb framework/ansi -km custom
//
//     o Important: Changes made in VIA may override some changes in keymap.c.
//          * The EEPROM needs erased so the flash can take full effect.
//          * Keycode can be assigned in keymap.c and in VIA (with ANY key mapping).
//
// - Print debugging:
//     uprintf("message\n");
//     qmk console
//
// - RGB -> HSV converter:
//     custom/generator/rgb2hsv.ipynb
//
///////////////////////////////////////////////////////////////////////////////
//
// *** TODO ***
//
// - Backlight timeout:
//     https://docs.qmk.fm/custom_quantum_functions#example-void-housekeeping-task-user-void-implementation
//
// - Bidirectional communications to link KB and macropad controls?
//     https://docs.qmk.fm/features/rawhid
//
///////////////////////////////////////////////////////////////////////////////


#include QMK_KEYBOARD_H

#include "framework.h"


#define array_size(a) (sizeof(a) / sizeof((a)[0]))


enum custom_keycodes {
    LED_TOG = FN_LOCK + 1,
    LED_DEC,
    LED_INC,
    SUPER,
};

enum _layers {
  _BASE,
  _FN,
  _FN_LOCK,
  _FM,
  _SUPER,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     /*
     *         ┌─────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬────┐
     * 14 keys │ESC  │F1 │F2 │F3 │F4 │F5 │F6 │F7 │F8 │F9 │F10│F11│F12│Del │
     *         ├───┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┤
     * 14 keys │ ` │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │Backsp│
     *         ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬────┤
     * 14 keys │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ [ │ ] │ \  │
     *         ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴────┤
     * 13 keys │ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │ Enter │
     *         ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───────┤
     * 12 keys │ Shift  │ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │  Shift  │
     *         ├────┬───┼───┼───┼───┴───┴───┴───┴───┼───┼───┼───┴┬───┬────┤
     *         │    │   │   │   │                   │   │   │    │↑  │    │
     * 11 keys │Ctrl│FN │GUI│Alt│                   │Alt│Ctl│ ←  ├───┤  → │
     *         │    │   │   │   │                   │   │   │    │  ↓│    │
     *         └────┴───┴───┴───┴───────────────────┴───┴───┴────┴───┴────┘
     * 78 total
     */
    [_BASE] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4, KC_F5,  KC_F6, KC_F7, KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,  KC_5,   KC_6,  KC_7,  KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,  KC_T,   KC_Y,  KC_U,  KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,  KC_G,   KC_H,  KC_J,  KC_K,    KC_L,    KC_SCLN, KC_QUOT,     KC_ENT,
             KC_LSFT,     KC_Z,    KC_X,    KC_C,  KC_V,   KC_B,  KC_N,  KC_M,    KC_COMM, KC_DOT,  KC_SLSH,     KC_RSFT,
        KC_LCTL, MO(_FN), KC_LGUI, KC_LALT,        KC_SPC,               KC_RALT, KC_RCTL, KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT
    ),
    //  /*
    //  * Function layer
    //  *         ┌─────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬────┐
    //  * 14 keys │FN lk│Mut│vDn│vUp│Prv│Ply│Nxt│bDn│bUp│Scn│Air│Prt│App│Ins │
    //  *         ├───┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┤
    //  * 14 keys │   │   │   │   │   │   │   │   │   │   │   │L- │L+ │      │
    //  *         ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬────┤
    //  * 14 keys │     │   │   │   │   │   │   │   │   │   │Pau│   │   │    │
    //  *         ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴────┤
    //  * 13 keys │      │   │SRq│   │   │   │   │   │ScL│   │   │   │       │
    //  *         ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───────┤
    //  * 12 keys │        │   │   │   │   │   │Brk│   │   │   │   │         │
    //  *         ├────┬───┼───┼───┼───┴───┴───┴───┴───┼───┼───┼───┴┬───┬────┤
    //  *         │    │   │   │   │                   │   │   │    │PgU│    │
    //  * 11 keys │    │   │   │   │     L TOGGLE      │   │   │Home├───┤End │
    //  *         │    │   │   │   │                   │   │   │    │PgD│    │
    //  *         └────┴───┴───┴───┴───────────────────┴───┴───┴────┴───┴────┘
    //  * 78 total
    //  */
    [_FN] = LAYOUT(
        FN_LOCK, KC_MUTE, KC_VOLD, KC_VOLU, KC_MPRV, KC_MPLY, KC_MNXT, KC_BRID, KC_BRIU, KC_SCRN, KC_AIRP, KC_PSCR, KC_MSEL, KC_INS,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, LED_DEC, LED_INC, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_PAUS, _______, _______, _______,
        _______, _______, KC_SYRQ, _______, _______, _______, _______, _______, KC_SCRL, _______, _______, _______,          _______,
        _______,          _______, _______, _______, _______, KC_BRK,  _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______,          LED_TOG,                   _______, _______, KC_HOME, KC_PGUP, KC_PGDN, KC_END
    ),
    // Function lock layer
    [_FN_LOCK] = LAYOUT(
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, MO(_FM), _______, _______,          _______,                   _______, _______, _______, _______, _______, _______
    ),
    // Locked+temporary FN (back to base plus extra keys)
    [_FM] = LAYOUT(
        FN_LOCK, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_CLEAR_EEPROM,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______,          _______,                   _______, _______, _______, _______, _______, _______
    ),
    [_SUPER] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______,          _______,                   _______, _______, _______, _______, _______, _______
    ),
};


// index for each LED
typedef enum LED {
   LED_TAB_1 = 0,
   LED_TAB_0 = 1,
   LED_E = 2,
   LED_W = 3,
   LED_R = 4,
   LED_Q = 5,
   LED_U = 6,
   LED_T = 7,
   LED_Y = 8,
   LED_5 = 9,
   LED_7 = 10,
   LED_4 = 11,
   LED_3 = 12,
   LED_2 = 13,
   LED_6 = 14,
   LED_1 = 15,
   LED_TILDE = 16,
   LED_8 = 17,
   LED_F3 = 18,
   LED_F2 = 19,
   LED_F4 = 20,
   LED_F1 = 21,
   LED_F5 = 22,
   LED_ESC_1 = 23,
   LED_F6 = 24,
   LED_ESC_0 = 25,
   LED_F7 = 26,
   LED_Z = 27,
   LED_LEFT_SHIFT_2 = 28,
   LED_X = 29,
   LED_LEFT_SHIFT_1 = 30,
   LED_C = 31,
   LED_LEFT_SHIFT_0 = 32,
   LED_V = 33,
   LED_LEFT_CTRL_0 = 34,
   LED_B = 35,
   LED_A = 36,
   LED_D = 37,
   LED_CAPS_LOCK_1 = 38,
   LED_G = 39,
   LED_H = 40,
   LED_S = 41,
   LED_CAPS_LOCK_2 = 42,
   LED_F = 43,
   LED_CAPS_LOCK_0 = 44,
   LED_LEFT_CTRL_1 = 45,
   LED_SUPER = 46,
   LED_LEFT_ALT = 47,
   LED_COLON = 48,
   LED_J = 49,
   LED_K = 50,
   LED_L = 51,
   LED_QUOTE = 52,
   LED_ENTER_0 = 53,
   LED_END_BRACE = 54,
   LED_PIPE = 55,
   LED_ENTER_1 = 56,
   LED_START_BRACE = 57,
   LED_I = 58,
   LED_O = 59,
   LED_P = 60,
   LED_0 = 61,
   LED_EQUAL = 62,
   LED_HYPHEN = 63,
   LED_BACKSPACE_0 = 64,
   LED_BACKSPACE_1 = 65,
   LED_F11_0 = 66,
   LED_F8 = 67,
   LED_F10 = 68,
   LED_9 = 69,
   LED_F12 = 70,
   LED_F11_1 = 71,
   LED_DELETE_0 = 72,
   LED_DELETE_1 = 73,
   LED_F9 = 74,
   LED_PERIOD = 75,
   LED_N = 76,
   LED_M = 77,
   LED_COMMA = 78,
   LED_QUESTION = 79,
   LED_RIGHT_SHIFT_0 = 80,
   LED_UP = 81,
   LED_RIGHT_SHIFT_1 = 82,
   LED_RIGHT_SHIFT_2 = 83,
   LED_RIGHT_ALT = 84,
   LED_SPACE_3 = 85,
   LED_SPACE_4 = 86,
   LED_SPACE_5 = 87,
   LED_RIGHT_CTRL = 88,
   LED_LEFT = 89,
   LED_DOWN = 90,
   LED_RIGHT = 91,
   LED_RIGHT_SHIFT_3 = 92,
   LED_FN = 93,
   LED_SPACE_0 = 94,
   LED_SPACE_1 = 95,
   LED_SPACE_2 = 96,
   LED_COUNT
} LED;


// various key groups
static const LED LED_ESCAPE[] = {LED_ESC_0, LED_ESC_1};

static const LED LED_TAB[] = {LED_TAB_0, LED_TAB_1};
static const LED LED_CAPS_LOCK[] = {LED_CAPS_LOCK_0, LED_CAPS_LOCK_1, LED_CAPS_LOCK_2};

static const LED LED_ENTER[] = {LED_ENTER_0, LED_ENTER_1};
static const LED LED_SPACE[] = {LED_SPACE_0, LED_SPACE_1, LED_SPACE_2, LED_SPACE_3, LED_SPACE_4, LED_SPACE_5};

static const LED LED_CTRL[] = {LED_LEFT_CTRL_0, LED_LEFT_CTRL_1, LED_RIGHT_CTRL};
static const LED LED_ALT[] = {LED_LEFT_ALT, LED_RIGHT_ALT};

static const LED LED_SHIFT[] = {LED_LEFT_SHIFT_0, LED_LEFT_SHIFT_1, LED_LEFT_SHIFT_2,
                                LED_RIGHT_SHIFT_0, LED_RIGHT_SHIFT_1, LED_RIGHT_SHIFT_2, LED_RIGHT_SHIFT_3};
static const LED LED_LEFT_SHIFT[] = {LED_LEFT_SHIFT_0, LED_LEFT_SHIFT_1, LED_LEFT_SHIFT_2};
static const LED LED_RIGHT_SHIFT[] = {LED_RIGHT_SHIFT_0, LED_RIGHT_SHIFT_1, LED_RIGHT_SHIFT_2, LED_RIGHT_SHIFT_3};

static const LED LED_ERASE[] = {LED_BACKSPACE_0, LED_BACKSPACE_1, LED_DELETE_0, LED_DELETE_1};
static const LED LED_BACKSPACE[] = {LED_BACKSPACE_0, LED_BACKSPACE_1};
static const LED LED_DELETE[] = {LED_DELETE_0, LED_DELETE_1};

static const LED LED_ARROWS[] = {LED_LEFT, LED_RIGHT, LED_UP, LED_DOWN};

static const LED LED_ALPHABET[] = {LED_A, LED_B, LED_C, LED_D, LED_E, LED_F, LED_G, LED_H, LED_I, LED_J, LED_K, LED_L, LED_M,
                                   LED_N, LED_O, LED_P, LED_Q, LED_R, LED_S, LED_T, LED_U, LED_V, LED_W, LED_X, LED_Y, LED_Z};

static const LED LED_NUMBERS[] = {LED_0, LED_1, LED_2, LED_3, LED_4, LED_5, LED_6, LED_7, LED_8, LED_9};

static const LED LED_FKEYS[] = {LED_F1, LED_F2, LED_F3, LED_F4, LED_F5, LED_F6, LED_F7, LED_F8, LED_F9, LED_F10, LED_F11_0, LED_F11_1, LED_F12};

static const LED LED_SYMBOLS[] = {LED_TILDE, LED_HYPHEN, LED_EQUAL, LED_START_BRACE, LED_END_BRACE,
                                  LED_PIPE, LED_COLON, LED_QUOTE, LED_COMMA, LED_PERIOD, LED_QUESTION};


// LED brightness control
static const uint8_t VS[] = {0, 8, 16, 32, 64, 128, 255};
static const uint8_t V_OFF = 0;
static const uint8_t V_MIN = 1;
static const uint8_t V_MAX = array_size(VS) - 1;
static const uint8_t V_DEF = MIN(4, V_MAX);
static       uint8_t v_idx = V_DEF;

void increase_led_brightness(void) {
    if (v_idx < V_MAX) {
        v_idx++;
    }
}

void decrease_led_brightness(void) {
    if (v_idx > V_MIN) {
        v_idx--;
    }
}

void toggle_led(void) {
    static uint8_t v_idx_save = V_DEF;
    if (v_idx == 0) {
        v_idx = v_idx_save;
    } else {
        v_idx_save = v_idx;
        v_idx = 0;
    }
}


// LED color control
#define COLOR(hsv) (HSV){hsv}

void set_led_color_rgb(const uint8_t idx, const RGB rgb) {
    rgb_matrix_set_color(idx, rgb.r, rgb.g, rgb.b);
}

void set_led_color_hsv(const uint8_t idx, HSV hsv) {
    // hsv.v = VS[v_idx];
    set_led_color_rgb(idx, hsv_to_rgb(hsv));
}

void set_led_color_hsv_unique_v(const uint8_t idx, HSV hsv) {
    set_led_color_rgb(idx, hsv_to_rgb(hsv));
}

#define set_array_color_hsv(x, y) _set_array_color_hsv(x, array_size(x), y)
void _set_array_color_hsv(const LED arr[], const size_t size, const HSV hsv) {
    for (size_t led_idx = 0; led_idx < size; ++led_idx) {
        set_led_color_hsv(arr[led_idx], hsv);
    }
}


static const uint16_t FADE_TIME = 25u;
static const uint16_t BLINK_TIME = 250u;


static bool blink_state = false;
void update_blink(void) {
    static uint16_t blink_timer = 0;
    if (timer_elapsed(blink_timer) > BLINK_TIME || !blink_timer) {
        blink_timer = timer_read();
        blink_state = !blink_state;
    }
}


// LED fade brightness control
static const uint8_t FADE_VS[] = {8,  27,  46,  65,  84, 103, 122, 141, 160, 179, 198, 217, 236, 255};
static const uint8_t FADE_V_MIN = 0;
static const uint8_t FADE_V_MAX = array_size(FADE_VS) - 1;
static const uint8_t FADE_V_DEF = MIN(3, FADE_V_MAX);
static       uint8_t fade_idx = FADE_V_DEF;
void update_fade(void) {
    static bool fade_up = true;
    static uint16_t fade_timer = 0;
    if (timer_elapsed(fade_timer) > FADE_TIME || !fade_timer) {
        fade_timer = timer_read();
        if (fade_idx == FADE_V_MIN) {
            fade_up = true;
        } else if (fade_idx == FADE_V_MAX) {
            fade_up = false;
        }
        if (fade_up) {
            ++fade_idx;
        } else {
            --fade_idx;
        }
    }
}



typedef enum STYLE {
    SOLID = 0,
    BLINK,
    FADE,
    NULL_STYLE
} STYLE;

typedef struct hsv_style_st {
    const STYLE style;
    const HSV*  hsv;
    const HSV*  alt;
} hsv_style_st;

static const HSV COLOR_OFF = COLOR(HSV_OFF);
// static const hsv_style_st HSV_STYLE_NULL = {NULL_STYLE, &COLOR_OFF, &COLOR_OFF};

#define hsv_style(style, hsv, alt) {style, hsv, alt}
// static inline hsv_style_st hsv_style(const STYLE style, const HSV* hsv, const HSV* alt) {
//     return (hsv_style_st){style, hsv, alt};
// }

#define hsv_style_solid(hsv) hsv_style(SOLID, hsv, NULL)
// static inline hsv_style_st hsv_style_solid(const HSV* hsv) {
//     return hsv_style(SOLID, hsv, &COLOR_OFF);
// }
#define hsv_style_blink(hsv, alt) hsv_style(BLINK, hsv, alt)
// static inline hsv_style_st hsv_style_blink(const HSV* hsv, const HSV* alt) {
//     return hsv_style(BLINK, hsv, alt);
// }
#define hsv_style_fade(hsv) hsv_style(FADE, hsv, NULL)
// static inline hsv_style_st hsv_style_fade(const HSV* hsv) {
//     return hsv_style(FADE, hsv, &COLOR_OFF);
// }


typedef struct led_style_st {
    const hsv_style_st* base;
    const hsv_style_st* fn;
    const hsv_style_st* fn_lock;
    const hsv_style_st* fm;
    const hsv_style_st* indicator;
} led_style_st;

// static const led_style_st LED_STYLE_NULL = {&HSV_STYLE_NULL, &HSV_STYLE_NULL, &HSV_STYLE_NULL, &HSV_STYLE_NULL, &HSV_STYLE_NULL};

#define led_style(base, fn, fn_lock, fm, indicator) (led_style_st){base, fn, fn_lock, fm, indicator}
// static inline led_style_st led_style(const hsv_style_st* base,    const hsv_style_st* fn,
//                               const hsv_style_st* fn_lock, const hsv_style_st* fm,
//                               const hsv_style_st* indicator) {
//     return (led_style_st){base, fn, fn_lock, fm, indicator};
// }


bool hsv_equal(const HSV* x, const HSV* y) {
    if (!x || !y) {
        return false;
    }
    return ((x->h == y->h)
         && (x->s == y->s)
         && (x->v == y->v));
}

HSV process_hsv_style(const hsv_style_st* hsv_style) {
    if (!hsv_style || v_idx == 0) {
        return COLOR_OFF;
    }
    HSV hsv = *(hsv_style->hsv);
    if (hsv_equal(&hsv, &COLOR_OFF)) {
        return COLOR_OFF;
    }

    switch (hsv_style->style) {
        case SOLID: {
            hsv.v = VS[v_idx];
            break;
        }
        case BLINK: {
            if (blink_state) {
                hsv = *(hsv_style->alt);
            }
            if (!hsv_equal(&hsv, &COLOR_OFF)) {
                hsv.v = VS[v_idx];
            }
            break;
        }
        case FADE: {
            // hsv.v = MIN(FADE_VS[fade_idx], VS[v_idx]);
            hsv.v = FADE_VS[fade_idx];
            break;
        }
        default:
            hsv = COLOR_OFF;
            break;
    }

    return hsv;
}

HSV get_led_style_hsv(const led_style_st* led_style) {
    HSV hsv = COLOR_OFF;
    if (!led_style) {
        return hsv;
    }

    if (led_style->fn && layer_state_is(_FN)) {
        hsv = process_hsv_style(led_style->fn);
    } else if (led_style->fn_lock && layer_state_is(_FN_LOCK)) {
        hsv = process_hsv_style(led_style->fn_lock);
    } else if (led_style->fm && layer_state_is(_FM)) {
        hsv = process_hsv_style(led_style->fm);
    } else if (led_style->base) {
        hsv = process_hsv_style(led_style->base);
    }

    return hsv;
}

HSV get_led_style_indicator_hsv(const led_style_st* led_style) {
    HSV hsv = COLOR_OFF;
    if (!led_style) {
        return hsv;
    }

    if (led_style->indicator) {
        hsv = process_hsv_style(led_style->indicator);
    } else if (led_style->base) {
        hsv = process_hsv_style(led_style->base);
    }

    return hsv;
}

HSV get_caps_lock_indicator_hsv(const hsv_style_st* hsv_style) {
    if (host_keyboard_led_state().caps_lock) {
        return process_hsv_style(hsv_style);
    } else {
        return COLOR_OFF;
    }
}


// LED -> HSV assignment
bool rgb_matrix_indicators_user(void) {
    static const HSV COLOR_OFF =         COLOR(HSV_OFF);
    static const HSV COLOR_FUCHSIA =     COLOR(HSV_FUCHSIA);
    static const HSV COLOR_VIOLET =      COLOR(HSV_VIOLET);
    static const HSV COLOR_BLUE_MED =    COLOR(HSV_BLUE_MED);
    static const HSV COLOR_LIME =        COLOR(HSV_LIME);
    static const HSV COLOR_BLUE =        COLOR(HSV_BLUE);
    static const HSV COLOR_RED =         COLOR(HSV_RED);
    static const HSV COLOR_GREEN =       COLOR(HSV_GREEN);
    static const HSV COLOR_AQUA =        COLOR(HSV_AQUA);
    static const HSV COLOR_WHITE =       COLOR(HSV_WHITE);
    static const HSV COLOR_CRIMSON =     COLOR(HSV_CRIMSON);
    static const HSV COLOR_ORANGE_DARK = COLOR(HSV_ORANGE_DARK);
    static const HSV COLOR_YELLOW = COLOR(HSV_YELLOW);
    static const HSV COLOR_TOMATO = COLOR(HSV_TOMATO);
    static const HSV COLOR_SLATEBLUE_MED = COLOR(HSV_SLATEBLUE_MED);
    // static const HSV COLOR_GRAY = COLOR(HSV_GRAY);


    static const hsv_style_st STYLE_OFF = hsv_style_solid(&COLOR_OFF);
    static const hsv_style_st STYLE_SOLID_FUCHSIA = hsv_style_solid(&COLOR_FUCHSIA);
    static const hsv_style_st STYLE_SOLID_BLUE_MED = hsv_style_solid(&COLOR_BLUE_MED);
    // static const hsv_style_st STYLE_SOLID_LIME = hsv_style_solid(&COLOR_LIME);
    static const hsv_style_st STYLE_SOLID_AQUA = hsv_style_solid(&COLOR_AQUA);
    static const hsv_style_st STYLE_SOLID_WHITE = hsv_style_solid(&COLOR_WHITE);
    static const hsv_style_st STYLE_SOLID_CRIMSON = hsv_style_solid(&COLOR_CRIMSON);
    static const hsv_style_st STYLE_SOLID_ORANGE_DARK = hsv_style_solid(&COLOR_ORANGE_DARK);
    static const hsv_style_st STYLE_SOLID_YELLOW = hsv_style_solid(&COLOR_YELLOW);
    static const hsv_style_st STYLE_SOLID_RED = hsv_style_solid(&COLOR_RED);
    static const hsv_style_st STYLE_SOLID_GREEN = hsv_style_solid(&COLOR_GREEN);
    static const hsv_style_st STYLE_SOLID_BLUE = hsv_style_solid(&COLOR_BLUE);
    static const hsv_style_st STYLE_SOLID_TOMATO = hsv_style_solid(&COLOR_TOMATO);
    static const hsv_style_st STYLE_SOLID_SLATEBLUE_MED = hsv_style_solid(&COLOR_SLATEBLUE_MED);


    static const hsv_style_st STYLE_BLINK_BLUE_RED = hsv_style_blink(&COLOR_BLUE, &COLOR_RED);
    static const hsv_style_st STYLE_BLINK_GREEN_OFF = hsv_style_blink(&COLOR_GREEN, &COLOR_OFF);


    static const hsv_style_st STYLE_FADE_VIOLET = hsv_style_fade(&COLOR_VIOLET);
    static const hsv_style_st STYLE_FADE_RED = hsv_style_fade(&COLOR_RED);
    static const hsv_style_st STYLE_FADE_GREEN = hsv_style_fade(&COLOR_GREEN);
    static const hsv_style_st STYLE_FADE_BLUE = hsv_style_fade(&COLOR_BLUE);
    static const hsv_style_st STYLE_FADE_BLUE_MED = hsv_style_fade(&COLOR_BLUE_MED);
    static const hsv_style_st STYLE_FADE_WHITE = hsv_style_fade(&COLOR_WHITE);
    static const hsv_style_st STYLE_FADE_LIME = hsv_style_fade(&COLOR_LIME);
    static const hsv_style_st STYLE_FADE_YELLOW = hsv_style_fade(&COLOR_YELLOW);
    static const hsv_style_st STYLE_FADE_TOMATO = hsv_style_fade(&COLOR_TOMATO);
    static const hsv_style_st STYLE_FADE_CRIMSON = hsv_style_fade(&COLOR_CRIMSON);
    static const hsv_style_st STYLE_FADE_AQUA = hsv_style_fade(&COLOR_AQUA);
    

    static const led_style_st LED_STYLE_ALPHABET = led_style(&STYLE_SOLID_FUCHSIA, NULL, &STYLE_FADE_VIOLET, NULL, NULL);
    static const led_style_st LED_STYLE_NUMBERS = led_style(&STYLE_SOLID_AQUA, NULL, NULL, NULL, NULL);
    static const led_style_st LED_STYLE_SYMBOLS = led_style(&STYLE_SOLID_ORANGE_DARK, NULL, NULL, NULL, NULL);

    static const led_style_st LED_STYLE_FKEYS =    led_style(&STYLE_SOLID_BLUE_MED, &STYLE_FADE_LIME, NULL, NULL, NULL);

    static const led_style_st LED_STYLE_ERASE = led_style(&STYLE_SOLID_CRIMSON, NULL, NULL, &STYLE_BLINK_BLUE_RED, NULL);

    static const led_style_st LED_STYLE_FN = led_style(&STYLE_OFF, &STYLE_FADE_VIOLET, &STYLE_OFF, &STYLE_BLINK_BLUE_RED, NULL);
    static const led_style_st LED_STYLE_TAB = led_style(&STYLE_SOLID_YELLOW, NULL, NULL, NULL, NULL);
    static const led_style_st LED_STYLE_CTRL = led_style(&STYLE_SOLID_RED, NULL, NULL, NULL, NULL);
    static const led_style_st LED_STYLE_ALT = led_style(&STYLE_SOLID_BLUE, NULL, NULL, NULL, NULL);
    static const led_style_st LED_STYLE_SHIFT = led_style(&STYLE_SOLID_GREEN, NULL, NULL, NULL, NULL);


    update_blink();
    update_fade();


    set_array_color_hsv(LED_ALPHABET, get_led_style_hsv(&LED_STYLE_ALPHABET));
    set_array_color_hsv(LED_NUMBERS,  get_led_style_hsv(&LED_STYLE_NUMBERS));
    set_array_color_hsv(LED_SYMBOLS,  get_led_style_hsv(&LED_STYLE_SYMBOLS));
    set_array_color_hsv(LED_FKEYS,    get_led_style_hsv(&LED_STYLE_FKEYS));

    set_array_color_hsv(LED_ERASE, get_led_style_hsv(&LED_STYLE_ERASE));
    set_array_color_hsv(LED_TAB,   get_led_style_hsv(&LED_STYLE_TAB));
    set_array_color_hsv(LED_ENTER, process_hsv_style(&STYLE_SOLID_BLUE_MED));
    set_array_color_hsv(LED_SPACE, process_hsv_style(&STYLE_SOLID_WHITE));

    set_array_color_hsv(LED_CTRL,      get_led_style_hsv(&LED_STYLE_CTRL));
    set_array_color_hsv(LED_ALT,       get_led_style_hsv(&LED_STYLE_ALT));
    set_array_color_hsv(LED_SHIFT,     get_led_style_hsv(&LED_STYLE_SHIFT));
    set_array_color_hsv(LED_CAPS_LOCK, get_caps_lock_indicator_hsv(&STYLE_BLINK_GREEN_OFF));

    set_array_color_hsv(LED_ARROWS, process_hsv_style(&STYLE_SOLID_SLATEBLUE_MED));
    set_array_color_hsv(LED_ESCAPE, process_hsv_style(&STYLE_SOLID_RED));

    if (layer_state_is(_SUPER)) {
        set_led_color_hsv(LED_SUPER, process_hsv_style(&STYLE_FADE_WHITE));

        set_led_color_hsv(LED_Q,     process_hsv_style(&STYLE_FADE_AQUA));
        set_led_color_hsv(LED_E,     process_hsv_style(&STYLE_FADE_AQUA));
        set_led_color_hsv(LED_F,     process_hsv_style(&STYLE_FADE_YELLOW));

        set_led_color_hsv(LED_K,     process_hsv_style(&STYLE_FADE_GREEN));
        set_led_color_hsv(LED_L,     process_hsv_style(&STYLE_FADE_RED));

        set_led_color_hsv(LED_PERIOD, process_hsv_style(&STYLE_FADE_WHITE));
        set_array_color_hsv(LED_DELETE, process_hsv_style(&STYLE_FADE_CRIMSON));

        set_array_color_hsv(LED_ESCAPE,  process_hsv_style(&STYLE_FADE_RED));
        set_array_color_hsv(LED_NUMBERS, process_hsv_style(&STYLE_FADE_BLUE));
        set_array_color_hsv(LED_SPACE,   process_hsv_style(&STYLE_FADE_BLUE_MED));
    } else {
        set_led_color_hsv(LED_SUPER, process_hsv_style(&STYLE_SOLID_TOMATO));
    }

    set_led_color_hsv(LED_FN, get_led_style_hsv(&LED_STYLE_FN));
    if (layer_state_is(_FN)) {
        set_led_color_hsv(LED_HYPHEN, process_hsv_style(&STYLE_FADE_TOMATO));
        set_led_color_hsv(LED_EQUAL, process_hsv_style(&STYLE_FADE_YELLOW));
        
        set_array_color_hsv(LED_SPACE,   process_hsv_style(&STYLE_SOLID_RED));
    }

    return true;
}


// keycode, macro, and layer handling
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Return code:
    //  - false: keypress is already handled (skip_handling)
    //  - true: process key normally        (!skip_handling)
    bool skip_handling = false;
    switch (keycode) {
        case FN_LOCK: {
            if (record->event.pressed) {
                if (layer_state_is(_FN)) {
                    layer_move(_FN_LOCK);
                }
                if (layer_state_is(_FM)) {
                    layer_move(_BASE);
                }
            }
            return skip_handling;
        }
        case KC_LGUI: {
            if (record->event.pressed) {
                layer_on(_SUPER);
            } else {
                layer_off(_SUPER);
            }
            return !skip_handling;
        }
        case LED_TOG: {
            if (record->event.pressed) {
                toggle_led();
            }
            return skip_handling;
        }
        case LED_DEC: {
            if (record->event.pressed) {
                decrease_led_brightness();
            }
            return skip_handling;
        }
        case LED_INC: {
            if (record->event.pressed) {
                increase_led_brightness();
            }
            return skip_handling;
        }
        default: {
            break;
        }
    }
    return !skip_handling;
}
