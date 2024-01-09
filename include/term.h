#ifndef _NVBOARD_TERM_H
#define _NVBOARD_TERM_H

class Term {
private:
  SDL_Renderer *renderer;
  SDL_Rect region;
  int w_in_char, h_in_char;
  std::vector<uint8_t *> lines;
  int cursor_x, cursor_y; // cursor_y start with all history when scrolling is supported
  bool is_cursor_visible;
  SDL_Texture *cursor_texture;
  int screen_y;
  bool dirty_screen;
  bool *dirty_line;
  bool *dirty_char;

  void clear_screen();
  void newline();
  uint8_t *add_line();
  void draw_cursor();
  bool is_cursor_on_screen();
  void set_dirty_char(int y, int x);
  void init_dirty(bool val);

public:
  Term(SDL_Renderer *r, int x, int y, int w, int h);
  ~Term();
  void feed_ch(uint8_t ch);
  void backspace(bool is_input);
  void feed_str(const char *s);
  void clear();
  void set_cursor_visibility(bool v);
  void update_gui();
};

#endif

