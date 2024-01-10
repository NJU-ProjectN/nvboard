#include <nvboard.h>
#include <keyboard.h>
#include "at_scancode.h"

typedef struct {
  SDL_Texture *t_up, *t_down;
  SDL_Rect rect;
  uint8_t map0, map1;
  bool pressing;
} Key;

static KEYBOARD* kb = NULL;
bool is_kb_idle = true;
static Key keys[256] = {};

KEYBOARD::KEYBOARD(SDL_Renderer *rend, int cnt, int init_val, int ct):
  Component(rend, cnt, init_val, ct),
  data_idx(0), left_clk(0), cur_key(NOT_A_KEY) { }


void KEYBOARD::push_key(uint8_t sdl_key, bool is_keydown){
  Key *e = &keys[sdl_key];
  uint8_t at_key = e->map0;
  if(at_key == 0xe0){
    all_keys.push(0xe0);
    at_key = e->map1;
  }
  if(!is_keydown) all_keys.push(0xf0);
  all_keys.push(at_key);
  is_kb_idle = false;

  if (e->pressing != is_keydown) {
    e->pressing = is_keydown;
    SDL_RenderCopy(get_renderer(), (is_keydown ? e->t_down : e->t_up), NULL, &e->rect);
    set_redraw();
  }
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
  SDL_Surface *s = SDL_CreateRGBSurface(0, w, h, 32, 0xff0000, 0x00ff00, 0x0000ff, 0xff000000);
  uint32_t black = SDL_MapRGBA(s->format, 0, 0, 0, 0xff);
  SDL_Rect r;
  r = (SDL_Rect){.x = 0, .y = 0, .w = 1, .h = h}; SDL_FillRect(s, &r, black);
  r = (SDL_Rect){.x = 0, .y = 0, .w = w, .h = 1}; SDL_FillRect(s, &r, black);
  r = (SDL_Rect){.x = w - 1, .y = 0, .w = 1, .h = h}; SDL_FillRect(s, &r, black);
  r = (SDL_Rect){.x = 0, .y = h - 1, .w = w, .h = 1}; SDL_FillRect(s, &r, black);
  return s;
}

static SDL_Surface* surdup(SDL_Surface *src, uint32_t bg) {
  SDL_PixelFormat *f = src->format;
  SDL_Surface *s = SDL_CreateRGBSurface(0, src->w, src->h,
      f->BitsPerPixel, f->Rmask, f->Gmask, f->Bmask, f->Amask);
  SDL_FillRect(s, NULL, bg);
  SDL_BlitSurface(src, NULL, s, NULL);
  return s;
}

static SDL_Texture* gen_key_texture(SDL_Renderer *renderer, const char *desc1,
    const char *desc2, SDL_Surface *key_shape, bool is_down) {
  std::string desc = std::string(desc1) + '\n' + desc2;
  uint32_t color_up = SDL_MapRGBA(key_shape->format, 0xf0, 0xf0, 0xf0, 0xff);
  uint32_t color_dn = SDL_MapRGBA(key_shape->format, 0xc0, 0xc0, 0xc0, 0xff);
  SDL_Surface *s = surdup(key_shape, is_down ? color_dn : color_up);
  SDL_Surface *s_desc = str2surface(desc.c_str(), 0);
  SDL_Rect r = { .x = 1, .y = 1 };
  SDL_BlitSurface(s_desc, NULL, s, &r);
  SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
  SDL_FreeSurface(s_desc);
  SDL_FreeSurface(s);
  return t;
}

static void init_key_texture(SDL_Renderer *renderer, uint8_t sdl_key,
    const char *desc1, const char *desc2, SDL_Surface *key_shape, int x, int y) {
  Key *e = &keys[sdl_key];
  e->t_up   = gen_key_texture(renderer, desc1, desc2, key_shape, false);
  e->t_down = gen_key_texture(renderer, desc1, desc2, key_shape, true);
  e->rect = { .x = x, .y = y, .w = key_shape->w, .h = key_shape->h };
}

