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
  printf("nvboard v0.2\n");
  // init verilog module
  dut_ptr = new TOP_NAME;
  
  // init SDL and SDL_image
  SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  IMG_Init(IMG_INIT_PNG);
  SDL_Window *main_window = nullptr;
  SDL_Renderer *main_renderer = nullptr;
  main_window = SDL_CreateWindow("nvboard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  main_renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  //vga_window = SDL_CreateWindow("nvboard-vga", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  //vga_renderer = SDL_CreateRenderer(vga_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  
  nboard_home = getenv("NBOARD_HOME");
  
  load_background(main_renderer);
  load_texture(main_renderer);
  //dbg_wait_esc("finish loading bg");
  init_components(main_renderer);
  init_gui(main_renderer);
  //dbg_wait_esc("finish init gui");
  init_input();
  init_output();
  update_input(dut_ptr);
  dut_ptr->eval();
  update_output(dut_ptr);
  update_components(main_renderer);
  
  // the main cycle
  while (1) {
    int ev = read_event();
    if (ev == -1) {
      break;
    } else if (ev) {
      update_input(dut_ptr);
      dut_ptr->eval();
      update_output(dut_ptr);
      update_components(main_renderer);
    }

    if (read_clock()) {
      update_input(dut_ptr);
      dut_ptr->eval();
      update_output(dut_ptr);
      update_components(main_renderer);
    }
  }
  
  delete_components();
  SDL_DestroyWindow(main_window);
  SDL_DestroyRenderer(main_renderer);
  IMG_Quit();
  SDL_Quit();
  delete dut_ptr;
  return 0;
}