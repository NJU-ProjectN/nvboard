#ifndef __TERM_H__
#define __TERM_H__

class Term {
private:
  SDL_Renderer *renderer;
  SDL_Rect region;
  int w_in_char, h_in_char;
  std::vector<uint8_t *> lines;
  int cursor_x, cursor_y; // cursor_y start with all history when scrolling is supported
  bool is_cursor_visible;
  bool is_focus;
  SDL_Texture *cursor_texture;
  SDL_Texture *get_focus_cursor_texture;
  int screen_y;
  bool dirty_screen;
  bool *dirty_line;
  bool *dirty_char;

  void clear_screen();
  void newline();
  void _return();
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
  void set_focus(bool v);
  void update_gui();
};

#endif

