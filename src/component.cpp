#include <component.h>
#include <configs.h>
#include <render.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <iostream>
#include <map>

std::vector<Component *> components;

extern std::map<input_pin, int> input_map;
extern std::map<output_pin, int> output_map;

Component::Component(SDL_Renderer *rend, int cnt, int init_val, int it, int ct) {
  m_renderer = rend;
  m_interface_type = it;
  m_component_type = ct;
  m_rects.resize(cnt);
  m_textures.resize(cnt);
  m_state = init_val;
}

bool Component::in_rect(int x, int y) const{
  SDL_Rect *temp = m_rects[0];
  return x >= temp->x && y >= temp->y 
      && x < temp->x + temp->w 
      && y < temp->y + temp->h;
}

SDL_Renderer *Component::get_renderer() const{
  return m_renderer;
}

int Component::get_interface_type() const{
  return m_interface_type;
}

int Component::get_component_type() const{
  return m_component_type;
}

SDL_Rect *Component::get_rect(int idx) const{
  return m_rects[idx];
}

SDL_Texture *Component::get_texture(int idx) const{
  return m_textures[idx];
}

int Component::get_state() const{
  return m_state;
}

input_pin Component::get_input(int idx) const{
  return pins[idx].m_in;
}

output_pin Component::get_output(int idx) const{
  return pins[idx].m_out;
}

void Component::set_rect(SDL_Rect *rect, int val) {
  m_rects[val] = rect;
}

void Component::set_texture(SDL_Texture *texture, int val) {
  m_textures[val] = texture;
}

void Component::set_state(int val) {
  m_state = val;
}

void Component::add_input(const input_pin &in) {
  Pin temp;
  temp.m_in = in;
  pins.push_back(temp);
}

void Component::add_output(const output_pin &out) {
  Pin temp;
  temp.m_out = out;
  pins.push_back(temp);
}

void Component::update_gui(int newval) {
  SDL_RenderCopy(m_renderer, m_textures[m_state], NULL, m_rects[m_state]);
}

void Component::update_state() {
  Pin pin = *(pins.begin());
  int newval = (m_interface_type == INPUT_TYPE) ? input_map[pin.m_in] : output_map[pin.m_out];
  if (newval != m_state) {
    m_state = newval;
    update_gui(newval);
  }
}

RGB_LED::RGB_LED(SDL_Renderer *rend, int cnt, int init_val, int it, int ct)
  : Component(rend, cnt, init_val, it, ct){}

void RGB_LED::update_gui(int newval) {
  SDL_RenderCopy(get_renderer(), get_texture(newval), NULL, get_rect(newval));
}

void RGB_LED::update_state() {
  int newval = 0;
  for (int i = 0; i < 3; ++i) {
    newval = (newval << 1) | output_map[get_output(i)];
  }
  if (newval != get_state()) {
    update_gui(newval);
    set_state(newval);
  }
}

SEGS7::SEGS7(SDL_Renderer *rend, int cnt, int init_val, int it, int ct)
  : Component(rend, cnt, init_val, it, ct){}

void SEGS7::update_gui(int newval) {
  for (int i = 0; i < 16; ++i) {
    if ((newval >> i) & 1) {
      SDL_RenderCopy(get_renderer(), get_texture(i), NULL, get_rect(i));
    }
  }
}

void SEGS7::update_state() {
  int newval = 0;
  if (output_map[get_output(8)] == 0) {
    newval = 0x5555;
  } else {
    for (int i = 0; i < 8; ++i) {
      newval |= (output_map[get_output(i)]) ? (1 << (i << 1)) : (1 << (i << 1 | 1));
    }
  }
  if (newval != get_state()) {
    update_gui(newval);
    set_state(newval);
  }
}

extern SDL_Texture *tbutton_on, *tbutton_off;
extern SDL_Texture *tswitch_on, *tswitch_off;
extern SDL_Texture *tsegled_ver_off, *tsegled_ver_on,
                   *tsegled_hor_off, *tsegled_hor_on,
                   *tsegled_dot_off, *tsegled_dot_on;
extern SDL_Texture *tled_off, *tled_r, *tled_g, *tled_b,
                   *tled_rg, *tled_rb, *tled_gb, *tled_rgb;
extern SDL_Texture *tfpga_background, *tseg7_background;

extern SDL_Rect segs_rect[8], btn_rects[6];

SDL_Rect operator+(const SDL_Rect &A, const SDL_Rect &B) {
  SDL_Rect ret;
  ret.x = A.x + B.x;
  ret.y = A.y + B.y;
  ret.w = A.w + B.w;
  ret.h = A.h + B.h;
  return ret;
}

