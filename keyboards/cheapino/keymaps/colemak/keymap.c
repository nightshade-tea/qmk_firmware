#include QMK_KEYBOARD_H

enum layers
{
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
  MACRO_ANGLES
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS]
    = { [L_COLEMAK] = LAYOUT_split_3x5_3 (
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
            KC_TRNS, QK_BOOT, KC_NO, KC_TRNS, KC_NO) };

bool
process_record_user (uint16_t keycode, keyrecord_t *record)
{
  if (record->event.pressed)
    {
      switch (keycode)
        {
        case MACRO_PARENS:
          SEND_STRING ("()" SS_TAP (X_LEFT));
          return false;
        case MACRO_BRACES:
          SEND_STRING ("{}" SS_TAP (X_LEFT) SS_TAP (X_ENTER) SS_TAP (X_UP)
                           SS_TAP (X_END) SS_TAP (X_ENTER));
          return false;
        case MACRO_BRACKETS:
          SEND_STRING ("[]" SS_TAP (X_LEFT));
          return false;
        case MACRO_QUOTES:
          SEND_STRING ("\"\"" SS_TAP (X_LEFT));
          return false;
        case MACRO_ARROW:
          SEND_STRING ("->");
          return false;
        case MACRO_ANGLES:
          SEND_STRING ("<>" SS_TAP (X_LEFT));
          return false;
        }
    }
  return true;
}
