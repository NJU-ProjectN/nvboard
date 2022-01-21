#ifndef VFPGA_RENDER_H
#define VFPGA_RENDER_H

#include <SDL2/SDL.h>

#define WINDOW_WIDTH   640
#define WINDOW_HEIGHT  480

#define SWITCH_X       80
#define SWITCH_Y       400
#define SWITCH_SEP     10
#define SWITCH_WIDTH   20
#define SWITCH_HEIGHT  40

#define BTNC_X         500
#define BTNC_Y         200
#define BTNC_SEP       20
#define BTNC_WIDTH     20
#define BTNC_HEIGHT    20

#define LED_X          85
#define LED_Y          360
#define LED_SEP        20
#define LED_WIDTH      10
#define LED_HEIGHT     4

#define SEG_X          40
#define SEG_Y          240

#define SEG_VER_WIDTH  2
#define SEG_VER_HEIGHT 40
#define SEG_HOR_WIDTH  40
#define SEG_HOR_HEIGHT 2
#define SEG_DOT_WIDTH  SEG_VER_WIDTH
#define SEG_DOT_HEIGHT SEG_HOR_HEIGHT

#define SEG_SEP        2

#define SEG_TOT_WIDTH  (SEG_SEP * 17 + SEG_VER_WIDTH * 24 + SEG_HOR_WIDTH * 8)
#define SEG_TOT_HEIGHT (SEG_SEP * 2 + SEG_VER_HEIGHT * 2 + SEG_HOR_HEIGHT * 3)

void load_background(SDL_Renderer *renderer);

void load_texture(SDL_Renderer *renderer);

void init_gui(SDL_Renderer *renderer);

void update_components(SDL_Renderer *renderer);

#endif