#include "keycodes.h"
#include QMK_KEYBOARD_H
#include "quantum.h"

enum custom_keycodes {
  MY_KC_CLICK_LEFT = QK_USER,
  MY_KC_CLICK_RIGHT,
  MY_KC_CLICK_MIDDLE,
  MY_KC_SCROLL,
};

enum click_state {
  NONE = 0,
  // マウスレイヤーが有効になるのを待つ。
  WAITING,
  // マウスレイヤー有効になりクリック入力が取れる。
  CLICKABLE,
  // クリック中
  CLICKING,
  // スクロール中
  SCROLLING
};

// ======================================
// =              Configs               =
// ======================================

// この閾値を超える度に垂直スクロールが実行される。
const int16_t SCROLL_THRESHOLD_V = 50;

// この閾値を超える度に水平スクロールが実行される。
const int16_t SCROLL_THRESHOLD_H = 50;

// スクロールの値に掛けられる値(垂直方向)。
// この値がマイナスだとWindowsのスクロール方向になる。
const int16_t SCROLL_MULTIPLIER_V = -1;

// スクロールの値に掛けられる値(水平方向)。
// この値がマイナスだとWindowsのスクロール方向になる。
const int16_t SCROLL_MULTIPLIER_H = -1;

// 自動マウス用レイヤ。
const uint16_t AUTO_MOUSE_LAYER = 4;

// 自動マウスレイヤが有効になるカーソルの移動距離
const int16_t AUTO_MOUSE_MOVEMENT_THRESHOLD = 20;

// 自動マウスレイヤが無効になるまでの時間(ms)
const int16_t AUTO_MOUSE_RESET_TIME = 600;

// この配列で指定されたキーはマウスレイヤー中に押下してもマウスレイヤーを解除しない
const uint16_t AUTO_MOUSE_CONTINUE_KEYS[] = {
    KC_LGUI, KC_LCTL, KC_LALT, KC_LSFT, KC_RGUI, KC_RCTL, KC_RALT, KC_RSFT};

// ======================================
// =              State                 =
// ======================================

// 現在のクリック入力受付の状態
enum click_state state;

// タイマー。状態に応じて時間で判定する。
uint16_t click_timer;

// 垂直スクロールの入力をカウントする。
int16_t scroll_v_mouse_interval_counter;

// 水平スクロールの入力をカウントする。
int16_t scroll_h_mouse_interval_counter;

// クリック入力後の移動量を測定する変数。
int16_t after_click_lock_movement = 0;

int16_t auto_mouse_movement = 0;

// ======================================
// =              Functions             =
// ======================================

// クリック用のレイヤーを有効にする。
void enable_click_layer(void) {
  layer_on(AUTO_MOUSE_LAYER);
  click_timer = timer_read();
  state = CLICKABLE;
}

// クリック用のレイヤーを無効にする。
void disable_click_layer(void) {
  state = NONE;
  layer_off(AUTO_MOUSE_LAYER);
  scroll_v_mouse_interval_counter = 0;
  scroll_h_mouse_interval_counter = 0;
}

int16_t my_abs(int16_t num) {
  if (num < 0) {
    num = -num;
  }

  return num;
}

