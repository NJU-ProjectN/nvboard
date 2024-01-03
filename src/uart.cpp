#include <nvboard.h>
#include <uart.h>

UART* uart = NULL;

UART::UART(SDL_Renderer *rend, int cnt, int init_val, int ct, int x, int y, int w, int h):
    Component(rend, cnt, init_val, ct) {
  term = new Term(rend, x, y, w, h);
}

UART::~UART() {
  SDL_DestroyTexture(get_texture(0));
}

void UART::update_gui() {
  term->update_gui();
}

void UART::update_state() {
  static int i = 0;
  i ++;
  if (i < 10) return;
  i = 0;
  static uint8_t ch = ' ';
  ch += 1;
  if (ch == 128) ch = ' ';
  term->feed_ch(ch);
  update_gui();
}

void init_uart(SDL_Renderer *renderer) {
  int x = WINDOW_WIDTH / 2, y = 0, w = WINDOW_WIDTH / 2, h = WINDOW_HEIGHT / 2;
  uart = new UART(renderer, 1, 0, UART_TYPE, x, y, w, h);
  SDL_Rect *rect_ptr = new SDL_Rect;
  *rect_ptr = (SDL_Rect){x, y, w, h};
  uart->set_rect(rect_ptr, 0);
  uart->add_pin(UART_TX);
  uart->add_pin(UART_RX);
  add_component(uart);
}
