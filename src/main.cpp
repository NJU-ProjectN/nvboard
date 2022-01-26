#include <cstdio>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

#include <nboard.h>
#include <stdlib.h>
#include <unistd.h>

std::string nboard_home;

int main() {
  printf("nvboard v0.2\n");
  
  // init SDL and SDL_image
  SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  IMG_Init(IMG_INIT_PNG);
  //std::cerr << "ok\n";
  SDL_Window *main_window = nullptr;
  SDL_Renderer *main_renderer = nullptr;
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
  //dbg_wait_esc("finish update");
  
  unsigned nxt_frame_time = SDL_GetTicks();
  unsigned cur_time = SDL_GetTicks();
  // the main cycle
  while (1) {
    bool halt = false;
    do {
      cur_time = SDL_GetTicks();
      int ev = read_event();
      if (ev == -1) {
        halt = true;
        break;
      } else if (ev) {
        dut_update();
        update_components(main_renderer);
      }
  
      if (read_clock()) {
        dut_update();
        update_components(main_renderer);
      }
    } while (cur_time < nxt_frame_time);
    if (halt) {
      break;
    }
    if ((int)(cur_time - nxt_frame_time) > 1000) {
      nxt_frame_time = cur_time + 1000 / 60;
    } else {
      nxt_frame_time += 1000 / 60;
    }
    SDL_RenderPresent(main_renderer);
  }
  
  delete_components();
  
  SDL_DestroyWindow(main_window);
  SDL_DestroyRenderer(main_renderer);
  IMG_Quit();
  SDL_Quit();
  return 0;
}