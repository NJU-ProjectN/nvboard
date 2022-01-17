#include <cstdio>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

#include <verilated.h>
#include <nboard.h>
#include <topname.h>

TOP_NAME *dut_ptr;
std::string nboard_home;

int main() {
  printf("nboard v0.1\n");
  // init verilog module
  dut_ptr = new TOP_NAME;
  
  // init SDL and SDL_image
  SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  IMG_Init(IMG_INIT_PNG);
  SDL_Window *window = nullptr;
  SDL_Renderer *render = nullptr;
  window = SDL_CreateWindow("nboard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  
  nboard_home = getenv("NBOARD_HOME");
  load_background(render);
  load_texture(render);
  //dbg_wait_esc();
  init_input();
  init_output();
  //dbg_wait_esc();
  update_input(dut_ptr);
  dut_ptr->eval();
  //dbg_wait_esc();
  init_gui(render);
  
  // the main cycle
  while (1) {
    int ev = read_event();
    if (ev == -1) {
      break;
    } else if (ev) {
      update_input(dut_ptr);
      update_gui_input(render);
      dut_ptr->eval();
      update_output(dut_ptr);
      update_gui_output(render);
    }

    if (read_clock()) {
      update_input(dut_ptr);
      dut_ptr->eval();
      update_output(dut_ptr);
      update_gui_output(render);
    }
  }
  
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(render);
  IMG_Quit();
  SDL_Quit();
  delete dut_ptr;
  return 0;
}