#include QMK_KEYBOARD_H

/* (h, s, v) */
#define CLR_QWERTY  15,  255, 255
#define CLR_COLEMAK 5,   255, 255
#define CLR_SYM     215, 255, 255
#define CLR_NUM     205, 255, 255
#define CLR_NAV     128, 255, 255
#define CLR_CAPS    0,   0,   255
#define CLR_STL     0,   255, 1

enum layers
{
  L_QWERTY,
  L_COLEMAK,
  L_NUM,
  L_SYM,
  L_NAV
};

enum custom_keycodes
{
  MACRO_PARENS = SAFE_RANGE,
  MACRO_BRACES,
  MACRO_BRACKETS,
  MACRO_QUOTES,
  MACRO_ARROW,
  MACRO_ANGLES,
  TG_BASE, /* toggle base layer between qwerty and colemak */
  TG_STL,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS]
    = { [L_QWERTY] = LAYOUT_split_3x5_3 (
            KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P,
            KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN,
            LSFT_T (KC_Z), LCTL_T (KC_X), KC_C, KC_V, KC_B, KC_N, KC_M,
            RGUI_T (KC_COMM), RALT_T (KC_DOT), RSFT_T (KC_SLSH), KC_SPC,
            MO (L_SYM), KC_ENT, KC_ESC, MO (L_NUM), KC_BSPC),

        [L_COLEMAK] = LAYOUT_split_3x5_3 (
            KC_Q, KC_W, KC_F, KC_P, KC_G, KC_J, KC_L, KC_U, KC_Y, KC_SCLN,
            KC_A, KC_R, KC_S, KC_T, KC_D, KC_H, KC_N, KC_E, KC_I, KC_O,
            LSFT_T (KC_Z), LCTL_T (KC_X), KC_C, KC_V, KC_B, KC_K, KC_M,
            RGUI_T (KC_COMM), RALT_T (KC_DOT), RSFT_T (KC_SLSH), KC_SPC,
            MO (L_SYM), KC_ENT, KC_ESC, MO (L_NUM), KC_BSPC),

        [L_NUM] = LAYOUT_split_3x5_3 (
            KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0,
            KC_LSFT, KC_NO, KC_NO, KC_NO, KC_DOT, KC_TAB, KC_LEFT, KC_DOWN,
            KC_UP, KC_RGHT, KC_LCTL, KC_LALT, KC_LGUI, KC_NO, KC_COMM, KC_GRV,
            KC_HOME, KC_PGDN, KC_PGUP, KC_END, KC_NO, MO (L_NAV), KC_NO, KC_NO,
            KC_TRNS, KC_NO),

        [L_SYM] = LAYOUT_split_3x5_3 (
            KC_NO, KC_NO, KC_NO, MACRO_BRACKETS, MACRO_ANGLES, LSFT (KC_BSLS),
            LSFT (KC_9), LSFT (KC_0), KC_LBRC, KC_RBRC, KC_LSFT, KC_NO,
            MACRO_QUOTES, MACRO_PARENS, MACRO_ARROW, LSFT (KC_7), LSFT (KC_1),
            KC_MINS, LSFT (KC_QUOT), KC_QUOT, KC_LCTL, KC_LALT, KC_LGUI,
            MACRO_BRACES, KC_NO, KC_BSLS, LSFT (KC_8), LSFT (KC_MINS), KC_EQL,
            KC_GRV, KC_NO, KC_TRNS, KC_NO, KC_CAPS, MO (L_NAV), KC_TAB),

        [L_NAV] = LAYOUT_split_3x5_3 (
            KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9,
            KC_F10, KC_BRID, KC_BRIU, KC_CALC, KC_WSCH, KC_WBAK, KC_F13,
            KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_UNDO, KC_CUT, KC_COPY,
            KC_PSTE, KC_F11, KC_F12, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
            KC_TRNS, KC_NO, TG_BASE, KC_TRNS, TG_STL) };

static unsigned char stl = 0;
static void
set_led_color (layer_state_t current_state, layer_state_t base_state);

bool
process_record_user (uint16_t keycode, keyrecord_t *record)
{
  if (record->event.pressed)
    {
      switch (keycode)
        {
        case MACRO_PARENS:
          tap_code16 (KC_LPRN);
          tap_code16 (KC_RPRN);
          tap_code (KC_LEFT);
          return false;

        case MACRO_BRACES:
          SEND_STRING ("{\n\n}" SS_TAP (X_UP));
          return false;

        case MACRO_BRACKETS:
          tap_code16 (KC_LBRC);
          tap_code16 (KC_RBRC);
          tap_code (KC_LEFT);
          return false;

        case MACRO_QUOTES:
          tap_code16 (KC_DQUO);
          tap_code16 (KC_DQUO);
          tap_code (KC_LEFT);
          return false;

        case MACRO_ARROW:
          tap_code (KC_MINS);
          tap_code16 (KC_GT);
          return false;

        case MACRO_ANGLES:
          tap_code16 (KC_LT);
          tap_code16 (KC_GT);
          tap_code (KC_LEFT);
          return false;

        case TG_BASE:
          if (default_layer_state & (1UL << L_COLEMAK))
            set_single_persistent_default_layer (L_QWERTY);
          else
            set_single_persistent_default_layer (L_COLEMAK);
          return false;

        case TG_STL:
          stl ^= 1;
          set_led_color (layer_state, default_layer_state);
          return false;
        }
    }
  return true;
}

static void
set_led_color (layer_state_t current_state, layer_state_t base_state)
{
  uint8_t current_layer = get_highest_layer (current_state);
  uint8_t base_layer = get_highest_layer (base_state);

  if (stl)
    {
      rgblight_sethsv_noeeprom (CLR_STL);
      return;
    }

  switch (current_layer)
    {
    case L_NAV:
      rgblight_sethsv_noeeprom (CLR_NAV);
      break;
    case L_SYM:
      rgblight_sethsv_noeeprom (CLR_SYM);
      break;
    case L_NUM:
      rgblight_sethsv_noeeprom (CLR_NUM);
      break;
    default:
      if (host_keyboard_led_state ().caps_lock)
        rgblight_sethsv_noeeprom (CLR_CAPS);
      else if (base_layer == L_COLEMAK)
        rgblight_sethsv_noeeprom (CLR_COLEMAK);
      else
        rgblight_sethsv_noeeprom (CLR_QWERTY);
      break;
    }
}

void
keyboard_post_init_user (void)
{
  rgblight_enable_noeeprom ();
  rgblight_mode_noeeprom (RGBLIGHT_MODE_STATIC_LIGHT);
  set_led_color (layer_state, default_layer_state);
}

layer_state_t
default_layer_state_set_user (layer_state_t state)
{
  set_led_color (layer_state, state);
  return state;
}

layer_state_t
layer_state_set_user (layer_state_t state)
{
  set_led_color (state, default_layer_state);
  return state;
}

bool
led_update_user (led_t led_state)
{
  set_led_color (layer_state, default_layer_state);
  return true;
}
