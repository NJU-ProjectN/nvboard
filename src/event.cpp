#include <nboard.h>
#include <SDL2/SDL.h>
#include <map>
#include <string>
#include <iostream>

extern std::map<std::string, bool> input_map, output_map;

static int keydown_handler(const SDL_Event &ev) {
  if (ev.key.keysym.sym == SDLK_ESCAPE) {
    return -1;
  } else {
    switch (ev.key.keysym.sym) {
    case SDLK_k:
      input_map["sw0"] ^= 1;
      return 1;
    case SDLK_j:
      input_map["sw1"] ^= 1;
      return 1;
    case SDLK_h:
      input_map["sw2"] ^= 1;
      return 1;
    case SDLK_g:
      input_map["sw3"] ^= 1;
      return 1;
    case SDLK_f:
      input_map["sw4"] ^= 1;
      return 1;
    case SDLK_d:
      input_map["sw5"] ^= 1;
      return 1;
    case SDLK_s:
      input_map["sw6"] ^= 1;
      return 1;
    case SDLK_a:
      input_map["sw7"] ^= 1;
      return 1;
    case SDLK_COMMA:
      input_map["sw8"] ^= 1;
      return 1;
    case SDLK_m:
      input_map["sw9"] ^= 1;
      return 1;
    case SDLK_n:
      input_map["sw10"] ^= 1;
      return 1;
    case SDLK_b:
      input_map["sw11"] ^= 1;
      return 1;
    case SDLK_v:
      input_map["sw12"] ^= 1;
      return 1;
    case SDLK_c:
      input_map["sw13"] ^= 1;
      return 1;
    case SDLK_x:
      input_map["sw14"] ^= 1;
      return 1;
    case SDLK_z:
      input_map["sw15"] ^= 1;
      return 1;
    case SDLK_l:
      input_map["btnl"] = 1;
      return 1;
    case SDLK_SEMICOLON:
      input_map["btnc"] = 1;
      return 1;
    case SDLK_QUOTE:
      input_map["btnr"] = 1;
      return 1;
    case SDLK_p:
      input_map["btnu"] = 1;
      return 1;
    case SDLK_PERIOD:
      input_map["btnd"] = 1;
      return 1;
    case SDLK_0:
      input_map["rst"] = 1;
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
    input_map["btnl"] = 0;
    return 1;
  case SDLK_SEMICOLON:
    input_map["btnc"] = 0;
    return 1;
  case SDLK_QUOTE:
    input_map["btnr"] = 0;
    return 1;
  case SDLK_p:
    input_map["btnu"] = 0;
    return 1;
  case SDLK_PERIOD:
    input_map["btnd"] = 0;
    return 1;
  case SDLK_0:
    input_map["rst"] = 0;
    return 1;
  default:
    return 0;
  }
  return 0;
}

static int mousedown_handler(const SDL_Event &ev) {
  // TODO: finish this function
  return 0;
}

static int mouseup_handler(const SDL_Event &ev) {
  // TODO: finish this function
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