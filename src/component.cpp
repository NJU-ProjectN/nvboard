#include <nvboard.h>
#include <vector>
#include <iostream>
#include <map>
#include <SDL2/SDL.h>

extern uint64_t input_map [];
extern uint64_t output_map [];

std::vector<Component *> components;
std::vector<Component *> rt_components; // real-time components

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

uint16_t Component::get_input(int idx) const{
  return pins[idx].m_in;
}

uint16_t Component::get_output(int idx) const{
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

void Component::add_input(const uint16_t in) {
  Pin temp;
  temp.m_in = in;
  pins.push_back(temp);
}

void Component::add_output(const uint16_t out) {
  Pin temp;
  temp.m_out = out;
  pins.push_back(temp);
}

void Component::update_gui() {
  SDL_RenderCopy(m_renderer, m_textures[m_state], NULL, m_rects[m_state]);
}

void Component::update_state() {
  Pin pin = *(pins.begin());
  int newval = (m_interface_type == INPUT_TYPE) ? input_map[pin.m_in] : output_map[pin.m_out];
  if (newval != m_state) {
    set_state(newval);
    update_gui();
  }
}

void Component::remove() {
  for (auto rect_ptr : m_rects) { delete rect_ptr; }
}

RGB_LED::RGB_LED(SDL_Renderer *rend, int cnt, int init_val, int it, int ct)
  : Component(rend, cnt, init_val, it, ct){}

void RGB_LED::update_gui() {
  SDL_RenderCopy(get_renderer(), get_texture(get_state()), NULL, get_rect(get_state()));
}

void RGB_LED::update_state() {
  int newval = 0;
  for (int i = 0; i < 3; ++i) {
    newval = (newval << 1) | output_map[get_output(i)];
  }
  if (newval != get_state()) {
    set_state(newval);
    update_gui();
  }
}

SEGS7::SEGS7(SDL_Renderer *rend, int cnt, int init_val, int it, int ct)
  : Component(rend, cnt, init_val, it, ct){}

void SEGS7::update_gui() {
  int newval = get_state();
  for (int i = 0; i < 16; ++i) {
    if ((newval >> i) & 1) {
      SDL_RenderCopy(get_renderer(), get_texture(i), NULL, get_rect(i));
    }
  }
}

void SEGS7::update_state() {
  int newval = 0;
  for (int i = 0; i < 8; ++i) {
    newval |= (output_map[get_output(i)]) ? (1 << (i << 1)) : (1 << (i << 1 | 1));
  }
  if (newval != get_state()) {
    set_state(newval);
    update_gui();
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
    
    ptr->add_input(BTNC + i);
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

    ptr->add_input(SW0 + i);
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

    ptr->add_output(LD0 + i);
    components.push_back(ptr);
  }

  // init 7-segment display
  for (int i = 0; i < 8; ++i) {
    SDL_Rect mv = {SEG_X + SEG_SEP + (7 - i) * (SEG_HOR_WIDTH + SEG_DOT_WIDTH + SEG_VER_WIDTH * 2 + SEG_SEP * 2), SEG_Y + SEG_SEP, 0, 0};
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

    for (int p = GET_SEGA(i); p <= GET_DECP(i); p ++) {
      ptr->add_output(p);
    }
    components.push_back(ptr);
  }

#ifdef VGA_ENA
  // init vga
  ptr = new VGA(renderer, 1, 0, OUTPUT_TYPE, VGA_TYPE);
  rect_ptr = new SDL_Rect;
  *rect_ptr = (SDL_Rect){WINDOW_WIDTH, 0, VGA_DEFAULT_WIDTH, VGA_DEFAULT_HEIGHT};
  ptr->set_rect(rect_ptr, 0);
  for (int p = VGA_VSYNC; p <= VGA_B7; p ++) {
    ptr->add_output(p);
  }
  rt_components.push_back(ptr);
#endif

  // init keyboard
  extern KEYBOARD* kb;
  kb = new KEYBOARD(renderer, 0, 0, INPUT_TYPE, KEYBOARD_TYPE);
  for (int p = PS2_CLK; p <= PS2_DAT; p ++) {
    kb->add_input(p);
  }
  rt_components.push_back(kb);
}

static void delete_components(std::vector<Component *> *c) {
  for (auto comp_ptr : *c) {
    comp_ptr->remove();
    delete comp_ptr;
  }
  c->clear();
}

void delete_components() {
  delete_components(&components);
  delete_components(&rt_components);
}

// render buttons, switches, leds and 7-segs
void init_gui(SDL_Renderer *renderer) {
  for (auto ptr : components) { ptr->update_gui(); }
}

void update_components(SDL_Renderer *renderer) {
  for (auto ptr : components) { ptr->update_state(); }
}

void update_rt_components(SDL_Renderer *renderer) {
  for (auto ptr : rt_components) { ptr->update_state(); }
}
