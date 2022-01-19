#include <nboard.h>
#include <SDL2/SDL.h>
#include <map>
#include <string>
#include <cstdio>
#include <string>
#include <vector>
#include <cassert>

extern std::vector<Component *> components;
extern std::map<input_pin, bool> input_map;
extern std::map<output_pin, bool> output_map;

static int keydown_handler(const SDL_Event &ev) {
  if (ev.key.keysym.sym == SDLK_ESCAPE) {
    return -1;
  } else {
    switch (ev.key.keysym.sym) {
    case SDLK_k:
      input_map[input_pin::SW0] ^= 1;
      return 1;
    case SDLK_j:
      input_map[input_pin::SW1] ^= 1;
      return 1;
    case SDLK_h:
      input_map[input_pin::SW2] ^= 1;
      return 1;
    case SDLK_g:
      input_map[input_pin::SW3] ^= 1;
      return 1;
    case SDLK_f:
      input_map[input_pin::SW4] ^= 1;
      return 1;
    case SDLK_d:
      input_map[input_pin::SW5] ^= 1;
      return 1;
    case SDLK_s:
      input_map[input_pin::SW6] ^= 1;
      return 1;
    case SDLK_a:
      input_map[input_pin::SW7] ^= 1;
      return 1;
    case SDLK_COMMA:
      input_map[input_pin::SW8] ^= 1;
      return 1;
    case SDLK_m:
      input_map[input_pin::SW9] ^= 1;
      return 1;
    case SDLK_n:
      input_map[input_pin::SW10] ^= 1;
      return 1;
    case SDLK_b:
      input_map[input_pin::SW11] ^= 1;
      return 1;
    case SDLK_v:
      input_map[input_pin::SW12] ^= 1;
      return 1;
    case SDLK_c:
      input_map[input_pin::SW13] ^= 1;
      return 1;
    case SDLK_x:
      input_map[input_pin::SW14] ^= 1;
      return 1;
    case SDLK_z:
      input_map[input_pin::SW15] ^= 1;
      return 1;
    case SDLK_l:
      input_map[input_pin::BTNL] = 1;
      return 1;
    case SDLK_SEMICOLON:
      input_map[input_pin::BTNC] = 1;
      return 1;
    case SDLK_QUOTE:
      input_map[input_pin::BTNR] = 1;
      return 1;
    case SDLK_p:
      input_map[input_pin::BTNU] = 1;
      return 1;
    case SDLK_PERIOD:
      input_map[input_pin::BTND] = 1;
      return 1;
    case SDLK_0:
      input_map[input_pin::RST] = 1;
      return 1;
    default:
      return 0;
    }
  }
  return 0;
}

static int keyup_handler(const SDL_Event &ev) {
  switch (ev.key.keysym.sym) {
  case SDLK_l:
    input_map[input_pin::BTNL] = 0;
    return 1;
  case SDLK_SEMICOLON:
    input_map[input_pin::BTNC] = 0;
    return 1;
  case SDLK_QUOTE:
    input_map[input_pin::BTNR] = 0;
    return 1;
  case SDLK_p:
    input_map[input_pin::BTNU] = 0;
    return 1;
  case SDLK_PERIOD:
    input_map[input_pin::BTND] = 0;
    return 1;
  case SDLK_0:
    input_map[input_pin::RST] = 0;
    return 1;
  default:
    return 0;
  }
  return 0;
}

extern std::string input_btns[];
extern std::string input_switches[];

static int mousedown_handler(const SDL_Event &ev) {
  int x_pos = ev.button.x;
  int y_pos = ev.button.y;
  for (auto i : components) {
    if (i->in_rect(x_pos, y_pos) && (i->get_interface_type() == INPUT_TYPE)) {
      switch (i->get_component_type()) {
      case BUTTON_TYPE:
        input_map[i->get_input()] = 1;
        return 1;
        break;
      case SWICTH_TYPE:
        input_map[i->get_input()] = i->get_state() ^ 1;
        return 1;
        break;
      }
    }
  }
  return 0;
}

static int mouseup_handler(const SDL_Event &ev) {
  int x_pos = ev.button.x;
  int y_pos = ev.button.y;
  for (auto i : components) {
    if (i->in_rect(x_pos, y_pos) && i->get_interface_type() == INPUT_TYPE) {
      switch (i->get_component_type())
      {
      case BUTTON_TYPE:
        input_map[i->get_input()] = 0;
        return 1;
        break;
      }
    }
  }
  return 0;
}

// Return -1 when esc is pressed.
// Else return whether buttons / switches are pressed.
int read_event() {
  SDL_Event ev;
  SDL_PollEvent(&ev);
  switch (ev.type) {
  case SDL_QUIT:
    return -1;
    break;
  case SDL_KEYDOWN:
    return keydown_handler(ev);
    break;
  case SDL_KEYUP:
    return keyup_handler(ev);
    break;
  case SDL_MOUSEBUTTONDOWN:
    return mousedown_handler(ev);
    break;
  case SDL_MOUSEBUTTONUP:
    return mouseup_handler(ev);
    break;
  default:
    return 0;
    break;
  }
  return 0;
}

void dbg_wait_esc() {
  while (1) {
    if (read_event() == -1) {
      break;
    }
  }
}