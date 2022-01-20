#include <cstdio>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

#include <nboard.h>
#include <stdlib.h>
#include <unistd.h>

std::string nboard_home;

int main() {
  printf("nvboard v0.2\n");
  // init verilog module
  
  // init SDL and SDL_image
  SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  IMG_Init(IMG_INIT_PNG);

  SDL_Window *vga_window = nullptr;
  SDL_Renderer *vga_renderer = nullptr;
  vga_window = SDL_CreateWindow("nboard-vga", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  vga_renderer = SDL_CreateRenderer(vga_window, -1, 
#ifdef HARDWARE_ACC
  SDL_RENDERER_ACCELERATED
#else
  SDL_RENDERER_SOFTWARE
#endif
  );


  SDL_Window *main_window = nullptr;
  SDL_Renderer *main_renderer = nullptr;
  main_window = SDL_CreateWindow("nvboard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  main_renderer = SDL_CreateRenderer(main_window, -1,
#ifdef HARDWARE_ACC
  SDL_RENDERER_ACCELERATED
#else
  SDL_RENDERER_SOFTWARE
#endif
  );
  
  // To avoid the SDL bugs on hby's linux
  usleep(200000);

  nboard_home = getenv("NBOARD_HOME");
  
  load_background(main_renderer);
  load_texture(main_renderer);
  //dbg_wait_esc("finish loading bg");
  init_components(main_renderer);
  init_gui(main_renderer);
  //dbg_wait_esc("finish init gui");
  init_input();
  init_output();

  dut_update();
  update_components(main_renderer);
  
  // the main cycle
  while (1) {
    int ev = read_event();
    if (ev == -1) {
      break;
    } else if (ev) {
      dut_update();
      update_components(main_renderer);
    }

    if (read_clock()) {
      dut_update();
      update_components(main_renderer);
    }
  }
  
  delete_components();

  SDL_DestroyWindow(vga_window);
  SDL_DestroyRenderer(vga_renderer);

  SDL_DestroyWindow(main_window);
  SDL_DestroyRenderer(main_renderer);
  IMG_Quit();
  SDL_Quit();
  return 0;
}