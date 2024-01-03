#include <nvboard.h>
#include <uart.h>

UART* uart = NULL;

SDL_Texture* render_str(SDL_Renderer *renderer, std::string str, int wrap_len_in_pixel, int *w, int *h);

UART::UART(SDL_Renderer *rend, int cnt, int init_val, int ct):
    Component(rend, cnt, init_val, ct),
    region_w(WINDOW_WIDTH / 2), region_h(WINDOW_HEIGHT / 2), str(" ") {
  SDL_Texture *temp_texture = SDL_CreateTexture(rend, SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING, region_w, region_h);
  set_texture(temp_texture, 0);
}

UART::~UART() {
  SDL_DestroyTexture(get_texture(0));
}

void UART::update_gui() {
  SDL_Renderer *r = get_renderer();
  SDL_Rect rect = *get_rect(0);
  SDL_RenderFillRect(r, &rect);
  int w = 0, h = 0;
  SDL_Texture *t = render_str(r, str, region_w, &w, &h);
  rect.w = w; rect.h = h;
  SDL_RenderCopy(r, t, NULL, &rect);
  SDL_DestroyTexture(t);
  set_redraw();
}

void UART::update_state() {
  static int i = 0;
  i ++;
  if (i < 10) return;
  i = 0;
  char last = str[str.length() - 1];
  str += last + 1;
  update_gui();
}

void init_uart(SDL_Renderer *renderer) {
  uart = new UART(renderer, 1, 0, UART_TYPE);
  SDL_Rect *rect_ptr = new SDL_Rect;
  *rect_ptr = (SDL_Rect){WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
  uart->set_rect(rect_ptr, 0);
  uart->add_pin(UART_TX);
  uart->add_pin(UART_RX);
  add_component(uart);
}
