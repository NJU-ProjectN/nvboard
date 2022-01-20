#include <cstdio>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

#include <verilated.h>
#include <nboard.h>

#include <stdlib.h>
#include <interface.h>

std::string nboard_home;

int main() {
  printf("nboard v0.1\n");
  // init verilog module
  
  // init SDL and SDL_image
  SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  IMG_Init(IMG_INIT_PNG);

  SDL_Window *vga_window = nullptr;
  SDL_Renderer *vga_render = nullptr;
  vga_window = SDL_CreateWindow("nboard-vga", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  vga_render = SDL_CreateRenderer(vga_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


  SDL_Window *window = nullptr;
  SDL_Renderer *render = nullptr;
  window = SDL_CreateWindow("nboard", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  
  usleep(300000);

  nboard_home = getenv("NBOARD_HOME");
  load_background(render);
  load_texture(render);
  //dbg_wait_esc();
  init_input();
  init_output();
  //dbg_wait_esc();
  update_input();
  dut_eval();
  update_output();
  update_gui_output(render);
  //dbg_wait_esc();
  init_gui(render);
  
  // the main cycle
  while (1) {
    int ev = read_event();
    if (ev == -1) {
      break;
    } else if (ev) {
      update_input();
      update_gui_input(render);
      dut_eval();
      update_output();
      update_gui_output(render);
    }

    if (read_clock()) {
      update_input();
      dut_eval();
      update_output();
      update_gui_output(render);
    }
  }
  
  SDL_DestroyWindow(vga_window);
  SDL_DestroyRenderer(vga_render);

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(render);
  IMG_Quit();
  SDL_Quit();
  return 0;
}