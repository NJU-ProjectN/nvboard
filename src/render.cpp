#include <render.h>
#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <configs.h>
#include <component.h>

SDL_Surface *sbutton_on, *sbutton_off;
SDL_Surface *sswitch_on, *sswitch_off;
SDL_Surface *ssegled_ver_off, *ssegled_ver_on,
            *ssegled_hor_off, *ssegled_hor_on,
            *ssegled_dot_off, *ssegled_dot_on;
SDL_Surface *sled_off, *sled_r, *sled_g, *sled_b,
            *sled_rg, *sled_rb, *sled_gb, *sled_rgb;
SDL_Surface *sfpga_background;
SDL_Surface *sseg7_background;

SDL_Texture *tbutton_on, *tbutton_off;
SDL_Texture *tswitch_on, *tswitch_off;
SDL_Texture *tsegled_ver_off, *tsegled_ver_on,
            *tsegled_hor_off, *tsegled_hor_on,
            *tsegled_dot_off, *tsegled_dot_on;
SDL_Texture *tled_off, *tled_r, *tled_g, *tled_b,
            *tled_rg, *tled_rb, *tled_gb, *tled_rgb;
SDL_Texture *tfpga_background, *tseg7_background;

SDL_Rect segs_rect[8] = {
  {SEG_SEP + SEG_VER_WIDTH,                     SEG_SEP                                                         , SEG_HOR_WIDTH, SEG_HOR_HEIGHT },
  {SEG_SEP + SEG_VER_WIDTH + SEG_HOR_WIDTH,     SEG_SEP + SEG_HOR_HEIGHT                                        , SEG_VER_WIDTH, SEG_VER_HEIGHT },
  {SEG_SEP + SEG_VER_WIDTH + SEG_HOR_WIDTH,     SEG_SEP + 2*SEG_HOR_HEIGHT + SEG_VER_HEIGHT                     , SEG_VER_WIDTH, SEG_VER_HEIGHT },
  {SEG_SEP + SEG_VER_WIDTH,                     SEG_SEP + 2*SEG_HOR_HEIGHT + 2*SEG_VER_HEIGHT                   , SEG_HOR_WIDTH, SEG_HOR_HEIGHT },
  {SEG_SEP,                                     SEG_SEP + 2*SEG_HOR_HEIGHT + SEG_VER_HEIGHT                     , SEG_VER_WIDTH, SEG_VER_HEIGHT },
  {SEG_SEP,                                     SEG_SEP + SEG_HOR_HEIGHT                                        , SEG_VER_WIDTH, SEG_VER_HEIGHT },
  {SEG_SEP + SEG_VER_WIDTH,                     SEG_SEP + SEG_HOR_HEIGHT + SEG_VER_HEIGHT                       , SEG_HOR_WIDTH, SEG_HOR_HEIGHT },
  {2*SEG_SEP + 2*SEG_VER_WIDTH + SEG_HOR_WIDTH, SEG_SEP + 3*SEG_HOR_HEIGHT + 2*SEG_VER_HEIGHT - SEG_DOT_HEIGHT  , SEG_DOT_WIDTH, SEG_DOT_HEIGHT }
};

SDL_Texture *segs_texture(int index, int val) {
  switch (index) {
  case 0:
  case 3:
  case 6:
    return val ? tsegled_hor_on : tsegled_hor_off;
    break;
  case 1:
  case 2:
  case 4:
  case 5:
    return val ? tsegled_ver_on : tsegled_ver_off;
    break;
  case 7:
    return val ? tsegled_dot_on : tsegled_dot_off;
    break;
  default:
    assert(0);
    break;
  }
  return nullptr;
}

SDL_Rect btn_rects[6] = {
  {BTNC_X,                            BTNC_Y,                                 BTNC_WIDTH, BTNC_HEIGHT },
  {BTNC_X,                            BTNC_Y - BTNC_HEIGHT - BTNC_SEP,        BTNC_WIDTH, BTNC_HEIGHT },
  {BTNC_X,                            BTNC_Y + BTNC_HEIGHT + BTNC_SEP,        BTNC_WIDTH, BTNC_HEIGHT },
  {BTNC_X - BTNC_WIDTH - BTNC_SEP,    BTNC_Y,                                 BTNC_WIDTH, BTNC_HEIGHT },
  {BTNC_X + BTNC_WIDTH + BTNC_SEP,    BTNC_Y,                                 BTNC_WIDTH, BTNC_HEIGHT },
  {BTNC_X + BTNC_WIDTH + BTNC_SEP/2,  BTNC_Y - 2 * (BTNC_HEIGHT + BTNC_SEP),  BTNC_WIDTH, BTNC_HEIGHT }
};

extern std::string nvboard_home;