static void init_render_local(SDL_Renderer *renderer) {
  const int key_unit_width = 34;
  const int key_gap = key_unit_width / 14;
  const int h_keyboard = key_unit_width * 6 + key_gap * 5 + key_unit_width / 2;
  const int x_top_left = WINDOW_WIDTH / 2 + 1;
  const int y_below_uart = WINDOW_HEIGHT / 2;
  const int y_top_left = y_below_uart + (WINDOW_HEIGHT - y_below_uart - h_keyboard) / 2;
  const int key_gap_before_extend_keys = key_unit_width / 3;

  SDL_Surface *s_1p0 = new_key_shape(key_unit_width, key_unit_width);
  SDL_Surface *s_1p5 = new_key_shape(key_unit_width + key_gap + key_unit_width / 2, key_unit_width);
  SDL_Surface *s_2p0 = new_key_shape(key_unit_width * 2 + key_gap, key_unit_width);
  SDL_Surface *s_2p25= new_key_shape(key_unit_width * 2 + key_gap * 2 + key_unit_width / 4, key_unit_width);
  SDL_Surface *s_6p0 = new_key_shape(key_unit_width * 6 + key_gap * 5, key_unit_width);

  int x = x_top_left, y = y_top_left;

#define _entry(k, desc1, desc2, width) do { \
  int idx = SDL_PREFIX(concat(_, k)); \
  init_key_texture(renderer, idx, desc1, desc2, concat(s_, width), x, y); \
  x += concat(s_, width)->w + key_gap; \
  SDL_RenderCopy(renderer, keys[idx].t_up, NULL, &keys[idx].rect); \
} while (0)

