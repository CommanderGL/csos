#include "keyboard.h"
#include "../interupts/idt.h"
#include "../interupts/ints.h"
#include "../io.h"
#include <stdbool.h>
#include <stddef.h>

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
bool key_modifier_caps = false;

uint8_t scancode_to_keycode(Scancode sc) {
  bool release = false;
  if (sc & 0x80) {
    release = true;
    sc -= 0x80;
  }

  switch (sc) {
  case KEYCODE_LSHIFT:
  case KEYCODE_RSHIFT:
    key_modifier_shift = !release;
    return 0;
  case KEYCODE_CAPS:
    if (!release) key_modifier_caps = !key_modifier_caps;
    return 0;
  default:
    break;
  }

  if (key_modifier_shift || key_modifier_caps) return keymap_shift[sc];
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

kb_handler kb_handlers[0xff];
int kb_handlers_length = 0;

void keyboard_handler_c() {
  Scancode sc = inportb(0x60);

  for (int i = 0; i < 0xff; i++) {
    if (kb_handlers[i] != NULL) kb_handlers[i](sc, scancode_to_keycode(sc));
  }

  outportb(0x20, 0x20);
}

int keyboard_init(kb_mode_t mode) {
  if (mode == kbm_poll) {
    while (inportb(KBC_STATUS) & 1) {
      inportb(KBC_EA);
    }

    send_command(0xF4);
    while (inportb(KBC_STATUS) & 1) {
      inportb(KBC_EA);
    }

    send_command(0xEE);
    while (inportb(KBC_STATUS) & 1) {
      inportb(KBC_EA);
    }

    return 0;
  }
  addInt(33, keyboard_handler, 0);

  return 0;
}
