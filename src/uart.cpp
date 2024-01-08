#include <nvboard.h>
#include <uart.h>

// There is no need to update TX too frequently
#define UART_TX_FPS 5

UART* uart = NULL;
int16_t uart_divisor_cnt = 0;


UART::UART(SDL_Renderer *rend, int cnt, int init_val, int ct, int x, int y, int w, int h):
    Component(rend, cnt, init_val, ct),
    state(0), divisor(16), tx_update_gui(false) {
  tx_term = new Term(rend, x, y, w, h);
  rx_term = new Term(rend, x, y + h, w, 20);
  uart_divisor_cnt = divisor - 1;
  int len = pin_array[UART_TX].vector_len;
  assert(len == 0 || len == 1); // either unbound or bound to 1 bit signal
  p_tx = (uint8_t *)pin_array[UART_TX].ptr;

  rx_term->feed_str(rx_input_prompt);
  rx_input = "";
  rx_update_gui = true;
}

UART::~UART() {
  SDL_DestroyTexture(get_texture(0));
}

void UART::update_gui() { // everything is done in update_state()
}

void UART::tx_check() {
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
    if (tx) { // stop bit
      state = 0;
      tx_term->feed_ch(data);
      tx_update_gui = true;
    }
  }
}

void UART::rx_getchar(uint8_t ch) {
  if (ch == '\n') {
    printf("Get RX input = %s\n", rx_input.c_str());
    rx_term->clear();
    rx_term->feed_str(rx_input_prompt);
    rx_input = "";
  }
  else {
    rx_input += ch;
    rx_term->feed_ch(ch);
  }
  rx_update_gui = true;
}

void UART::update_state() {
  if (tx_update_gui) {
    static uint64_t last = 0;
    uint64_t now = nvboard_get_time();
    if (now - last > 1000000 / UART_TX_FPS) {
      last = now;
      tx_update_gui = false;
      tx_term->update_gui();
    }
  }
  if (rx_update_gui) {
    rx_update_gui = false;
    rx_term->update_gui();
  }
}

void UART::set_divisor(uint16_t d) {
  divisor = d;
}

void init_uart(SDL_Renderer *renderer) {
  int x = WINDOW_WIDTH / 2, y = 0, w = WINDOW_WIDTH / 2, h = WINDOW_HEIGHT / 2;
  uart = new UART(renderer, 1, 0, UART_TYPE, x, y, w, h);
  SDL_Rect *rect_ptr = new SDL_Rect;
  *rect_ptr = (SDL_Rect){x, y + h, w, 10};
  uart->set_rect(rect_ptr, 0);
  uart->add_pin(UART_TX);
  uart->add_pin(UART_RX);
  add_component(uart);
}

void uart_rx_getchar(uint8_t ch) {
  uart->rx_getchar(ch);
}
