#ifndef _VFPGA_VGA_H
#define _VFPGA_VGA_H

#include <constrs.h>
#include <SDL2/SDL.h>

#define VGA_DEFAULT_WIDTH  640
#define VGA_DEFAULT_HEIGHT 480

enum { //VGA_MOD_ID
  VGA_MODE_640_480, NR_VGA_MODE
};

struct VGA_MODE{
  int h_frontporch, h_active, h_backporch, h_total;
  int v_frontporch, v_active, v_backporch, v_total;
};

class VGA{
  SDL_Window *vga_window;
  SDL_Renderer *vga_renderer;
  SDL_Texture *vga_texture;
  int vga_screen_width, vga_screen_height;
  uint32_t *pixels;
  int vga_pos;
  int vga_pre_clk, vga_pre_vsync, vga_pre_hsync;
  int vga_vaddr, vga_haddr;
  void update_screen();
public:
  VGA();
  ~VGA();
  void update_vga();
};

#endif