// ======================================
// =              Handlers              =
// ======================================

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case MY_KC_CLICK_LEFT:
  case MY_KC_CLICK_RIGHT:
  case MY_KC_CLICK_MIDDLE: {
    report_mouse_t currentReport = pointing_device_get_report();

    // どこのビットを対象にするか。 Which bits are to be targeted?
    uint8_t btn = 1 << (keycode - MY_KC_CLICK_LEFT);

    if (record->event.pressed) {
      // ビットORは演算子の左辺と右辺の同じ位置にあるビットを比較して、両方のビットのどちらかが「1」の場合に「1」にします。
      // Bit OR compares bits in the same position on the left and right sides
      // of the operator and sets them to "1" if either of both bits is "1".
      currentReport.buttons |= btn;
      state = CLICKING;
      after_click_lock_movement = 30;
    } else {
      // ビットANDは演算子の左辺と右辺の同じ位置にあるビットを比較して、両方のビットが共に「1」の場合だけ「1」にします。
      // Bit AND compares the bits in the same position on the left and right
      // sides of the operator and sets them to "1" only if both bits are "1"
      // together.
      currentReport.buttons &= ~btn;
    }

    enable_click_layer();

    pointing_device_set_report(currentReport);
    pointing_device_send();
    return false;
  }

  case MY_KC_SCROLL:
    if (record->event.pressed) {
      state = SCROLLING;
    } else {
      // スクロールキーを離した時に再度クリックレイヤーを有効にする。
      enable_click_layer();
    }
    return false;

  default:
    if (record->event.pressed) {

      if (state == CLICKING || state == SCROLLING) {
        enable_click_layer();
        return false;
      }

      for (int i = 0; i < sizeof(AUTO_MOUSE_CONTINUE_KEYS) /
                              sizeof(AUTO_MOUSE_CONTINUE_KEYS[0]);
           i++) {
        if (keycode == AUTO_MOUSE_CONTINUE_KEYS[i]) {
          return true;
        }
      }

      disable_click_layer();
    }
  }

  return true;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
  int16_t current_x = mouse_report.x;
  int16_t current_y = mouse_report.y;
  int16_t current_h = 0;
  int16_t current_v = 0;

  if (current_x != 0 || current_y != 0) {
    switch (state) {
    case CLICKABLE:
      click_timer = timer_read();
      break;

    case CLICKING:
      after_click_lock_movement -= my_abs(current_x) + my_abs(current_y);

      if (after_click_lock_movement > 0) {
        current_x = 0;
        current_y = 0;
      }

      break;

    case SCROLLING: {
      int8_t rep_v = 0;
      int8_t rep_h = 0;

      // 垂直スクロールの方の感度を高める。
      if (my_abs(current_y) * 2 > my_abs(current_x)) {
        scroll_v_mouse_interval_counter += current_y;
        while (my_abs(scroll_v_mouse_interval_counter) > SCROLL_THRESHOLD_V) {
          if (scroll_v_mouse_interval_counter < 0) {
            scroll_v_mouse_interval_counter += SCROLL_THRESHOLD_V;
            rep_v += SCROLL_THRESHOLD_V;
          } else {
            scroll_v_mouse_interval_counter -= SCROLL_THRESHOLD_V;
            rep_v -= SCROLL_THRESHOLD_V;
          }
        }
      } else {
        scroll_h_mouse_interval_counter += current_x;

        while (my_abs(scroll_h_mouse_interval_counter) > SCROLL_THRESHOLD_H) {
          if (scroll_h_mouse_interval_counter < 0) {
            scroll_h_mouse_interval_counter += SCROLL_THRESHOLD_H;
            rep_h += SCROLL_THRESHOLD_H;
          } else {
            scroll_h_mouse_interval_counter -= SCROLL_THRESHOLD_H;
            rep_h -= SCROLL_THRESHOLD_H;
          }
        }
      }

      current_h = rep_h / SCROLL_THRESHOLD_H * SCROLL_MULTIPLIER_H;
      current_v = -rep_v / SCROLL_THRESHOLD_V * SCROLL_MULTIPLIER_V;
      current_x = 0;
      current_y = 0;
    } break;

    case WAITING:
      /*
      if (timer_elapsed(click_timer) > user_config.to_clickable_time) {
          enable_click_layer();
      }
      */

      auto_mouse_movement += my_abs(current_x) + my_abs(current_y);

      if (auto_mouse_movement >= AUTO_MOUSE_MOVEMENT_THRESHOLD) {
        auto_mouse_movement = 0;
        enable_click_layer();
      }
      break;

    default:
      click_timer = timer_read();
      state = WAITING;
      auto_mouse_movement = 0;
    }
  } else {
    switch (state) {
    case CLICKING:
    case SCROLLING:
      break;

    case CLICKABLE:
      if (timer_elapsed(click_timer) > AUTO_MOUSE_RESET_TIME) {
        disable_click_layer();
      }
      break;

    case WAITING:
      if (timer_elapsed(click_timer) > 50) {
        auto_mouse_movement = 0;
        state = NONE;
      }
      break;

    default:
      auto_mouse_movement = 0;
      state = NONE;
    }
  }

  mouse_report.x = current_x;
  mouse_report.y = current_y;
  mouse_report.h = current_h;
  mouse_report.v = current_v;

  return mouse_report;
}

#ifdef OLED_ENABLE

#include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
  keyball_oled_render_keyinfo();
  keyball_oled_render_ballinfo();

  oled_write_P(PSTR("Layer:"), false);
  oled_write(get_u8_str(get_highest_layer(layer_state), ' '), false);
}

#endif

