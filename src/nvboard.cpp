#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <nvboard.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <assert.h>
#include <string>

using namespace std;

typedef struct PinMap {
  bool is_vec;
  bool is_output;
  union {
    uint16_t pin;
    struct {
      int len;
      uint16_t *pins;
    };
  };
  void *signal;
  PinMap *next;
} PinMap;

static PinMap *pin_map = NULL;

static SDL_Window *main_window = nullptr;
static SDL_Renderer *main_renderer = nullptr;
uint64_t input_map[NR_INPUT_PINS] = {0};
uint64_t output_map[NR_OUTPUT_PINS] = {0};
string nvboard_home;

int read_event();

static int read_event_flag = true;
static int render_flag = true;
static void alarm_sig_handler(int signum) {
    read_event_flag = true;
    render_flag = true;
}

static int nvboard_event_handler() {
    if(!read_event_flag) return 0;
    read_event_flag = false;
    int ev = read_event();
    if(ev != -1){
        update_components(main_renderer);
    } else {
      exit(0);
    }
    return ev;
}

static void nvboard_update_input(PinMap *p) {
  void *ptr = p->signal;
  if (!p->is_vec) {
    uint8_t val = input_map[p->pin];
    *(uint8_t *)ptr = val;
    return;
  }

  int len = p->len;
  uint64_t val = 0;
  for (int i = 0; i < len; i ++) {
    val <<= 1;
    val |= input_map[p->pins[i]];
  }
  if (len <= 8) { *(uint8_t *)ptr = val; }
  else if (len <= 16) { *(uint16_t *)ptr = val; }
  else if (len <= 32) { *(uint32_t *)ptr = val; }
  else if (len <= 64) { *(uint64_t *)ptr = val; }
}

static void nvboard_update_output(PinMap *p) {
  void *ptr = p->signal;
  if (!p->is_vec) {
    uint8_t val = *(uint8_t *)ptr;
    output_map[p->pin] = val & 1;
    return;
  }

  int len = p->len;
  uint64_t val = 0;
  if (len <= 8) { val = *(uint8_t *)ptr; }
  else if (len <= 16) { val = *(uint16_t *)ptr; }
  else if (len <= 32) { val = *(uint32_t *)ptr; }
  else if (len <= 64) { val = *(uint64_t *)ptr; }
  for (int i = 0; i < len; i ++) {
    output_map[p->pins[i]] = val & 1;
    val >>= 1;
  }
}

void nvboard_update() {
#ifdef CALCULATE_CLOCK_FREQUENCY
  static int update_count = 0, last_count = 0;
  static time_t begin = 0;

  update_count ++;
  if(!begin) time(&begin);
  else {
    time_t end;
    time(&end);
    if(end - begin >= CALCULATE_CLOCK_FREQUENCY_INTERVAL) {
      printf("Clock frequency: %ld cycles/s\n", (update_count - last_count) / (end - begin));
      begin = end;
      last_count = update_count;
    }
  }
#endif

  for (auto p = pin_map; p != NULL; p = p->next) {
    if (p->is_output) nvboard_update_output(p);
    else nvboard_update_input(p);
  }

  update_components(main_renderer);
  if(render_flag) {
    SDL_RenderPresent(main_renderer);
    render_flag = false;
  }

  nvboard_event_handler();
}

void nvboard_init() {
    printf("NVBoard v0.2\n");
    // init SDL and SDL_image
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    IMG_Init(IMG_INIT_PNG);

    main_window = SDL_CreateWindow("nvboard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH * 2, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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
    
    // To avoid the SDL bugs on hby's linux
    //usleep(200000);

    nvboard_home = getenv("NVBOARD_HOME");
    
    load_background(main_renderer);
    load_texture(main_renderer);
    init_components(main_renderer);
    init_gui(main_renderer);

    update_components(main_renderer);
    struct sigaction s;
    memset(&s, 0, sizeof(s));
    s.sa_handler = alarm_sig_handler;
    int ret = sigaction(SIGVTALRM, &s, NULL);
    assert(ret == 0);

    struct itimerval it = {};
    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = 1000000 / 60;
    it.it_interval = it.it_value;
    ret = setitimer(ITIMER_VIRTUAL, &it, NULL);
    assert(ret == 0);
}

void nvboard_quit(){
    delete_components();
    SDL_DestroyWindow(main_window);
    SDL_DestroyRenderer(main_renderer);
    IMG_Quit();
    SDL_Quit();
}

void nvboard_bind_output_pin(vector<uint16_t> &pin, void *signal) {
  PinMap *p = new PinMap;
  p->is_vec = true;
  p->is_output = true;
  p->len = pin.size();
  assert(p->len < 64);
  p->pins = new uint16_t[p->len];
  for (int i = 0; i < p->len; i ++) {
    p->pins[i] = pin[p->len - 1 - i];
  }
  p->signal = signal;
  p->next = pin_map;
  pin_map = p;
}

void nvboard_bind_input_pin(vector<uint16_t> &pin, void *signal) {
  PinMap *p = new PinMap;
  p->is_vec = true;
  p->is_output = false;
  p->len = pin.size();
  assert(p->len < 64);
  p->pins = new uint16_t[p->len];
  for (int i = 0; i < p->len; i ++) {
    p->pins[i] = pin[i];
  }
  p->signal = signal;
  p->next = pin_map;
  pin_map = p;
}

void nvboard_bind_output_pin(uint16_t pin, void *signal) {
  PinMap *p = new PinMap;
  p->is_vec = false;
  p->is_output = true;
  p->pin = pin;
  p->signal = signal;
  p->next = pin_map;
  pin_map = p;
}

void nvboard_bind_input_pin(uint16_t pin, void *signal) {
  PinMap *p = new PinMap;
  p->is_vec = false;
  p->is_output = false;
  p->pin = pin;
  p->signal = signal;
  p->next = pin_map;
  pin_map = p;
}
