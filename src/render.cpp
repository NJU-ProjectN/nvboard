#include <nboard.h>
#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cassert>
#include <cstdlib>
#include <iostream>

static SDL_Surface *sbutton_on, *sbutton_off;
static SDL_Surface *sswitch_on, *sswitch_off;
static SDL_Surface *ssegled_ver_off, *ssegled_ver_on,
                   *ssegled_hor_off, *ssegled_hor_on,
                   *ssegled_dot_off, *ssegled_dot_on;
static SDL_Surface *sled_off, *sled_r, *sled_g, *sled_b,
                   *sled_rg, *sled_rb, *sled_gb, *sled_rgb;
static SDL_Surface *sfpga_background;
static SDL_Surface *sseg7_background;

static SDL_Texture *tbutton_on, *tbutton_off;
static SDL_Texture *tswitch_on, *tswitch_off;
static SDL_Texture *tsegled_ver_off, *tsegled_ver_on,
                   *tsegled_hor_off, *tsegled_hor_on,
                   *tsegled_dot_off, *tsegled_dot_on;
static SDL_Texture *tled_off, *tled_r, *tled_g, *tled_b,
                   *tled_rg, *tled_rb, *tled_gb, *tled_rgb;
static SDL_Texture *tfpga_background;
static SDL_Texture *tseg7_background;

extern std::string nboard_home;

void load_background(SDL_Renderer *renderer) {
  sfpga_background = IMG_Load((nboard_home + "/pic/" + BG_PATH).c_str());
  tfpga_background = SDL_CreateTextureFromSurface(renderer, sfpga_background);
  SDL_RenderCopy(renderer, tfpga_background, NULL, NULL);

#ifdef SEG_BKGND_ENA
#ifdef SEG_BKGND_CUSTOM
  sseg7_background = IMG_Load((nboard_home + "/pic/" + VSEGLED_BG_PATH).c_str());
#else
  sseg7_background = SDL_CreateRGBSurface(0, SEG_TOT_WIDTH, SEG_TOT_HEIGHT, 32, 0, 0, 0, 0);
  SDL_FillRect(sseg7_background, NULL, SDL_MapRGB(sseg7_background->format, 0x00, 0x00, 0x00));
#endif
  
  tseg7_background = SDL_CreateTextureFromSurface(renderer, sseg7_background);
  SDL_Rect rect = {SEG_X, SEG_Y, SEG_TOT_WIDTH, SEG_TOT_HEIGHT};
  SDL_RenderCopy(renderer, tseg7_background, NULL, &rect);
#endif
  SDL_RenderPresent(renderer);
}

void fill_rect_texture(SDL_Renderer *renderer, SDL_Surface **surface, SDL_Texture **texture, int r, int g, int b) {
  SDL_FillRect(*surface, NULL, SDL_MapRGB((*surface)->format, r, g, b));
  *texture = SDL_CreateTextureFromSurface(renderer, *surface);
}

