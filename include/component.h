#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <SDL.h>
#include <vector>

// component type
enum {
  BUTTON_TYPE = 1, SWITCH_TYPE, NAIVE_LED_TYPE, RGB_LED_TYPE, SEGS7_TYPE,
  VGA_TYPE, KEYBOARD_TYPE, UART_TYPE
};

// logic type
enum {
  COMB_TYPE = 1, SEQ_TYPE = 2
};

class Component{
private:
  SDL_Renderer *m_renderer;
  int m_component_type;
  std::vector<SDL_Rect *> m_rects;
  std::vector<SDL_Texture *> m_textures;
  int m_state;
  std::vector<uint16_t> pins;

public:
  Component(SDL_Renderer *rend, int cnt, int init_val, int ct);

  bool in_rect(int x, int y) const;
  SDL_Renderer *get_renderer() const;
  int get_component_type() const;
  SDL_Rect *get_rect(int idx) const;
  SDL_Texture *get_texture(int idx) const;
  int get_state() const;
  uint16_t get_pin(int idx = 0) const;

  void set_rect(SDL_Rect *rect, int val);
  void set_texture(SDL_Texture *texture, int val);
  void set_state(int val);
  void add_pin(const uint16_t pin);
  virtual void update_gui();
  virtual void update_state();
  void remove();

  friend void delete_components();
};

#if 0
class RGB_LED : public Component{
public:
  RGB_LED(SDL_Renderer *rend, int cnt, int init_val, int ct);
  virtual void update_gui();
  virtual void update_state();
};
#endif

class SEGS7 : public Component{
private:
  bool is_len8;
public:
  SEGS7(SDL_Renderer *rend, int cnt, int init_val, int ct, bool is_len8);
  virtual void update_gui();
  virtual void update_state();
};

void init_components(SDL_Renderer *renderer);
void init_gui(SDL_Renderer *renderer);

void add_component(Component *c);
void update_components(SDL_Renderer *renderer);
void delete_components();

#endif
