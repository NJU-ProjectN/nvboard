#include <keyboard.h>
#include <pins.h>
#include <configs.h>
#include "at_scancode.h"
#include <string>

static KEYBOARD* kb = NULL;
bool is_kb_idle = true;
SDL_Surface* get_font_surface(const char *str);

#define FILL_KEYMAP_FIRST(a) keymap_first[SDL_PREFIX(a)] = GET_FIRST(AT_PREFIX(a));
#define FILL_KEYMAP_DECOND(a) keymap_second[SDL_PREFIX(a)] = GET_SECOND(AT_PREFIX(a));

static int keymap_first[256] = {};
static int keymap_second[256] = {};

KEYBOARD::KEYBOARD(SDL_Renderer *rend, int cnt, int init_val, int ct):
  Component(rend, cnt, init_val, ct),
  data_idx(0), left_clk(0), cur_key(NOT_A_KEY) { }


void KEYBOARD::push_key(uint8_t sdl_key, bool is_keydown){
  uint8_t at_key = keymap_first[sdl_key];
  if(at_key == 0xe0){
    all_keys.push(0xe0);
    at_key = keymap_second[sdl_key];
  }
  if(!is_keydown) all_keys.push(0xf0);
  all_keys.push(at_key);
  is_kb_idle = false;
}

void KEYBOARD::update_state(){
  if(cur_key == NOT_A_KEY){
    if(all_keys.empty()) {
      is_kb_idle = true;
      return;
    }
    cur_key = all_keys.front();
    assert(data_idx == 0);
    left_clk = CLK_NUM;
  }

  if(left_clk == 0){
    uint8_t ps2_clk = pin_peek(PS2_CLK);
    ps2_clk = !ps2_clk;
    pin_poke(PS2_CLK, ps2_clk);
    left_clk = CLK_NUM;
    if(ps2_clk){
      assert(!all_keys.empty());
      uint8_t ps2_dat = (data_idx == PS2_PARTIAL) ? !UINT8_XOR(all_keys.front()) : \
                 (data_idx == PS2_STOP) ? 1 : \
                 ((data_idx >= PS2_DATA_0) && (data_idx <= PS2_DATA_7)) ? (cur_key & 1) : 0;
      pin_poke(PS2_DAT, ps2_dat);
      if((data_idx >= PS2_DATA_0) && (data_idx <= PS2_DATA_7)) cur_key >>= 1;
      data_idx ++;
    } else if(data_idx == 11){
      data_idx = 0;
      cur_key = NOT_A_KEY;
      all_keys.pop();
    }
  }
  else{
    left_clk --;
  }
}

static SDL_Surface* new_key_shape(int w, int h) {
  SDL_Surface *s = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
  uint32_t black = SDL_MapRGB(s->format, 0, 0, 0);
  uint32_t white = SDL_MapRGB(s->format, 0xff, 0xff, 0xff);
  SDL_FillRect(s, NULL, white);
  SDL_Rect r;
  r = (SDL_Rect){.x = 0, .y = 0, .w = 1, .h = h}; SDL_FillRect(s, &r, black);
  r = (SDL_Rect){.x = 0, .y = 0, .w = w, .h = 1}; SDL_FillRect(s, &r, black);
  r = (SDL_Rect){.x = w - 1, .y = 0, .w = 1, .h = h}; SDL_FillRect(s, &r, black);
  r = (SDL_Rect){.x = 0, .y = h - 1, .w = w, .h = 1}; SDL_FillRect(s, &r, black);
  return s;
}

static SDL_Surface* surdup(SDL_Surface *src) {
  SDL_Surface *s = SDL_CreateRGBSurface(0, src->w, src->h, 32, 0, 0, 0, 0);
  SDL_BlitSurface(src, NULL, s, NULL);
  return s;
}

static SDL_Texture* gen_key(SDL_Renderer *renderer, const char *desc_up,
    const char *desc_down, SDL_Surface *key_shape) {
  std::string desc = std::string(desc_up) + '\n' + desc_down;
  SDL_Surface *s = surdup(key_shape);
  SDL_Surface *s_desc = get_font_surface(desc.c_str());
  SDL_Rect r = { .x = 1, .y = 1 };
  SDL_BlitSurface(s_desc, NULL, s, &r);
  SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
  SDL_FreeSurface(s_desc);
  SDL_FreeSurface(s);
  return t;
}

static void render_keyboard(SDL_Renderer *renderer) {
  const int key_unit_width = 34;
  const int key_gap = key_unit_width / 14;
  const int x_top_left = WINDOW_WIDTH / 2 + 1;
  const int key_gap_before_extend_keys = key_unit_width / 3;

  SDL_Surface *s_1p0 = new_key_shape(key_unit_width, key_unit_width);
  SDL_Surface *s_1p5 = new_key_shape(key_unit_width + key_gap + key_unit_width / 2, key_unit_width);

  SDL_Texture *t_1 = gen_key(renderer, "!", "1", s_1p0);
  SDL_Texture *t_backspace = gen_key(renderer, "<-", "", s_1p5);
  SDL_Rect r_1p0 = {.x = x_top_left, .y = WINDOW_HEIGHT / 2 + 50, .w = s_1p0->w, .h = s_1p0->h };
  SDL_Rect r_1p5 = {.x = x_top_left, .y = WINDOW_HEIGHT / 2 + 50, .w = s_1p5->w, .h = s_1p5->h };
  SDL_FreeSurface(s_1p0);
  SDL_FreeSurface(s_1p5);


  SDL_Rect r = r_1p0;
  for (int i = 0; i < 13; i ++) {
    SDL_RenderCopy(renderer, t_1, NULL, &r);
    r.x += r_1p0.w + key_gap;
  }
  r.w = r_1p5.w;
  SDL_RenderCopy(renderer, t_backspace, NULL, &r);
  r.x += r_1p5.w + key_gap_before_extend_keys;
  r.w = r_1p0.w;
  for (int i = 0; i < 3; i ++) {
    SDL_RenderCopy(renderer, t_1, NULL, &r);
    r.x += r_1p0.w + key_gap;
  }
}

void init_keyboard(SDL_Renderer *renderer) {
  kb = new KEYBOARD(renderer, 0, 0, KEYBOARD_TYPE);
  for (int p = PS2_CLK; p <= PS2_DAT; p ++) {
    kb->add_pin(p);
  }
  MAP(SCANCODE_LIST, FILL_KEYMAP_FIRST)
  MAP(SCANCODE_LIST, FILL_KEYMAP_DECOND)

  render_keyboard(renderer);
}

void kb_update() {
  kb->update_state();
}

void kb_push_key(uint8_t scancode, bool is_keydown){
  kb->push_key(scancode, is_keydown);
}
