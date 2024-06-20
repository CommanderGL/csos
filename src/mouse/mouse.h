#include "../graphics/catppuccin.h"
#include "../graphics/graphics.h"
#include "../types.h"

#ifndef MOUSE_H
#define MOUSE_H

#define CURSOR_WIDTH 12
#define CURSOR_HEIGHT 19
#define CURSOR_COLOR CATPPUCCIN_MAUVE

extern const uint32_t cursor_sprite[CURSOR_HEIGHT][CURSOR_WIDTH];
extern byte mouse_cycle;
extern char mouse_byte[3];
extern int mouse_x;
extern int mouse_y;
extern int old_mouse_x;
extern int old_mouse_y;

void draw_cursor(void);
void mouse_init(void);

#endif