SDL_Texture *segs_texture(int index, int val);

void init_components(SDL_Renderer *renderer) {
  Component *ptr = nullptr;
  SDL_Rect *rect_ptr = nullptr;

  // init buttons
  for (int i = 0; i < 6; ++i) {
    ptr = new Component(renderer, 2, 0, INPUT_TYPE, BUTTON_TYPE);
    
    // off
    rect_ptr = new SDL_Rect;
    *rect_ptr = btn_rects[i];
    ptr->set_rect(rect_ptr, 0);
    ptr->set_texture(tbutton_off, 0);
    
    // on
    rect_ptr = new SDL_Rect;
    *rect_ptr = btn_rects[i];
    ptr->set_rect(rect_ptr, 1);
    ptr->set_texture(tbutton_on, 1);
    
    ptr->add_input(input_pin(int(input_pin::BTNC) + i));
    components.push_back(ptr);
  }

  // init switches
  for (int i = 0; i < 16; ++i) {
    ptr = new Component(renderer, 2, 0, INPUT_TYPE, SWICTH_TYPE);
    
    // off
    rect_ptr = new SDL_Rect;
    *rect_ptr = (SDL_Rect){SWITCH_X + (15 - i) * (SWITCH_WIDTH + SWITCH_SEP), SWITCH_Y, SWITCH_WIDTH, SWITCH_HEIGHT};
    ptr->set_rect(rect_ptr, 0);
    ptr->set_texture(tswitch_off, 0);
    
    // on
    rect_ptr = new SDL_Rect;
    *rect_ptr = (SDL_Rect){SWITCH_X + (15 - i) * (SWITCH_WIDTH + SWITCH_SEP), SWITCH_Y, SWITCH_WIDTH, SWITCH_HEIGHT};
    ptr->set_rect(rect_ptr, 1);
    ptr->set_texture(tswitch_on, 1);

    ptr->add_input(input_pin(int(input_pin::SW0) + i));
    components.push_back(ptr);
  }
  
  // init naive leds
  for (int i = 0; i < 16; ++i) {
    ptr = new Component(renderer, 2, 0, OUTPUT_TYPE, NAIVE_LED_TYPE);

    // off
    rect_ptr = new SDL_Rect;
    *rect_ptr = (SDL_Rect){LED_X + (15 - i) * (LED_WIDTH + LED_SEP), LED_Y, LED_WIDTH, LED_HEIGHT};
    ptr->set_rect(rect_ptr, 0);
    ptr->set_texture(tled_off, 0);

    // on
    rect_ptr = new SDL_Rect;
    *rect_ptr = (SDL_Rect){LED_X + (15 - i) * (LED_WIDTH + LED_SEP), LED_Y, LED_WIDTH, LED_HEIGHT};
    ptr->set_rect(rect_ptr, 1);
    ptr->set_texture(tled_g, 1);

    ptr->add_output(output_pin(int(output_pin::LD0) + i));
    components.push_back(ptr);
  }

  // init 7-segment display
  for (int i = 0; i < 8; ++i) {
    SDL_Rect mv = {(7 - i) * (SEG_HOR_WIDTH + SEG_DOT_WIDTH + SEG_SEP * 4), 0, 0, 0};
    ptr = new SEGS7(renderer, 16, 0x5555, OUTPUT_TYPE, SEGS7_TYPE);
    for (int j = 0; j < 8; ++j) {
      rect_ptr = new SDL_Rect;
      *rect_ptr = mv + segs_rect[j];
      ptr->set_texture(segs_texture(j, 0), j << 1 | 0);
      ptr->set_rect(rect_ptr, j << 1 | 0);
      rect_ptr = new SDL_Rect;
      *rect_ptr = mv + segs_rect[j];
      ptr->set_texture(segs_texture(j, 1), j << 1 | 1);
      ptr->set_rect(rect_ptr, j << 1 | 1);
    }

    for (output_pin p = output_pin::SEGA; p <= output_pin::DECP; p = output_pin(int(p) + 1)) {
      ptr->add_output(p);
    }
    ptr->add_output(output_pin(int(output_pin::AN0) + i));
    components.push_back(ptr);
  }
}

void delete_components() {
  for (auto comp_ptr : components) {
    for (auto rect_ptr : comp_ptr->m_rects) {
      delete rect_ptr;
    }
    delete comp_ptr;
  }
  components.clear();
}