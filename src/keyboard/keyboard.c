#include "keyboard.h"
#include "../io.h"
#include <stdbool.h>

#define IFV if (false)
#define IFVV if (false)

#define KBC_STATUS 0x64
#define KBC_EA 0x60

static void send_command(uint8_t command) {
  while ((inportb(KBC_STATUS) & 2)) {}
  outportb(KBC_EA, command);
}

Scancode keyboard_get_scancode() {
  static unsigned e0_code = 0;
  static unsigned e1_code = 0;
  static uint16_t e1_prev = 0;

  uint8_t scancode = 0;

  if (inportb(KBC_STATUS) & 1) {
    scancode = inportb(KBC_EA);
    if (e0_code == 1) {
      e0_code = 0;
      return (0xe000 | scancode);
    } else if (e1_code == 1) {
      e1_prev = scancode;
      e1_code = 2;
    } else if (e1_code == 2) {
      e1_code = 0;
      return (0xe10000 | e1_prev << 8 | scancode);
    } else if (scancode == 0xe0) {
      e0_code = 1;
      scancode = 0;
    } else if (scancode == 0xe1) {
      e1_code = 1;
      scancode = 0;
    }
  }

  return scancode;
}

bool key_modifier_shift = false;

uint8_t scancode_to_keycode(Scancode sc) {
  switch (sc) {
  case KEYCODE_LSHIFT:
  case KEYCODE_RSHIFT:
    key_modifier_shift = true;
    return 0;
  case KEYCODE_LSHIFT_RELEASE:
  case KEYCODE_RSHIFT_RELEASE:
    key_modifier_shift = false;
    return 0;
  default:
    break;
  }
  if (key_modifier_shift) return keymap_shift[sc];
  return keymap[sc];
}

uint8_t wait_for_key() {
  uint32_t sc;
  while ((sc = keyboard_get_scancode()) == 0) {}
  return scancode_to_keycode(sc);
}

Scancode wait_for_scancode() {
  uint32_t sc;
  while ((sc = keyboard_get_scancode()) == 0) {}
  return sc;
}

int keyboard_mode(kb_mode_t mode) {
  if (mode == kbm_poll) {
    /* remove keyboard interrupt handler */
    return 0;
  } else {
    /* install keyboard interrupt handler */
    return 1;
  }
  return 0;
}

int keyboard_init(kb_mode_t mode) {
  keyboard_mode(mode);

  while (inportb(KBC_STATUS) & 1) {
    inportb(KBC_EA);
  }

  send_command(0xF4);
  while (inportb(KBC_STATUS) & 1) {
    inportb(KBC_EA);
  }

  send_command(0xEE);
  unsigned char c = 0;
  while (inportb(KBC_STATUS) & 1) {
    c = inportb(KBC_EA);
  }

  return 0;
}
