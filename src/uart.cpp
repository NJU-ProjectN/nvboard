#include <nvboard.h>
#include <uart.h>

// There is no need to update TX too frequently
#define UART_TX_FPS 5

static UART* uart = NULL;
int16_t uart_divisor_cnt = 0;
bool is_uart_rx_idle = true;

UART::UART(SDL_Renderer *rend, int cnt, int init_val, int ct, int x, int y, int w, int h):
    Component(rend, cnt, init_val, ct),
    tx_state(0), rx_state(0), divisor(16), need_update_gui(false) {
  term = new Term(rend, x, y, w, h);

  SDL_Rect *rect_ptr = new SDL_Rect;
  *rect_ptr = (SDL_Rect){x, y, w, h};
  set_rect(rect_ptr, 0);

  uart_divisor_cnt = divisor - 1;
  int len = pin_array[UART_TX].vector_len;
  assert(len == 0 || len == 1); // either unbound or bound to 1 bit signal
  p_tx = (uint8_t *)pin_array[UART_TX].ptr;

  SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0x00, 0);
  SDL_RenderDrawLine(rend, x, y + h, x + w, y + h);
  SDL_SetRenderDrawColor(rend, 0xff, 0xff, 0xff, 0);

  rx_sending_str = "";
  pin_poke(UART_RX, 1);
}

UART::~UART() {
  SDL_DestroyTexture(get_texture(0));
}

void UART::update_gui() { // everything is done in update_state()
}

void UART::tx_receive() {
  uart_divisor_cnt = divisor - 1;

  uint8_t tx = *p_tx;
  if (tx_state == 0) { // idle
    if (!tx) { // start bit
      tx_data = 0;
      tx_state ++;
    }
  } else if (tx_state >= 1 && tx_state <= 8) { // data
    tx_data = (tx << 7) | (tx_data >> 1);  // data bit
    tx_state ++;
  } else if (tx_state == 9) {
    if (tx) { // stop bit
      tx_state = 0;
      term->feed_ch(tx_data);
      need_update_gui = true;
    }
  }
}

void UART::rx_send() {
  // the uart_divisor_cnt is maintained in tx_receive()
  if (rx_state == 0) { // idle
    rx_data = rx_sending_str[0];
    if (rx_data == '\0') {
      is_uart_rx_idle = true;
      return;
    }
    rx_sending_str.erase(0, 1);
    pin_poke(UART_RX, 0);  // start bit
    rx_state ++;
  } else if (rx_state >= 1 && rx_state <= 8) { // data
    pin_poke(UART_RX, rx_data & 1);  // data bit
    rx_data >>= 1;
    rx_state ++;
  } else if (rx_state == 9) {
    pin_poke(UART_RX, 1);  // stop bit
    rx_state = 0;
  }
}

void UART::rx_getchar(uint8_t ch) {
  rx_sending_str += ch;
  is_uart_rx_idle = false;
}

void UART::update_state() {
  if (need_update_gui) {
    static uint64_t last = 0;
    uint64_t now = nvboard_get_time();
    if (now - last > 1000000 / UART_TX_FPS) {
      last = now;
      need_update_gui = false;
      term->update_gui();
    }
  }
}

void UART::set_divisor(uint16_t d) {
  divisor = d;
}

void UART::term_focus(bool v) {
  term->set_focus(v);
}

static void init_render_local(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0);
  SDL_Point p[2];
  p[0] = Point(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2) - Point(0, 30);
  p[1] = p[0] - Point(10, 0);
  const char* label[2] = { "RX", "TX" };
  for (int i = 0; i < 2; i ++) {
    draw_thicker_line(renderer, p, 2);
    draw_str(renderer, label[i], p[1].x - 2 * CH_WIDTH, p[1].y - CH_HEIGHT / 2, 0xffffff);
    p[0].y -= CH_HEIGHT; p[1].y -= CH_HEIGHT;
  }
  draw_str(renderer, "UART-[", p[1].x - 8 * CH_WIDTH, p[1].y + CH_HEIGHT, 0xffffff);
}

void init_uart(SDL_Renderer *renderer) {
  init_render_local(renderer);
  int x = WINDOW_WIDTH / 2, y = 0, w = WINDOW_WIDTH / 2, h = WINDOW_HEIGHT / 2;
  uart = new UART(renderer, 1, 0, UART_TYPE, x, y, w, h);
  uart->add_pin(UART_TX);
  uart->add_pin(UART_RX);
  add_component(uart);
}

void uart_tx_receive() {
  uart->tx_receive();
}

void uart_rx_send() {
  uart->rx_send();
}

void uart_rx_getchar(uint8_t ch) {
  uart->rx_getchar(ch);
}

void uart_term_focus(bool v) {
  uart->term_focus(v);
}