void load_background(SDL_Renderer *renderer) {
  sfpga_background = IMG_Load((nvboard_home + "/pic/" + BG_PATH).c_str());
  tfpga_background = SDL_CreateTextureFromSurface(renderer, sfpga_background);
  SDL_Rect rect_bg = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
  SDL_RenderCopy(renderer, tfpga_background, NULL, &rect_bg);
  
#ifdef SEG_BKGND_ENA
#ifdef SEG_BKGND_CUSTOM
  sseg7_background = IMG_Load((nvboard_home + "/pic/" + VSEGLED_BG_PATH).c_str());
#else
  sseg7_background = SDL_CreateRGBSurface(0, SEG_TOT_WIDTH, SEG_TOT_HEIGHT, 32, 0, 0, 0, 0);
  SDL_FillRect(sseg7_background, NULL, SDL_MapRGB(sseg7_background->format, 0x00, 0x00, 0x00));
#endif
  
  tseg7_background = SDL_CreateTextureFromSurface(renderer, sseg7_background);
  SDL_Rect rect_seg7 = {SEG_X, SEG_Y, SEG_TOT_WIDTH, SEG_TOT_HEIGHT};
  SDL_RenderCopy(renderer, tseg7_background, NULL, &rect_seg7);
#endif
}

void fill_rect_texture(SDL_Renderer *renderer, SDL_Surface **surface, SDL_Texture **texture, int r, int g, int b) {
  SDL_FillRect(*surface, NULL, SDL_MapRGB((*surface)->format, r, g, b));
  *texture = SDL_CreateTextureFromSurface(renderer, *surface);
}

void load_texture(SDL_Renderer *renderer) {
  // buttons
  sbutton_on = IMG_Load((nvboard_home + "/pic/" + VBTN_ON_PATH).c_str());
  assert(sbutton_on != nullptr);
  tbutton_on = SDL_CreateTextureFromSurface(renderer, sbutton_on);
  sbutton_off = IMG_Load((nvboard_home + "/pic/" + VBTN_OFF_PATH).c_str());
  assert(sbutton_off != nullptr);
  tbutton_off = SDL_CreateTextureFromSurface(renderer, sbutton_off);

  // switches
  sswitch_on = IMG_Load((nvboard_home + "/pic/" + VSW_ON_PATH).c_str());
  tswitch_on = SDL_CreateTextureFromSurface(renderer, sswitch_on);
  sswitch_off = IMG_Load((nvboard_home + "/pic/" + VSW_OFF_PATH).c_str());
  tswitch_off = SDL_CreateTextureFromSurface(renderer, sswitch_off);
  
  // 7 segs
  // vertical
  ssegled_ver_on = SDL_CreateRGBSurface(0, SEG_VER_WIDTH, SEG_VER_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &ssegled_ver_on, &tsegled_ver_on, 0xff, 0x00, 0x00);
  ssegled_ver_off = SDL_CreateRGBSurface(0, SEG_VER_WIDTH, SEG_VER_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &ssegled_ver_off, &tsegled_ver_off, 0x2b, 0x2b, 0x2b);

  // horizontal
  ssegled_hor_on = SDL_CreateRGBSurface(0, SEG_HOR_WIDTH, SEG_HOR_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &ssegled_hor_on, &tsegled_hor_on, 0xff, 0x00, 0x00);
  ssegled_hor_off = SDL_CreateRGBSurface(0, SEG_HOR_WIDTH, SEG_HOR_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &ssegled_hor_off, &tsegled_hor_off, 0x2b, 0x2b, 0x2b);
  
  // dot
  ssegled_dot_on = SDL_CreateRGBSurface(0, SEG_DOT_WIDTH, SEG_DOT_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &ssegled_dot_on, &tsegled_dot_on, 0xff, 0x00, 0x00);
  ssegled_dot_off = SDL_CreateRGBSurface(0, SEG_DOT_WIDTH, SEG_DOT_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &ssegled_dot_off, &tsegled_dot_off, 0x2b, 0x2b, 0x2b);
  
  // LEDs
  sled_off = SDL_CreateRGBSurface(0, LED_WIDTH, LED_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &sled_off, &tled_off, 0x7f, 0x7f, 0x7f);

  sled_r = SDL_CreateRGBSurface(0, LED_WIDTH, LED_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &sled_r, &tled_r, 0xff, 0x00, 0x00);

  sled_g = SDL_CreateRGBSurface(0, LED_WIDTH, LED_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &sled_g, &tled_g, 0x00, 0xff, 0x00);
  
  sled_b = SDL_CreateRGBSurface(0, LED_WIDTH, LED_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &sled_b, &tled_b, 0x00, 0x00, 0xff);

  sled_rg = SDL_CreateRGBSurface(0, LED_WIDTH, LED_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &sled_rg, &tled_rg, 0xff, 0xff, 0x00);

  sled_rb = SDL_CreateRGBSurface(0, LED_WIDTH, LED_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &sled_rb, &tled_rb, 0xff, 0x00, 0xff);

  sled_gb = SDL_CreateRGBSurface(0, LED_WIDTH, LED_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &sled_gb, &tled_gb, 0x00, 0xff, 0xff);

  sled_rgb = SDL_CreateRGBSurface(0, LED_WIDTH, LED_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &sled_rgb, &tled_rgb, 0xff, 0xff, 0xff);
}
