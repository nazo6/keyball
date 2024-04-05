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

};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // ベースレイヤ
  [0] = LAYOUT_universal(
    LT(3,JP_ZNHN), KC_1   , KC_2         , KC_3       , KC_4   , KC_5        ,          /**/          KC_6        , KC_7   , KC_8   , KC_9   , KC_0    , JP_BSLSH_2,
    KC_TAB       , KC_Q   , KC_W         , KC_E       , KC_R   , KC_T        ,          /**/          KC_Y        , KC_U   , KC_I   , KC_O   , KC_P    , KC_MINS,
    KC_ESC       , KC_A   , KC_S         , KC_D       , KC_F   , KC_G        ,          /**/          KC_H        , KC_J   , KC_K   , KC_L   , KC_SCLN , JP_COLN,
    KC_LSFT      , KC_Z   , KC_X         , KC_C       , KC_V   , KC_B        , KC_RBRC, /**/ KC_NUHS, KC_N        , KC_M   , KC_COMM, KC_DOT , KC_SLSH , JP_BSLSH,
    KC_LCTL      , KC_LWIN, LT(1,K_MHNKN), TG(2)      , KC_LALT, LT(2,KC_SPC), KC_SPC , /**/ KC_BSPC, LT(2,KC_ENT), _______, _______, _______, JP_CARET, JP_AT
  ),

  // 記号とか？
  [1] = LAYOUT_universal(
    S(KC_ESC), S(KC_1)   , KC_LBRC   , S(KC_3)   , S(KC_4), S(KC_5),             /**/             KC_EQL , S(KC_6), S(KC_QUOT), S(KC_8)  , S(KC_9)   , S(KC_INT1),
    S(KC_DEL), S(KC_Q)   , S(KC_W)   , S(KC_E)   , S(KC_R), S(KC_T),             /**/             S(KC_Y), S(KC_U), S(KC_I)   , S(KC_O)  , S(KC_P)   , S(KC_INT3),
    S(KC_TAB), S(KC_A)   , S(KC_S)   , S(KC_D)   , S(KC_F), S(KC_G),             /**/             S(KC_H), S(KC_J), S(KC_K)   , S(KC_L)  , KC_QUOT   , S(KC_2)   ,
    _______  , S(KC_Z)   , S(KC_X)   , S(KC_C)   , S(KC_V), S(KC_B), S(KC_RBRC), /**/ S(KC_NUHS), S(KC_N), S(KC_M), S(KC_COMM), S(KC_DOT), S(KC_SLSH), S(KC_RSFT),
    _______  , S(KC_LCTL), S(KC_LALT), S(KC_LGUI), _______, _______, _______   , /**/ _______   , _______, _______, S(KC_RGUI), _______  , S(KC_RALT), _______
  ),

  // マウスレイヤ(自動も)+便利キー
  [2] = LAYOUT_universal(
    _______, KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_F5     ,          /**/           KC_F6   , KC_F7    , KC_F8    , KC_F9    , KC_F10   , KC_F11   ,
    _______, _______  , KC_7     , KC_8     , KC_9     , _______   ,          /**/          KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , _______  , KC_F12   ,
    _______, _______  , KC_4     , KC_5     , KC_6     , S(KC_SCLN),          /**/          KC_PGUP  , KC_BTN1  , SCRL_MO  , KC_BTN2  , _______  , KC_AUDIO_VOL_UP,
    _______, _______  , KC_1     , KC_2     , KC_3     , S(KC_MINS), S(KC_8), /**/ S(KC_9), KC_PGDN  , KC_BTN4  , KC_BTN3  , KC_BTN5  , _______  , KC_AUDIO_VOL_DOWN,
    _______, _______  , KC_0     , TG(2)    , _______  , _______   , _______, /**/ _______, _______  , _______  , _______  , _______  , _______  , KC_PRINT_SCREEN
  ),

  // メタレイヤ(keyball自体の設定とか)
  [3] = LAYOUT_universal(
    _______  , AML_TO   , AML_I50  , AML_D50 , _______  , _______  ,            /**/          RGB_M_P  , RGB_M_B  , RGB_M_R  , RGB_M_SW , RGB_M_SN , RGB_M_K  ,
    RGB_MOD  , RGB_HUI  , RGB_SAI  , RGB_VAI , _______  , _______  ,            /**/          RGB_M_X  , RGB_M_G  , RGB_M_T  , RGB_M_TW , _______  , _______  ,
    RGB_RMOD , RGB_HUD  , RGB_SAD  , RGB_VAD , _______  , _______  ,            /**/          CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , KBC_SAVE , KBC_RST  ,
    RGB_TOG  , _______  , SCRL_DVD , SCRL_DVI, SCRL_MO  , SCRL_TO  , EE_CLR   , /**/ EE_CLR , KC_HOME  , KC_PGDN  , KC_PGUP  , KC_END   , _______  , _______  ,
    QK_BOOT  , _______  , KC_LEFT  , TO(0)   , KC_UP    , KC_RGHT  , _______  , /**/ _______, KC_BSPC  , AML_TO   , _______  , _______  , AML_TO  , QK_BOOT
  ),
  };
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
  switch (get_highest_layer(state)) {
  case 1:
    rgblight_sethsv(50, 255, 10);
    break;
  case 2:
    rgblight_sethsv(0, 2, 5);
    break;
  case 3:
    rgblight_sethsv(130, 255, 10);
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

void keyboard_post_init_user(void) {
  set_auto_mouse_enable(true);
  keyball_set_scroll_div(6);
}

// 容量削減
uint16_t keycode_config(uint16_t keycode) { return keycode; }
uint8_t mod_config(uint8_t mod) { return mod; }