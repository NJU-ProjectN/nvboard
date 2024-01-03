#include <nvboard.h>
#include <term.h>

SDL_Texture* get_font_texture(uint8_t ch);

Term::Term(SDL_Renderer *r, int x, int y, int w, int h):
    renderer(r), cursor_x(0), cursor_y(0), screen_y(0) {
  region = { .x = x, .y = y, .w = w, .h = h };
  w_in_char = region.w / 10;
  h_in_char = region.h / 16;
  uint8_t *l = add_line();
  cursor_texture = new_texture(r, 10, 16, 0x10, 0x10, 0x10);
}

Term::~Term() {
  SDL_DestroyTexture(cursor_texture);
}

void Term::clear_screen() {
  SDL_RenderFillRect(renderer, &region);
}

void Term::newline() {
  cursor_x = 0;
  cursor_y ++;
  if (cursor_y >= lines.size()) add_line();
  if (cursor_y == screen_y + h_in_char) screen_y ++;  // scroll one line
}

uint8_t* Term::add_line() {
  uint8_t *l = new uint8_t[w_in_char];
  memset(l, ' ', w_in_char);
  lines.push_back(l);
  return l;
}

void Term::feed_ch(uint8_t ch) {
  assert(ch < 128);
  int y = cursor_y;
  assert(y < lines.size());
  if (ch == '\n') {
    newline();
    return;
  }
  uint8_t *l = lines[y];
  l[cursor_x] = ch;
  cursor_x ++;
  if (cursor_x == w_in_char) newline();
}

void Term::draw_cursor() {
  if (cursor_y >= screen_y && cursor_y < screen_y + h_in_char) {
    int y = cursor_y - screen_y;
    int x = cursor_x;
    SDL_Rect rect = region;
    rect.w = 10, rect.h = 16;
    rect.y += 16 * y;
    rect.x += 10 * x;
    SDL_RenderCopy(renderer, cursor_texture, NULL, &rect);
  }
}

void Term::update_gui() {
  clear_screen();

  SDL_Rect rect = region;
  int x_start = rect.x;
  rect.w = 10, rect.h = 16;
  for (int y = 0; y < h_in_char; y ++) {
    if (screen_y + y >= lines.size()) break;
    uint8_t *l = lines[screen_y + y];
    for (int x = 0; x < w_in_char; x ++) {
      uint8_t ch = l[x];
      SDL_Texture *t  = get_font_texture(ch);
      SDL_RenderCopy(renderer, t, NULL, &rect);
      rect.x += 10;
    }
    rect.x = x_start;
    rect.y += 16;
  }
  draw_cursor();
  set_redraw();
}
