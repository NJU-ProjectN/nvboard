#include <nvboard.h>
#include <term.h>

Term::Term(SDL_Renderer *r, int x, int y, int w, int h):
    renderer(r), cursor_x(0), cursor_y(0), screen_y(0) {
  region = { .x = x, .y = y, .w = w, .h = h };
  w_in_char = region.w / CH_WIDTH;
  h_in_char = region.h / CH_HEIGHT;
  uint8_t *l = add_line();
  cursor_texture = new_texture(r, CH_WIDTH, CH_HEIGHT, 0x10, 0x10, 0x10);
  get_focus_cursor_texture = new_texture(r, CH_WIDTH, CH_HEIGHT, 0xff, 0x00, 0xff);
  is_cursor_visible = true;
  is_focus = false;
  clear_screen();
  draw_cursor();
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

void Term::set_cursor_visibility(bool v) {
  bool update_cursor_gui = is_cursor_visible ^ v;
  is_cursor_visible = v;
  if (update_cursor_gui) draw_cursor();
}

void Term::set_focus(bool v) {
  bool update_cursor_gui = is_focus ^ v;
  is_focus = v;
  if (update_cursor_gui) draw_cursor();
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
  if (cursor_y == screen_y + h_in_char) { // scroll down one line
    screen_y ++;  // TODO: only set dirty if screen_y changes between two draws
    init_dirty(true);
  }
}

void Term::_return() {
  cursor_x = 0;
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

void Term::backspace(bool is_input) {
  if (cursor_x == 0) {
    if (!is_input) return;
    if (cursor_y == 0) return;
    delete [] lines[cursor_y];
    lines.erase(lines.begin() + cursor_y);
    cursor_y --;
    cursor_x = w_in_char - 1;
    if (cursor_y < screen_y) { // scroll up one line
      screen_y --;
      init_dirty(true);
    }
  } else {
    if (is_cursor_on_screen()) set_dirty_char(cursor_y - screen_y, cursor_x);
    cursor_x --;
  }
  lines[cursor_y][cursor_x] = ' ';
  if (is_cursor_on_screen()) set_dirty_char(cursor_y - screen_y, cursor_x);
}

void Term::feed_ch(uint8_t ch) {
  assert(ch < 128);
  if (is_cursor_on_screen()) set_dirty_char(cursor_y - screen_y, cursor_x);
  int y = cursor_y;
  assert(y < lines.size());
  if      (ch == '\n') { newline(); return; }
  else if (ch == '\r') { _return(); return; }
  else if (ch == '\b') { backspace(false); return; }
  lines[y][cursor_x] = ch;
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
    rect.w = CH_WIDTH, rect.h = CH_HEIGHT;
    rect.y += CH_HEIGHT * y;
    rect.x += CH_WIDTH * x;
    SDL_Texture *t = is_cursor_visible ? (is_focus ? get_focus_cursor_texture :
                                          cursor_texture) :  ch2texture_term(' ');
    SDL_RenderCopy(renderer, t, NULL, &rect);
    set_redraw();
  }
}

void Term::update_gui() {
  if (!dirty_screen) return;
  SDL_Rect rect = region;
  int x_start = rect.x;
  rect.w = CH_WIDTH, rect.h = CH_HEIGHT;
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
      SDL_Texture *t  = ch2texture_term(ch);
      SDL_RenderCopy(renderer, t, NULL, &rect);
      set_redraw();
    }
  }
  draw_cursor();
  init_dirty(false);
}
