#ifndef VFPGA_RENDER_H
#define VFPGA_RENDER_H

#include <SDL2/SDL.h>

#define WINDOW_WIDTH   640
#define WINDOW_HEIGHT  480

#define SWITCH_X       60
#define SWITCH_Y       400
#define SWITCH_SEP     10
#define SWITCH_WIDTH   20
#define SWITCH_HEIGHT  40

#define BTNC_X         520
#define BTNC_Y         250
#define BTNC_SEP       20
#define BTNC_WIDTH     30
#define BTNC_HEIGHT    30

#define LED_X          60 + (4/2)
#define LED_Y          360
#define LED_SEP        14
#define LED_WIDTH      16
#define LED_HEIGHT     8

#define SEG_X          60
#define SEG_Y          225

#define SEG_VER_WIDTH  3
#define SEG_VER_HEIGHT 30
#define SEG_HOR_WIDTH  30
#define SEG_HOR_HEIGHT 3
#define SEG_DOT_WIDTH  4
#define SEG_DOT_HEIGHT 4

#define SEG_SEP        3

#define SEG_TOT_WIDTH  (SEG_SEP * 18 + SEG_VER_WIDTH * 16 + SEG_DOT_WIDTH * 8 + SEG_HOR_WIDTH * 8)
#define SEG_TOT_HEIGHT (SEG_SEP * 4 + SEG_VER_HEIGHT * 2 + SEG_HOR_HEIGHT * 3)

void load_background(SDL_Renderer *renderer);

void load_texture(SDL_Renderer *renderer);

#endif
