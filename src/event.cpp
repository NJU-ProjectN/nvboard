#include <nboard.h>
#include <SDL2/SDL.h>
#include <map>
#include <string>
#include <iostream>
#include <string>

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

extern std::string input_btns[];
extern std::string input_switches[];

static int mousedown_handler(const SDL_Event &ev) {
  int x_pos = ev.button.x;
  int y_pos = ev.button.y;
  if (y_pos >= SWITCH_Y && y_pos < SWITCH_Y + SWITCH_HEIGHT && x_pos >= SWITCH_X && x_pos < SWITCH_X + 16 * SWITCH_WIDTH + 15 * SWITCH_SEP) {
    if ((x_pos - SWITCH_X) % (SWITCH_WIDTH + SWITCH_SEP) < SWITCH_WIDTH) {
      int index = 15 - (x_pos - SWITCH_X) / (SWITCH_WIDTH + SWITCH_SEP);
      input_map[input_switches[index]] ^= 1;
      return 1;
    } else {
      return 0;
    }
  } else if (y_pos >= BTNC_Y && y_pos < BTNC_Y + BTNC_HEIGHT && x_pos >= BTNC_X && x_pos < BTNC_X + BTNC_WIDTH) {
    input_map["btnc"] = 1;
    return 1;
  } else if (y_pos >= BTNC_Y - BTNC_HEIGHT - BTNC_SEP && y_pos < BTNC_Y - BTNC_SEP && x_pos >= BTNC_X && x_pos < BTNC_X + BTNC_WIDTH) {
    input_map["btnu"] = 1;
    return 1;
  } else if (y_pos >= BTNC_Y + BTNC_HEIGHT + BTNC_SEP && y_pos < BTNC_Y + 2 * BTNC_HEIGHT + BTNC_SEP && x_pos >= BTNC_X && x_pos < BTNC_X + BTNC_WIDTH) {
    input_map["btnd"] = 1;
    return 1;
  } else if (y_pos >= BTNC_Y && y_pos < BTNC_Y + BTNC_HEIGHT && x_pos >= BTNC_X - BTNC_WIDTH - BTNC_SEP && x_pos < BTNC_X - BTNC_SEP) {
    input_map["btnl"] = 1;
    return 1;
  } else if (y_pos >= BTNC_Y && y_pos < BTNC_Y + BTNC_HEIGHT && x_pos >= BTNC_X + BTNC_WIDTH + BTNC_SEP && x_pos < BTNC_X + 2 * BTNC_WIDTH + BTNC_SEP) {
    input_map["btnr"] = 1;
    return 1;
  } else if (y_pos >= BTNC_Y - 2 * BTNC_HEIGHT - 2 * BTNC_SEP && y_pos < BTNC_Y - BTNC_HEIGHT - 2 * BTNC_SEP && x_pos >= BTNC_X && x_pos < BTNC_X + BTNC_WIDTH) {
    input_map["rst"] = 1;
    return 1;
  }
  return 0;
}

static int mouseup_handler(const SDL_Event &ev) {
  int x_pos = ev.button.x;
  int y_pos = ev.button.y;
  if (y_pos >= BTNC_Y && y_pos < BTNC_Y + BTNC_HEIGHT && x_pos >= BTNC_X && x_pos < BTNC_X + BTNC_WIDTH) {
    input_map["btnc"] = 0;
    return 1;
  } else if (y_pos >= BTNC_Y - BTNC_HEIGHT - BTNC_SEP && y_pos < BTNC_Y - BTNC_SEP && x_pos >= BTNC_X && x_pos < BTNC_X + BTNC_WIDTH) {
    input_map["btnu"] = 0;
    return 1;
  } else if (y_pos >= BTNC_Y + BTNC_HEIGHT + BTNC_SEP && y_pos < BTNC_Y + 2 * BTNC_HEIGHT + BTNC_SEP && x_pos >= BTNC_X && x_pos < BTNC_X + BTNC_WIDTH) {
    input_map["btnd"] = 0;
    return 1;
  } else if (y_pos >= BTNC_Y && y_pos < BTNC_Y + BTNC_HEIGHT && x_pos >= BTNC_X - BTNC_WIDTH - BTNC_SEP && x_pos < BTNC_X - BTNC_SEP) {
    input_map["btnl"] = 0;
    return 1;
  } else if (y_pos >= BTNC_Y && y_pos < BTNC_Y + BTNC_HEIGHT && x_pos >= BTNC_X + BTNC_WIDTH + BTNC_SEP && x_pos < BTNC_X + 2 * BTNC_WIDTH + BTNC_SEP) {
    input_map["btnr"] = 0;
    return 1;
  } else if (y_pos >= BTNC_Y - 2 * BTNC_HEIGHT - 2 * BTNC_SEP && y_pos < BTNC_Y - BTNC_HEIGHT - 2 * BTNC_SEP && x_pos >= BTNC_X && x_pos < BTNC_X + BTNC_WIDTH) {
    input_map["rst"] = 0;
    return 1;
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
  case SDL_WINDOWEVENT:
    if(ev.window.event == SDL_WINDOWEVENT_CLOSE){
      return -1;
    }
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