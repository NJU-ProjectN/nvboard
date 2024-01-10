#include <SDL.h>
#include <component.h>
#include <pins.h>

extern std::vector<Component *> components;
void uart_rx_getchar(uint8_t ch);
void uart_term_focus(bool v);
void kb_push_key(uint8_t scancode, bool is_keydown);
static bool uart_term_get_focus = false;

static void mousedown_handler(const SDL_Event &ev) {
  int x_pos = ev.button.x;
  int y_pos = ev.button.y;
  bool click_uart_term = false;
  for (auto i : components) {
    if (i->in_rect(x_pos, y_pos)) {
      switch (i->get_component_type()) {
        case BUTTON_TYPE: pin_poke(i->get_pin(), 1); break;
        case SWITCH_TYPE: pin_poke(i->get_pin(), i->get_state() ^ 1); break;
        case UART_TYPE: click_uart_term = true; break;
      }
    }
  }
  if (uart_term_get_focus ^ click_uart_term) {
    if (click_uart_term) SDL_StartTextInput();
    else SDL_StopTextInput();
    uart_term_get_focus = click_uart_term;
    uart_term_focus(uart_term_get_focus);
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
      if (uart_term_get_focus) {
        switch (ev.key.keysym.sym) {
          case SDLK_RETURN: uart_rx_getchar('\n'); break;
          case SDLK_BACKSPACE: uart_rx_getchar('\b'); break;
        }
      }
    case SDL_KEYUP:
      if (!uart_term_get_focus) kb_push_key(ev.key.keysym.scancode, ev.key.type == SDL_KEYDOWN);
      break;
    case SDL_TEXTINPUT: if (uart_term_get_focus) uart_rx_getchar(ev.text.text[0]); break;
  }
}
