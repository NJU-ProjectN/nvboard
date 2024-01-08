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
  clear_screen();
  dirty_line = new bool[h_in_char];
  dirty_char = new bool[w_in_char * h_in_char];
  init_dirty(false);
}

void Term::init_dirty(bool val) {
  dirty_screen = val;
  memset(dirty_line, val, h_in_char);
  memset(dirty_char, val, w_in_char * h_in_char);
}

Term::~Term() {
  SDL_DestroyTexture(cursor_texture);
}

void Term::clear_screen() {
  SDL_RenderFillRect(renderer, &region);
}

void Term::clear() {
  while (lines.size() > 1) {
    delete [] lines.back();
    lines.pop_back();
  }
  memset(lines[0], ' ', w_in_char);
  cursor_x = cursor_y = screen_y = 0;
  clear_screen();
  init_dirty(false);
  set_redraw();
}

void Term::newline() {
  cursor_x = 0;
  cursor_y ++;
  if (cursor_y >= lines.size()) add_line();
  if (cursor_y == screen_y + h_in_char) { // scroll one line
    screen_y ++;  // TODO: only set dirty if screen_y changes between two draws
    init_dirty(true);
  }
}

uint8_t* Term::add_line() {
  uint8_t *l = new uint8_t[w_in_char];
  memset(l, ' ', w_in_char);
  lines.push_back(l);
  return l;
}

void Term::set_dirty_char(int y, int x) {
  dirty_char[y * w_in_char + x] = true;
  dirty_line[y] = true;
  dirty_screen = true;
}

void Term::feed_ch(uint8_t ch) {
  assert(ch < 128);
  if (is_cursor_on_screen()) set_dirty_char(cursor_y - screen_y, cursor_x);
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

void Term::feed_str(const char *s) {
  while (*s != '\0') feed_ch(*(s ++));
}

bool Term::is_cursor_on_screen() {
  return cursor_y >= screen_y && cursor_y < screen_y + h_in_char;
}

void Term::draw_cursor() {
  if (is_cursor_on_screen()) {
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
  if (!dirty_screen) return;
  SDL_Rect rect = region;
  int x_start = rect.x;
  rect.w = 10, rect.h = 16;
  for (int y = 0; y < h_in_char; y ++) {
    if (screen_y + y >= lines.size()) break;
    if (!dirty_line[y]) continue;

    uint8_t *l = lines[screen_y + y];
    rect.y = region.y + rect.h * y;
    bool *dirty = &dirty_char[y * w_in_char];
    for (int x = 0; x < w_in_char; x ++) {
      if (!dirty[x]) continue;

      uint8_t ch = l[x];
      rect.x = region.x + rect.w * x;
      SDL_Texture *t  = get_font_texture(ch);
      SDL_RenderCopy(renderer, t, NULL, &rect);
    }
  }
  draw_cursor();
  set_redraw();
  init_dirty(false);
}
