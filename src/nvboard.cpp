#include <nvboard.h>
#include <keyboard.h>
#include <vga.h>
#include <uart.h>
#include <sys/time.h>
#include <stdarg.h>

#define FPS 60

static uint64_t boot_time = 0;

static uint64_t get_time_internal() {
  struct timeval now;
  gettimeofday(&now, NULL);
  uint64_t us = now.tv_sec * 1000000 + now.tv_usec;
  return us;
}

static uint64_t get_time() {
  uint64_t now = get_time_internal();
  return now - boot_time;
}

static SDL_Window *main_window = nullptr;
static SDL_Renderer *main_renderer = nullptr;
PinNode pin_array[NR_PINS];

static bool need_redraw = true;
void set_redraw() { need_redraw = true; }

void nvboard_update() {
  extern VGA *vga;
  extern uint8_t *vga_blank_n_ptr;
  if (*vga_blank_n_ptr) vga->update_state();

  extern KEYBOARD* kb;
  extern bool is_kb_idle;
  if (!is_kb_idle) kb->update_state();

  extern UART* uart;
  extern bool is_uart_idle;
  extern uint8_t *uart_tx_ptr;
  if (!(is_uart_idle && *uart_tx_ptr)) uart->check_tx();

  static uint64_t last = 0;
  static int cpf = 1; // count per frame
  static int cnt = 0;
  if ((-- cnt) < 0) {
    uint64_t now = get_time();
    uint64_t diff = now - last;
    int cpf_new = ((uint64_t)cpf * 1000000) / ((uint64_t)diff * FPS); // adjust cpf
    cnt += cpf_new - cpf;
    cpf = cpf_new;
    if (diff > 1000000 / FPS) {
      last = now;
      cnt = cpf;

      void read_event();
      read_event();
      update_components(main_renderer);
      if (need_redraw) {
        SDL_RenderPresent(main_renderer);
        need_redraw = false;
      }
    }
  }
}

void nvboard_init(int vga_clk_cycle) {
    printf("NVBoard v0.3 (2024.01.02)\n");
    // init SDL and SDL_image
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    IMG_Init(IMG_INIT_PNG);

    main_window = SDL_CreateWindow("nvboard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    main_renderer = SDL_CreateRenderer(main_window, -1, 
    #ifdef VSYNC
        SDL_RENDERER_PRESENTVSYNC |
    #endif
    #ifdef HARDWARE_ACC
        SDL_RENDERER_ACCELERATED |
    #else
        SDL_RENDERER_SOFTWARE |
    #endif
        0
    );
    SDL_SetRenderDrawColor(main_renderer, 0xff, 0xff, 0xff, 0);

    for (int i = 0; i < NR_PINS; i ++) {
      if (pin_array[i].ptr == NULL) pin_array[i].ptr = &pin_array[i].data;
    }

    void init_font(SDL_Renderer *renderer);
    init_font(main_renderer);
    init_render(main_renderer);
    init_components(main_renderer);
    init_gui(main_renderer);

    update_components(main_renderer);

    boot_time = get_time_internal();
    extern void vga_set_clk_cycle(int cycle);
    vga_set_clk_cycle(vga_clk_cycle);
}

void nvboard_quit(){
    delete_components();
    SDL_DestroyWindow(main_window);
    SDL_DestroyRenderer(main_renderer);
    IMG_Quit();
    SDL_Quit();
}

void nvboard_bind_pin(void *signal, int len, ...) {
  assert(len < 64);
  va_list ap;
  va_start(ap, len);
  for (int i = 0; i < len; i ++) {
    uint16_t pin = va_arg(ap, int);
    pin_array[pin].ptr = signal;
    pin_array[pin].vector_len = len;
    pin_array[pin].bit_offset = len - 1 - i;
  }
  va_end(ap);
}