// ======================================
// =           Default keymaps          =
// ======================================

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    LAYOUT_universal(
        KC_H   , KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,                                  KC_6     , KC_7     , KC_8     , KC_9     , KC_0     , KC_MINS  ,
        KC_DEL   , KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                                  KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     , KC_INT3  ,
        KC_TAB   , KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                                  KC_H     , KC_J     , KC_K     , KC_L     , KC_SCLN  , S(KC_7)  ,
        MO(1)    , KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     , KC_RBRC  ,              KC_NUHS, KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  , KC_RSFT  ,
        _______  , KC_LCTL  , KC_LALT  , KC_LGUI,LT(1,KC_LNG2),LT(2,KC_SPC),LT(3,KC_LNG1),    KC_BSPC,LT(2,KC_ENT),LT(1,KC_LNG2),KC_RGUI, _______ , KC_RALT  , KC_PSCR
    ),

    LAYOUT_universal(
        S(KC_ESC), S(KC_1)  , KC_LBRC  , S(KC_3)  , S(KC_4)  , S(KC_5)  ,                                  KC_EQL   , S(KC_6)  ,S(KC_QUOT), S(KC_8)  , S(KC_9)  ,S(KC_INT1),
        S(KC_DEL), S(KC_Q)  , S(KC_W)  , S(KC_E)  , S(KC_R)  , S(KC_T)  ,                                  S(KC_Y)  , S(KC_U)  , S(KC_I)  , S(KC_O)  , S(KC_P)  ,S(KC_INT3),
        S(KC_TAB), S(KC_A)  , S(KC_S)  , S(KC_D)  , S(KC_F)  , S(KC_G)  ,                                  S(KC_H)  , S(KC_J)  , S(KC_K)  , S(KC_L)  , KC_QUOT  , S(KC_2)  ,
        _______  , S(KC_Z)  , S(KC_X)  , S(KC_C)  , S(KC_V)  , S(KC_B)  ,S(KC_RBRC),           S(KC_NUHS), S(KC_N)  , S(KC_M)  ,S(KC_COMM), S(KC_DOT),S(KC_SLSH),S(KC_RSFT),
        _______  ,S(KC_LCTL),S(KC_LALT),S(KC_LGUI), _______  , _______  , _______  ,            _______  , _______  , _______  ,S(KC_RGUI), _______  , S(KC_RALT), _______
    ),

    LAYOUT_universal(
        _______  , KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_F5    ,                                  KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   , KC_F11   ,
        _______  , _______  , KC_7     , KC_8     , KC_9     , _______  ,                                  _______  , KC_LEFT  , KC_UP    , KC_RGHT  , _______  , KC_F12   ,
        _______  , _______  , KC_4     , KC_5     , KC_6     ,S(KC_SCLN),                                  KC_PGUP  , KC_BTN1  , KC_DOWN  , KC_BTN2  , KC_BTN3  , _______  ,
        _______  , _______  , KC_1     , KC_2     , KC_3     ,S(KC_MINS), S(KC_8)  ,            S(KC_9)  , KC_PGDN  , _______  , _______  , _______  , _______  , _______  ,
        _______  , _______  , KC_0     , KC_DOT   , _______  , _______  , _______  ,             KC_DEL  , _______  , _______  , _______  , _______  , _______  , _______
    ),

    LAYOUT_universal(
        RGB_TOG  , _______  , _______  , _______  , _______  , _______  ,                                  RGB_M_P  , RGB_M_B  , RGB_M_R  , RGB_M_SW , RGB_M_SN , RGB_M_K  ,
        RGB_MOD  , RGB_HUI  , RGB_SAI  , RGB_VAI  , _______  , _______  ,                                  RGB_M_X  , RGB_M_G  , RGB_M_T  , RGB_M_TW , _______  , _______  ,
        RGB_RMOD , RGB_HUD  , RGB_SAD  , RGB_VAD  , _______  , _______  ,                                  CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , KBC_SAVE , KBC_RST  ,
        _______  , _______  , SCRL_DVD , SCRL_DVI , SCRL_MO  , SCRL_TO  , EE_CLR  ,              EE_CLR  , KC_HOME  , KC_PGDN  , KC_PGUP  , KC_END   , _______  , _______  ,
        QK_BOOT  , _______  , KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , _______ ,             _______  , KC_BSPC  , _______  , _______  , _______  , _______  , QK_BOOT
    ),

    LAYOUT_universal(
        _______  , _______  , _______  , _______  , _______  , _______  ,                                  _______  , _______  , _______  , _______  , _______  , _______  ,
        _______  , _______  , _______  , _______  , _______  , _______  ,                                  _______  , _______  , _______  , _______  , _______  , _______  ,
        _______  , _______  ,MY_KC_CLICK_RIGHT, MY_KC_SCROLL,MY_KC_CLICK_LEFT, _______  ,                                  _______  ,MY_KC_CLICK_LEFT, MY_KC_SCROLL,MY_KC_CLICK_RIGHT, _______  , _______  ,
        _______  , _______  , _______  , _______  , _______  , _______  , _______  ,            _______  , _______  , _______  , _______  , _______  , _______  , _______  ,
        _______  , _______  , _______  , _______  , _______  , _______  , _______  ,            _______  , _______  , _______  , _______  , _______  , _______  , _______
    ),
};