#define nextline() do { x = x_top_left; y += key_unit_width + key_gap; } while (0)

  _entry(ESCAPE,    "ESC", "", 1p0);
  x += key_unit_width / 2 + key_gap;
  _entry(F1,        "F1", "", 1p0);
  _entry(F2,        "F2", "", 1p0);
  _entry(F3,        "F3", "", 1p0);
  _entry(F4,        "F4", "", 1p0);
  x += key_unit_width / 2;
  _entry(F5,        "F5", "", 1p0);
  _entry(F6,        "F6", "", 1p0);
  _entry(F7,        "F7", "", 1p0);
  _entry(F8,        "F8", "", 1p0);
  x += key_unit_width / 2;
  _entry(F9,        "F9", "", 1p0);
  _entry(F10,       "F10", "", 1p0);
  _entry(F11,       "F11", "", 1p0);
  _entry(F12,       "F12", "", 1p0);
  x += key_gap_before_extend_keys;
  _entry(PRINTSCREEN, "Prt", "Scr", 1p0);
  _entry(SCROLLLOCK,  "Scr", "Lck", 1p0);
  _entry(PAUSE,       "Pa-", "use", 1p0);

  nextline();
  y += key_unit_width / 2;  // more space between the 1st and 2nd line
  _entry(GRAVE,     "~", "`", 1p0);
  _entry(1,         "!", "1", 1p0);
  _entry(2,         "@", "2", 1p0);
  _entry(3,         "#", "3", 1p0);
  _entry(4,         "$", "4", 1p0);
  _entry(5,         "%", "5", 1p0);
  _entry(6,         "^", "6", 1p0);
  _entry(7,         "&", "7", 1p0);
  _entry(8,         "*", "8", 1p0);
  _entry(9,         "(", "9", 1p0);
  _entry(0,         ")", "0", 1p0);
  _entry(MINUS,     "_", "-", 1p0);
  _entry(EQUALS,    "+", "=", 1p0);
  _entry(BACKSPACE, "Back", "Space", 1p5);
  x += key_gap_before_extend_keys - key_gap;
  _entry(INSERT,    "Ins",  "", 1p0);
  _entry(HOME,      "Ho-", "me", 1p0);
  _entry(PAGEUP,    "Pg", "Up", 1p0);

  nextline();
  _entry(TAB,       "Tab", "", 1p5);
  _entry(Q,         "Q", "", 1p0);
  _entry(W,         "W", "", 1p0);
  _entry(E,         "E", "", 1p0);
  _entry(R,         "R", "", 1p0);
  _entry(T,         "T", "", 1p0);
  _entry(Y,         "Y", "", 1p0);
  _entry(U,         "U", "", 1p0);
  _entry(I,         "I", "", 1p0);
  _entry(O,         "O", "", 1p0);
  _entry(P,         "P", "", 1p0);
  _entry(LEFTBRACKET, "{", "[", 1p0);
  _entry(RIGHTBRACKET, "}", "]", 1p0);
  _entry(BACKSLASH, "|", "\\", 1p0);
  x += key_gap_before_extend_keys - key_gap;
  _entry(DELETE,    "Del",  "", 1p0);
  _entry(END,       "End",  "", 1p0);
  _entry(PAGEDOWN,  "Pg", "Dn", 1p0);

  nextline();
  _entry(CAPSLOCK,  "Caps", "Lock", 2p0);
  _entry(A,         "A", "", 1p0);
  _entry(S,         "S", "", 1p0);
  _entry(D,         "D", "", 1p0);
  _entry(F,         "F", "", 1p0);
  _entry(G,         "G", "", 1p0);
  _entry(H,         "H", "", 1p0);
  _entry(J,         "J", "", 1p0);
  _entry(K,         "K", "", 1p0);
  _entry(L,         "L", "", 1p0);
  _entry(SEMICOLON, ":", ";", 1p0);
  _entry(APOSTROPHE,"\"", "'", 1p0);
  _entry(RETURN,    "Enter", "", 1p5);

  nextline();
  _entry(LSHIFT,    "Shift", "", 2p25);
  _entry(Z,         "Z", "", 1p0);
  _entry(X,         "X", "", 1p0);
  _entry(C,         "C", "", 1p0);
  _entry(V,         "V", "", 1p0);
  _entry(B,         "B", "", 1p0);
  _entry(N,         "N", "", 1p0);
  _entry(M,         "M", "", 1p0);
  _entry(COMMA,     "<", ",", 1p0);
  _entry(PERIOD,    ">", ".", 1p0);
  _entry(SLASH,     "?", "/", 1p0);
  _entry(RSHIFT,    "Shift", "", 2p25);
  x += key_gap_before_extend_keys + key_unit_width;
  _entry(UP,        " ^", " |", 1p0);

  nextline();
  _entry(LCTRL,    "Ctrl", "", 1p5);
  x += key_unit_width + key_gap + key_unit_width / 4;
  _entry(LALT,     "Alt", "", 1p5);
  _entry(SPACE,    "Space", "", 6p0);
  _entry(RALT,     "Alt", "", 1p5);
  x += key_unit_width + key_gap + key_unit_width / 4;
  _entry(RCTRL,    "Ctrl", "", 1p5);
  x += key_gap_before_extend_keys - key_gap;
  _entry(LEFT,      "<-", "", 1p0);
  _entry(DOWN,      " |", " V", 1p0);
  _entry(RIGHT,     "->", "", 1p0);

  SDL_FreeSurface(s_1p0);
  SDL_FreeSurface(s_1p5);
  SDL_FreeSurface(s_2p0);
  SDL_FreeSurface(s_2p25);
  SDL_FreeSurface(s_6p0);


  // draw line
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0);
  SDL_Point p[3];
  p[0] = Point(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
  p[1] = p[0] - Point(16, 16);
  p[2] = p[1] - Point(20, 0);
  draw_thicker_line(renderer, p, 3);

  // draw label
  const char *str = "PS/2 Keyboard";
  draw_str(renderer, str, p[2].x - strlen(str) * CH_WIDTH, p[2].y - CH_HEIGHT / 2, 0xffffff);
}

void init_keyboard(SDL_Renderer *renderer) {
  init_render_local(renderer);
  kb = new KEYBOARD(renderer, 0, 0, KEYBOARD_TYPE);
  for (int p = PS2_CLK; p <= PS2_DAT; p ++) {
    kb->add_pin(p);
  }
#define FILL_KEYMAP0(a) keys[SDL_PREFIX(a)].map0 = GET_FIRST(AT_PREFIX(a));
#define FILL_KEYMAP1(a) keys[SDL_PREFIX(a)].map1 = GET_SECOND(AT_PREFIX(a));
  MAP(SCANCODE_LIST, FILL_KEYMAP0)
  MAP(SCANCODE_LIST, FILL_KEYMAP1)
}

void kb_update() {
  kb->update_state();
}

void kb_push_key(uint8_t scancode, bool is_keydown){
  kb->push_key(scancode, is_keydown);
}