void load_texture(SDL_Renderer *renderer) {
  // buttons
  sbutton_on = IMG_Load((nboard_home + "/pic/" + VBTN_ON_PATH).c_str());
  assert(sbutton_on != nullptr);
  tbutton_on = SDL_CreateTextureFromSurface(renderer, sbutton_on);
  sbutton_off = IMG_Load((nboard_home + "/pic/" + VBTN_OFF_PATH).c_str());
  assert(sbutton_off != nullptr);
  tbutton_off = SDL_CreateTextureFromSurface(renderer, sbutton_off);

  // switches
  sswitch_on = IMG_Load((nboard_home + "/pic/" + VSW_ON_PATH).c_str());
  tswitch_on = SDL_CreateTextureFromSurface(renderer, sswitch_on);
  sswitch_off = IMG_Load((nboard_home + "/pic/" + VSW_OFF_PATH).c_str());
  tswitch_off = SDL_CreateTextureFromSurface(renderer, sswitch_off);
  
  // 7 segs
  // vertical
  ssegled_ver_on = SDL_CreateRGBSurface(0, SEG_VER_WIDTH, SEG_VER_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &ssegled_ver_on, &tsegled_ver_on, 0xff, 0x00, 0x00);
  ssegled_ver_off = SDL_CreateRGBSurface(0, SEG_VER_WIDTH, SEG_VER_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &ssegled_ver_off, &tsegled_ver_off, 0x7f, 0x7f, 0x7f);

  // horizontal
  ssegled_hor_on = SDL_CreateRGBSurface(0, SEG_HOR_WIDTH, SEG_HOR_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &ssegled_hor_on, &tsegled_hor_on, 0xff, 0x00, 0x00);
  ssegled_hor_off = SDL_CreateRGBSurface(0, SEG_HOR_WIDTH, SEG_HOR_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &ssegled_hor_off, &tsegled_hor_off, 0x7f, 0x7f, 0x7f);
  
  // dot
  ssegled_dot_on = SDL_CreateRGBSurface(0, SEG_DOT_WIDTH, SEG_DOT_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &ssegled_dot_on, &tsegled_dot_on, 0xff, 0x00, 0x00);
  ssegled_dot_off = SDL_CreateRGBSurface(0, SEG_DOT_WIDTH, SEG_DOT_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &ssegled_dot_off, &tsegled_dot_off, 0x7f, 0x7f, 0x7f);
  
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

extern std::map<std::string, bool> input_map, output_map;
extern std::map<std::string, bool> prev_input_map, prev_output_map;

extern std::string input_pins[], output_pins[];
extern const int input_size, output_size;

std::string input_btns[] = {
  BTN_INPUT, RST_INPUT
};
static const int btns_cnt = sizeof(input_btns) / sizeof(std::string);

std::string input_switches[] = {
  SWITCH_INPUT
};
static const int switches_cnt = sizeof(input_switches) / sizeof(std::string);

static void update_gui_switch(SDL_Renderer *renderer, int index, bool val) {
  index = 15 - index;
  SDL_Rect rect = {SWITCH_X + (SWITCH_WIDTH + SWITCH_SEP) * index, SWITCH_Y, SWITCH_WIDTH, SWITCH_HEIGHT};
  SDL_RenderCopy(renderer, val ? tswitch_on : tswitch_off, NULL, &rect);
}

static void update_gui_button(SDL_Renderer *renderer, int index, bool val) {
  SDL_Rect rect;
  switch (index)
  {
  case 0: // BTNC
    rect = {BTNC_X, BTNC_Y, BTNC_WIDTH, BTNC_HEIGHT};
    break;
  case 1: // BTNU
    rect = {BTNC_X, BTNC_Y - BTNC_HEIGHT - BTNC_SEP, BTNC_WIDTH, BTNC_HEIGHT};
    break;
  case 2: // BTND
    rect = {BTNC_X, BTNC_Y + BTNC_HEIGHT + BTNC_SEP, BTNC_WIDTH, BTNC_HEIGHT};
    break;
  case 3: // BTNL
    rect = {BTNC_X - BTNC_WIDTH - BTNC_SEP, BTNC_Y, BTNC_WIDTH, BTNC_HEIGHT};
    break;
  case 4: // BTNR
    rect = {BTNC_X + BTNC_WIDTH + BTNC_SEP, BTNC_Y, BTNC_WIDTH, BTNC_HEIGHT};
    break;
  case 5: // RST
    rect = {BTNC_X, BTNC_Y - 2 * (BTNC_HEIGHT + BTNC_SEP), BTNC_WIDTH, BTNC_HEIGHT};
    break;
  default:
    assert(0);
    break;
  }
  SDL_RenderCopy(renderer, val ? tbutton_on : tbutton_off, NULL, &rect);
}

void update_gui_input(SDL_Renderer *renderer) {
  for (int i = 0; i < switches_cnt; ++i) {
    std::string &str = input_switches[i];
    if (input_map.count(str) && prev_input_map[str] != input_map[str]) {
      prev_input_map[str] = input_map[str];
      update_gui_switch(renderer, i, input_map[str]);
    }
  }

  for (int i = 0; i < btns_cnt; ++i) {
    std::string &str = input_btns[i];
    if (input_map.count(str) && prev_input_map[str] != input_map[str]) {
      prev_input_map[str] = input_map[str];
      update_gui_button(renderer, i, input_map[str]);
    }
  }
}

std::string output_naive_leds[] = {
  NAIVE_LEDS
};
const int naive_leds_cnt = sizeof(output_naive_leds) / sizeof(std::string);

std::string output_rgb_leds[] = {
  RGB_LEDS
};

std::string output_seg7_leds[] = {
  SEG7_LEDS
};

static void update_gui_naive_led(SDL_Renderer *renderer, int index, bool val) {
  //std::cout << "update " << index << std::endl;
  index = 15 - index;
  SDL_Rect rect = {LED_X + (LED_WIDTH + LED_SEP) * index, LED_Y, LED_WIDTH, LED_HEIGHT};
  SDL_RenderCopy(renderer, val ? tled_g : tled_off, NULL, &rect);
}

static void update_gui_rgb_led(SDL_Renderer *renderer, int index, int color) {
  SDL_Texture *texture = nullptr;
  switch (color)
  {
  case 0b000:
    texture = tled_off;
    break;
  case 0b001:
    texture = tled_b;
    break;
  case 0b010:
    texture = tled_g;
    break;
  case 0b011:
    texture = tled_gb;
    break;
  case 0b100:
    texture = tled_r;
    break;
  case 0b101:
    texture = tled_rb;
    break;
  case 0b110:
    texture = tled_rg;
    break;
  case 0b111:
    texture = tled_rgb;
    break;
  default:
    assert(0);
    break;
  }
}

// After SDL receives the changes of inputs, call update_gui_input() 

// After updating the outputs of module, call update_gui_output()
void update_gui_output(SDL_Renderer *renderer) {
  // check naive_leds
  for (int i = 0; i < naive_leds_cnt; ++i) {
    std::string &str = output_naive_leds[i];
    if (output_map.count(str) && prev_output_map[str] != output_map[str]) {
      prev_output_map[str] = output_map[str];
      update_gui_naive_led(renderer, i, output_map[str]);
    }
  }
  // check rgb_leds
  for (int i = 0; i < 2; ++i) {
    bool flag = false;
    int color = 0;
    for (int j = 0; j < 3; ++j) {
      std::string &str = output_rgb_leds[i * 3 + j];
      if (output_map.count(str)) {
        if (prev_output_map[str] != output_map[str]) {
          flag = true;
          color = (color << 1) | (prev_output_map[str] = output_map[str]);
        }
      }
    }
    if (flag) {
      update_gui_rgb_led(renderer, i, color);
    }
  }
  
  // check seg7s

  SDL_RenderPresent(renderer);
  return;
}

// render buttons, switches, leds and 7-segs
void init_gui(SDL_Renderer *renderer) {
  
  for (int i = 0; i < switches_cnt; ++i) {
    update_gui_switch(renderer, i, 0);
  }

  for (int i = 0; i < btns_cnt; ++i) {
    update_gui_button(renderer, i, 0);
  }
  //SDL_RenderPresent(renderer);
  //dbg_wait_esc();
  for (int i = 0; i < naive_leds_cnt; ++i) {
    update_gui_naive_led(renderer, i, 0);
  }
 
  SDL_RenderPresent(renderer);
}