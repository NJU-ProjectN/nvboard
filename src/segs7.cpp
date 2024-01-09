#include <nvboard.h>

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

#define GET_SEGA(i) (SEG0A + 8 * i)
#define GET_DECP(i) (SEG0A + 8 * i + 7)

static SDL_Texture *tsegled_ver_off, *tsegled_ver_on,
                   *tsegled_hor_off, *tsegled_hor_on,
                   *tsegled_dot_off, *tsegled_dot_on;

static SDL_Rect segs_rect[8] = {
  {SEG_SEP + SEG_VER_WIDTH,                     SEG_SEP                                                         , SEG_HOR_WIDTH, SEG_HOR_HEIGHT },
  {SEG_SEP + SEG_VER_WIDTH + SEG_HOR_WIDTH,     SEG_SEP + SEG_HOR_HEIGHT                                        , SEG_VER_WIDTH, SEG_VER_HEIGHT },
  {SEG_SEP + SEG_VER_WIDTH + SEG_HOR_WIDTH,     SEG_SEP + 2*SEG_HOR_HEIGHT + SEG_VER_HEIGHT                     , SEG_VER_WIDTH, SEG_VER_HEIGHT },
  {SEG_SEP + SEG_VER_WIDTH,                     SEG_SEP + 2*SEG_HOR_HEIGHT + 2*SEG_VER_HEIGHT                   , SEG_HOR_WIDTH, SEG_HOR_HEIGHT },
  {SEG_SEP,                                     SEG_SEP + 2*SEG_HOR_HEIGHT + SEG_VER_HEIGHT                     , SEG_VER_WIDTH, SEG_VER_HEIGHT },
  {SEG_SEP,                                     SEG_SEP + SEG_HOR_HEIGHT                                        , SEG_VER_WIDTH, SEG_VER_HEIGHT },
  {SEG_SEP + SEG_VER_WIDTH,                     SEG_SEP + SEG_HOR_HEIGHT + SEG_VER_HEIGHT                       , SEG_HOR_WIDTH, SEG_HOR_HEIGHT },
  {2*SEG_SEP + 2*SEG_VER_WIDTH + SEG_HOR_WIDTH, SEG_SEP + 3*SEG_HOR_HEIGHT + 2*SEG_VER_HEIGHT - SEG_DOT_HEIGHT  , SEG_DOT_WIDTH, SEG_DOT_HEIGHT }
};

static SDL_Texture *segs_texture(int index, int val) {
  switch (index) {
    case 0: case 3: case 6:         return val ? tsegled_hor_on : tsegled_hor_off;
    case 1: case 2: case 4: case 5: return val ? tsegled_ver_on : tsegled_ver_off;
    case 7: return val ? tsegled_dot_on : tsegled_dot_off;
    default: assert(0); return nullptr;
  }
}

SEGS7::SEGS7(SDL_Renderer *rend, int cnt, int init_val, int ct, bool is_len8)
  : Component(rend, cnt, init_val, ct), is_len8(is_len8) {}

void SEGS7::update_gui() {
  int newval = get_state();
  for (int i = 0; i < 8; ++i) {
    int texture_idx = (7 - i) * 2 + (((newval >> i) & 1) ? 0 : 1);
    SDL_RenderCopy(get_renderer(), get_texture(texture_idx), NULL, get_rect(texture_idx));
  }
  set_redraw();
}

void SEGS7::update_state() {
  int newval = 0;
  if (is_len8) {
    newval = pin_peek8(get_pin());
  } else {
    for (int i = 0; i < 8; ++i) {
      newval |= (pin_peek(get_pin(7 - i)) << i);
    }
  }
  if (newval != get_state()) {
    set_state(newval);
    update_gui();
  }
}

static void init_render_local(SDL_Renderer *renderer) {
  // vertical
  tsegled_ver_on  = new_texture(renderer, SEG_VER_WIDTH, SEG_VER_HEIGHT, 0xff, 0x00, 0x00);
  tsegled_ver_off = new_texture(renderer, SEG_VER_WIDTH, SEG_VER_HEIGHT, 0x2b, 0x2b, 0x2b);

  // horizontal
  tsegled_hor_on  = new_texture(renderer, SEG_HOR_WIDTH, SEG_HOR_HEIGHT, 0xff, 0x00, 0x00);
  tsegled_hor_off = new_texture(renderer, SEG_HOR_WIDTH, SEG_HOR_HEIGHT, 0x2b, 0x2b, 0x2b);

  // dot
  tsegled_dot_on  = new_texture(renderer, SEG_DOT_WIDTH, SEG_DOT_HEIGHT, 0xff, 0x00, 0x00);
  tsegled_dot_off = new_texture(renderer, SEG_DOT_WIDTH, SEG_DOT_HEIGHT, 0x2b, 0x2b, 0x2b);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_Rect rect_seg7 = {SEG_X, SEG_Y, SEG_TOT_WIDTH, SEG_TOT_HEIGHT};
  SDL_RenderFillRect(renderer, &rect_seg7);
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0);

  // draw surrounding lines
  const int gap = 14;
  draw_surrounding_line(renderer, Rect(SEG_X, SEG_Y, SEG_TOT_WIDTH, SEG_TOT_HEIGHT), gap);

  // draw indices of each 7-seg display
  SDL_Point p = Point(SEG_X, SEG_Y) + Point(SEG_TOT_WIDTH, SEG_TOT_HEIGHT) + Point(0, gap)
                 - Point(SEG_TOT_WIDTH / 8 / 2, 0) - Point(CH_WIDTH / 2, CH_HEIGHT / 2);
  char buf[2] = "?";
  for (int i = 0; i < 8; i ++) {
    buf[0] = '0' + i;
    draw_str(renderer, buf, p.x, p.y, 0xffffff, BOARD_BG_COLOR);
    p = p - Point(SEG_TOT_WIDTH / 8, 0);
  }

  // draw label
  const char *str = "Seven Segment Display";
  p = Point(SEG_X, SEG_Y) - Point(0, gap) - Point(0, CH_HEIGHT / 2)
       + Point(SEG_TOT_WIDTH / 2, 0) - Point(CH_WIDTH * strlen(str) / 2, 0);
  draw_str(renderer, str, p.x, p.y, 0xffffff, BOARD_BG_COLOR);
}

void init_segs7(SDL_Renderer *renderer) {
  init_render_local(renderer);
  for (int i = 0; i < 8; ++i) {
    SDL_Rect mv = {SEG_X + SEG_SEP + (7 - i) * (SEG_HOR_WIDTH + SEG_DOT_WIDTH + SEG_VER_WIDTH * 2 + SEG_SEP * 2), SEG_Y + SEG_SEP, 0, 0};
    bool is_len8 = (pin_array[GET_SEGA(i)].vector_len == 8);
    Component *ptr = new SEGS7(renderer, 16, 0x5555, SEGS7_TYPE, is_len8);
    for (int j = 0; j < 8; ++j) {
      SDL_Rect *rect_ptr = new SDL_Rect;
      *rect_ptr = mv + segs_rect[j];
      ptr->set_texture(segs_texture(j, 0), j << 1 | 0);
      ptr->set_rect(rect_ptr, j << 1 | 0);
      rect_ptr = new SDL_Rect;
      *rect_ptr = mv + segs_rect[j];
      ptr->set_texture(segs_texture(j, 1), j << 1 | 1);
      ptr->set_rect(rect_ptr, j << 1 | 1);
    }

    for (int p = GET_SEGA(i); p <= GET_DECP(i); p ++) {
      ptr->add_pin(p);
    }
    add_component(ptr);
  }
}
