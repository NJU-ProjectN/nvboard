#include <nboard.h>
#include <map>
#include <SDL2/SDL_timer.h>
#include <iostream>

extern std::map<input_pin, int> input_map;
extern std::map<output_pin, int> output_map;

static bool clock_ena = false;
static uint64_t freq;
static uint64_t init_time;
static uint64_t clk_cnt;

bool read_clock() {
  if (!clock_ena) {
    freq = SDL_GetPerformanceFrequency();
    init_time = SDL_GetPerformanceCounter();
    clock_ena = true;
    clk_cnt = 0;
  }
  uint64_t now_time = SDL_GetPerformanceCounter();
  uint64_t now_clk_cnt = (now_time - init_time) * CLK_FREQ * 2 / freq;
  if (now_clk_cnt > clk_cnt) {
    clk_cnt = now_clk_cnt;
    input_map[input_pin::CLK] ^= 1;
    return true;
  }
  return false;
}