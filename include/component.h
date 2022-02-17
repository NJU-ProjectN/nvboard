#ifndef _VFPGA_COMPONENT_H
#define _VFPGA_COMPONENT_H

#include <SDL2/SDL.h>
#include <vector>
#include <constrs.h>

// interface type
enum {
  INPUT_TYPE = 1, OUTPUT_TYPE = 2
};

// component type
enum {
  BUTTON_TYPE = 1, SWICTH_TYPE, NAIVE_LED_TYPE, RGB_LED_TYPE, SEGS7_TYPE, VGA_TYPE, KEYBOARD_TYPE
};

// logic type
enum {
  COMB_TYPE = 1, SEQ_TYPE = 2
};

union Pin{
  input_pin m_in;
  output_pin m_out;
};

class Component{
private:
  SDL_Renderer *m_renderer;
  int m_interface_type;
  int m_component_type;
  std::vector<SDL_Rect *> m_rects;
  std::vector<SDL_Texture *> m_textures;
  int m_state;
  std::vector<Pin> pins;

public:
  Component(SDL_Renderer *rend, int cnt, int init_val, int it, int ct);
  
  bool in_rect(int x, int y) const;
  SDL_Renderer *get_renderer() const;
  int get_interface_type() const;
  int get_component_type() const;
  SDL_Rect *get_rect(int idx) const;
  SDL_Texture *get_texture(int idx) const;
  int get_state() const;
  input_pin get_input(int idx = 0) const;
  output_pin get_output(int idx = 0) const;

  void set_rect(SDL_Rect *rect, int val);
  void set_texture(SDL_Texture *texture, int val);
  void set_state(int val);
  void add_input(const input_pin &in);
  void add_output(const output_pin &out);
  virtual void update_gui();
  virtual void update_state();

  friend void delete_components();
};

class RGB_LED : public Component{
public:
  RGB_LED(SDL_Renderer *rend, int cnt, int init_val, int it, int ct);
  virtual void update_gui();
  virtual void update_state();
};

class SEGS7 : public Component{
public:
  SEGS7(SDL_Renderer *rend, int cnt, int init_val, int it, int ct);
  virtual void update_gui();
  virtual void update_state();
};

void init_components(SDL_Renderer *renderer);

void delete_components();

#endif
