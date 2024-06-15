/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)
Copyright 2024 nazo6

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "keycodes.h"
#include "rgblight.h"
#include QMK_KEYBOARD_H

#include "quantum.h"

enum nazo6_kc {
  K_MHNKN = 0x428B,
  K_HNKN = 0x418A,
  JP_COLN = KC_QUOTE,
  // バックスラッシュ(Shiftで_)
  JP_BSLSH = KC_INT1,
  // バックスラッシュ(Shiftで|)
  JP_BSLSH_2 = KC_INT3,
  JP_AT = KC_LEFT_BRACKET,
  JP_CARET = KC_EQUAL,
  JP_ZNHN = KC_GRAVE,

  MIDDLE_BTN = KEYBALL_SAFE_RANGE
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // ベースレイヤ
  [0] = LAYOUT_universal(
    LT(4,JP_ZNHN), KC_1   , KC_2    , KC_3   , KC_4         , KC_5        ,          /**/          KC_6        , KC_7   , KC_8   , KC_9   , KC_0    , KC_EQUAL,
    KC_TAB       , KC_Q   , KC_W    , KC_E   , KC_R         , KC_T        ,          /**/          KC_Y        , KC_U   , KC_I   , KC_O   , KC_P    , KC_MINS,
    KC_ESC       , KC_A   , KC_S    , KC_D   , KC_F         , KC_G        ,          /**/          KC_H        , KC_J   , KC_K   , KC_L   , KC_SCLN , KC_QUOT,
    KC_LSFT      , KC_Z   , KC_X    , KC_C   , KC_V         , KC_B        , KC_LBRC, /**/ KC_RBRC, KC_N        , KC_M   , KC_COMM, KC_DOT , KC_SLSH , KC_BSLS,
    KC_LCTL      , KC_LWIN, TG(2)   , KC_LALT, LT(3,KC_SPC) , LT(2,KC_SPC), KC_SPC , /**/ KC_BSPC, LT(2,KC_ENT), _______, _______, _______, KC_RSFT , KC_RCTL
  ),


  // マウスレイヤ(自動)
  [1] = LAYOUT_universal(
    _______, _______, _______, _______, _______, _______,          /**/          _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,          /**/          _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,          /**/          _______, KC_BTN1, SCRL_MO, KC_BTN2, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, /**/ _______, _______, KC_BTN4, KC_BTN3, KC_BTN5, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, /**/ _______, _______, _______, _______, _______, _______, _______
  ),

  // マウス+便利キー
  [2] = LAYOUT_universal(
    _______, KC_F1    , KC_F2   , KC_F3   , KC_F4   , KC_F5   ,          /**/           KC_F6   , KC_F7    , KC_F8    , KC_F9    , KC_F10   , KC_F11,
    _______, _______  , KC_INS  , KC_HOME , KC_PGUP , _______ ,          /**/          KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , _______  , KC_F12,
    _______, _______  , KC_DEL  , KC_END  , KC_PGDN , _______ ,          /**/          KC_PGUP  , KC_BTN1  , SCRL_MO  , KC_BTN2  , _______  , KC_AUDIO_VOL_UP,
    _______, _______  , _______ , _______ , _______ , _______ , _______, /**/ _______, KC_PGDN  , KC_BTN4  , KC_BTN3  , KC_BTN5  , _______  , KC_AUDIO_VOL_DOWN,
    _______, _______  , _______ , _______ , _______ , _______ , _______, /**/ KC_DEL , _______  , _______  , _______  , _______  , _______  , KC_PRINT_SCREEN
  ),

  // 記号
  [3] = LAYOUT_universal(
    _______, _______, _______, _______, _______, _______,          /**/           _______   , _______   , _______   , _______    , _______, _______,
    _______, _______, KC_KP_7, KC_KP_8, KC_KP_9, _______,          /**/           S(KC_1)   , S(KC_2)   , S(KC_3)   , S(KC_4)    , S(KC_5), _______,
    _______, _______, KC_KP_4, KC_KP_5, KC_KP_6, _______,          /**/           S(KC_6)   , S(KC_7)   , S(KC_8)   , S(KC_9)    , S(KC_0), _______,
    _______, _______, KC_KP_1, KC_KP_2, KC_KP_3, _______, _______, /**/ _______ , KC_QUOT   , S(KC_QUOT), KC_EQUAL  , S(KC_EQUAL), _______, _______,
    _______, _______, KC_KP_0, _______, _______, _______, _______, /**/ _______ , _______   , _______   , _______   , _______    , _______, _______
  ),

  // メタレイヤ(keyball自体の設定とか)
  [4] = LAYOUT_universal(
    _______, _______, _______, _______, _______, _______,          /**/          _______ , _______, CPI_I1K , _______ , _______, _______,
    _______, _______, _______, _______, _______, _______,          /**/          _______ , AML_I50, CPI_I100, SCRL_DVI, _______, _______,
    _______, _______, _______, _______, _______, _______,          /**/          _______ , AML_D50, CPI_D100, SCRL_DVD, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, /**/ _______, _______ , _______, CPI_D1K , _______ , _______, _______,
    _______, _______, _______, _______, _______, _______, RGB_TOG, /**/ KBC_RST, KBC_SAVE, _______, _______ , _______ , _______, _______
  ),
};

// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
  switch (get_highest_layer(state)) {
  case 1:
    rgblight_sethsv(210, 200, 5);
    break;
  case 2:
    rgblight_sethsv(250, 200, 10);
    break;
  case 3:
    rgblight_sethsv(30, 200, 10);
    break;
  case 4:
    rgblight_sethsv(85, 200, 10);
    break;

  default:
    rgblight_sethsv(HSV_OFF);
  }

  return state;
}

#ifdef OLED_ENABLE

#include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
  keyball_oled_render_keyinfo();
  keyball_oled_render_ballinfo();
  keyball_oled_render_layerinfo();
}

#endif

void keyboard_post_init_user(void) { set_auto_mouse_enable(true); }
