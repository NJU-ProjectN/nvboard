#include <SDL2/SDL.h>
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <component.h>

extern std::vector<Component *> components;
extern std::map<input_pin, int> input_map;

static void mousedown_handler(const SDL_Event &ev) {
  int x_pos = ev.button.x;
  int y_pos = ev.button.y;
  for (auto i : components) {
    if (i->in_rect(x_pos, y_pos) && (i->get_interface_type() == INPUT_TYPE)) {
      switch (i->get_component_type()) {
        case BUTTON_TYPE: input_map[i->get_input()] = 1; break;
        case SWICTH_TYPE: input_map[i->get_input()] = i->get_state() ^ 1; break;
      }
    }
  }
}

static void mouseup_handler(const SDL_Event &ev) {
  int x_pos = ev.button.x;
  int y_pos = ev.button.y;
  for (auto i : components) {
    if (i->in_rect(x_pos, y_pos) && i->get_interface_type() == INPUT_TYPE) {
      switch (i->get_component_type()) {
        case BUTTON_TYPE: input_map[i->get_input()] = 0; break;
      }
    }
  }
}

// Return -1 when esc is pressed.
// Else return whether buttons / switches are pressed.
int read_event() {
  SDL_Event ev;
  SDL_PollEvent(&ev);
  switch (ev.type) {
    case SDL_QUIT: return -1;
    case SDL_WINDOWEVENT:
      if(ev.window.event == SDL_WINDOWEVENT_CLOSE) { return -1; }
      break;
    case SDL_MOUSEBUTTONDOWN: mousedown_handler(ev); break;
    case SDL_MOUSEBUTTONUP: mouseup_handler(ev); break;
  }
  return 0;
}
