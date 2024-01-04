#include <nvboard.h>
#include <uart.h>

UART* uart = NULL;
bool is_uart_idle = true;
uint8_t *uart_tx_ptr = NULL;

UART::UART(SDL_Renderer *rend, int cnt, int init_val, int ct, int x, int y, int w, int h):
    Component(rend, cnt, init_val, ct),
    state(0), divisor(16), need_update_gui(false) {
  term = new Term(rend, x, y, w, h);
  divisor_cnt = divisor - 1;
  assert(pin_array[UART_TX].vector_len == 1);
  uart_tx_ptr = (uint8_t *)pin_array[UART_TX].ptr;
}

UART::~UART() {
  SDL_DestroyTexture(get_texture(0));
}

void UART::update_gui() {
  term->update_gui();
}

void UART::check_tx() {
  if (divisor_cnt > 0) { divisor_cnt --; return; }
  divisor_cnt = divisor - 1;

  uint8_t tx = *uart_tx_ptr;
  if (state == 0) { // idle
    if (!tx) { // start bit
      data = 0;
      is_uart_idle = false;
      state ++;
    }
  } else if (state >= 1 && state <= 8) { // data
    data = (tx << 7) | (data >> 1);  // data bit
    state ++;
  } else if (state == 9) {
    if (tx) {
      term->feed_ch(data);
      state = 0;
      is_uart_idle = true;
      need_update_gui = true;
    } // stop bit
  }
}

void UART::update_state() {
  if (need_update_gui) {
    need_update_gui = false;
    update_gui();
  }
}

void UART::set_divisor(uint16_t d) {
  divisor = d;
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
