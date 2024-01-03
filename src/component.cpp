#include <nvboard.h>
#include <macro.h>

Component::Component(SDL_Renderer *rend, int cnt, int init_val, int ct) {
  m_renderer = rend;
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

uint16_t Component::get_pin(int idx) const{
  return pins[idx];
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

void Component::add_pin(const uint16_t pin) {
  pins.push_back(pin);
}

void Component::update_gui() {
  SDL_RenderCopy(m_renderer, m_textures[m_state], NULL, m_rects[m_state]);
  set_redraw();
}

void Component::update_state() {
  uint16_t pin = *(pins.begin());
  int newval = pin_peek(pin);
  if (newval != m_state) {
    set_state(newval);
    update_gui();
  }
}

void Component::remove() {
  for (auto rect_ptr : m_rects) { delete rect_ptr; }
}

#if 0
RGB_LED::RGB_LED(SDL_Renderer *rend, int cnt, int init_val, int ct)
  : Component(rend, cnt, init_val, ct){}

void RGB_LED::update_gui() {
  SDL_RenderCopy(get_renderer(), get_texture(get_state()), NULL, get_rect(get_state()));
  set_redraw();
}

void RGB_LED::update_state() {
  int newval = 0;
  for (int i = 0; i < 3; ++i) {
    newval = (newval << 1) | pin_peek(get_pin(i));
  }
  if (newval != get_state()) {
    set_state(newval);
    update_gui();
  }
}
#endif

void init_components(SDL_Renderer *renderer) {
#define COMPONENT_LIST(f) f(led) f(switch) f(button) f(segs7) f(keyboard) f(vga) f(uart)
#define INIT_FN(c) { void concat(init_, c)(SDL_Renderer *); concat(init_, c)(renderer); }
  COMPONENT_LIST(INIT_FN);
}

std::vector<Component *> components;

void add_component(Component *c) {
  components.push_back(c);
}

void delete_components() {
  for (auto comp_ptr : components) {
    comp_ptr->remove();
    delete comp_ptr;
  }
  components.clear();
}

// render buttons, switches, leds and 7-segs
void init_gui(SDL_Renderer *renderer) {
  for (auto ptr : components) { ptr->update_gui(); }
}

void update_components(SDL_Renderer *renderer) {
  for (auto ptr : components) { ptr->update_state(); }
}
