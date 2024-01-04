#include <nvboard.h>
#include <uart.h>

// There is no need to update TX too frequently
#define UART_TX_FPS 5

UART* uart = NULL;
int16_t uart_divisor_cnt = 0;

UART::UART(SDL_Renderer *rend, int cnt, int init_val, int ct, int x, int y, int w, int h):
    Component(rend, cnt, init_val, ct),
    state(0), divisor(16), need_update_gui(false) {
  term = new Term(rend, x, y, w, h);
  uart_divisor_cnt = divisor - 1;
  int len = pin_array[UART_TX].vector_len;
  assert(len == 0 || len == 1); // either unbound or bound to 1 bit signal
  p_tx = (uint8_t *)pin_array[UART_TX].ptr;
}

UART::~UART() {
  SDL_DestroyTexture(get_texture(0));
}

void UART::update_gui() {
  term->update_gui();
}

void UART::check_tx() {
  uart_divisor_cnt = divisor - 1;

  uint8_t tx = *p_tx;
  if (state == 0) { // idle
    if (!tx) { // start bit
      data = 0;
      state ++;
    }
  } else if (state >= 1 && state <= 8) { // data
    data = (tx << 7) | (data >> 1);  // data bit
    state ++;
  } else if (state == 9) {
    if (tx) {
      term->feed_ch(data);
      state = 0;
      need_update_gui = true;
    } // stop bit
  }
}

void UART::update_state() {
  if (need_update_gui) {
    static uint64_t last = 0;
    uint64_t now = nvboard_get_time();
    if (now - last > 1000000 / UART_TX_FPS) {
      last = now;
      need_update_gui = false;
      update_gui();
    }
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
