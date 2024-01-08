#include <SDL.h>
#include <component.h>
#include <pins.h>

extern std::vector<Component *> components;
void uart_rx_getchar(uint8_t ch);
void uart_rx_term_focus(bool v);
void kb_push_key(uint8_t scancode, bool is_keydown);
static bool focus_uart_rx_term = false;

static void mousedown_handler(const SDL_Event &ev) {
  int x_pos = ev.button.x;
  int y_pos = ev.button.y;
  bool click_uart_rx_term = false;
  for (auto i : components) {
    if (i->in_rect(x_pos, y_pos)) {
      switch (i->get_component_type()) {
        case BUTTON_TYPE: pin_poke(i->get_pin(), 1); break;
        case SWITCH_TYPE: pin_poke(i->get_pin(), i->get_state() ^ 1); break;
        case UART_TYPE: click_uart_rx_term = true; break;
      }
    }
  }
  if (focus_uart_rx_term ^ click_uart_rx_term) {
    if (click_uart_rx_term) SDL_StartTextInput();
    else SDL_StopTextInput();
    focus_uart_rx_term = click_uart_rx_term;
    uart_rx_term_focus(focus_uart_rx_term);
  }
}

static void mouseup_handler(const SDL_Event &ev) {
  int x_pos = ev.button.x;
  int y_pos = ev.button.y;
  for (auto i : components) {
    if (i->in_rect(x_pos, y_pos)) {
      switch (i->get_component_type()) {
        case BUTTON_TYPE: pin_poke(i->get_pin(), 0); break;
      }
    }
  }
}

void read_event() {
  SDL_Event ev;
  SDL_PollEvent(&ev);
  switch (ev.type) {
    case SDL_QUIT: exit(0);
    case SDL_WINDOWEVENT:
      if (ev.window.event == SDL_WINDOWEVENT_CLOSE) { exit(0); }
      break;
    case SDL_MOUSEBUTTONDOWN: mousedown_handler(ev); break;
    case SDL_MOUSEBUTTONUP: mouseup_handler(ev); break;
    case SDL_KEYDOWN:
      if (focus_uart_rx_term) {
        switch (ev.key.keysym.sym) {
          case SDLK_RETURN: uart_rx_getchar('\n'); break;
          case SDLK_BACKSPACE: uart_rx_getchar('\b'); break;
        }
      }
    case SDL_KEYUP:
      if (!focus_uart_rx_term) kb_push_key(ev.key.keysym.scancode, ev.key.type == SDL_KEYDOWN);
      break;
    case SDL_TEXTINPUT: if (focus_uart_rx_term) uart_rx_getchar(ev.text.text[0]); break;
  }
}
