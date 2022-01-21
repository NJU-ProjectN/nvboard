#include <vga.h>
#include <constrs.h>
#include <macro.h>

VGA_MODE vga_mod_accepted[NR_VGA_MODE] = {
  [VGA_MODE_640_480] = {
    .h_frontporch = 96,
    .h_active = 144,
    .h_backporch = 784,
    .h_total = 800,
    .v_frontporch = 2,
    .v_active = 35,
    .v_backporch = 515,
    .v_total = 525,
  },
};

VGA::VGA(): 
    vga_screen_width(VGA_DEFAULT_WIDTH), vga_screen_height(VGA_DEFAULT_HEIGHT),
    vga_pre_clk(0), vga_pre_hsync(0), vga_pre_vsync(0),
    vga_pos(0), vga_vaddr(0), vga_haddr(0) {
  vga_window = SDL_CreateWindow("nboard-vga",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    vga_screen_width, vga_screen_height, 
    SDL_WINDOW_SHOWN
  );
  vga_renderer = SDL_CreateRenderer(vga_window, -1, 
#ifdef HARDWARE_ACC
  SDL_RENDERER_ACCELERATED
#else
  SDL_RENDERER_SOFTWARE
#endif
  );
  vga_texture = SDL_CreateTexture(vga_renderer, SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STATIC, vga_screen_width, vga_screen_height);
  pixels = new uint32_t[vga_screen_width * vga_screen_height];
}

VGA::~VGA() {
  SDL_DestroyWindow(vga_window);
  SDL_DestroyRenderer(vga_renderer);
  SDL_DestroyTexture(vga_texture);
  delete []pixels;
}

void VGA::update_screen() {
  static int frames = 0;
  frames ++;
  printf("%d frames\n", frames);
  SDL_UpdateTexture(vga_texture, NULL, pixels, vga_screen_width * sizeof(uint32_t));
  SDL_RenderClear(vga_renderer);
  SDL_RenderCopy(vga_renderer, vga_texture, NULL, NULL);
  SDL_RenderPresent(vga_renderer);
}

void VGA::update_vga() {
  int vga_clk = output_map[output_pin::VGA_CLK];
  int vga_vsync = output_map[output_pin::VGA_VSYNC];
  int vga_hsync = output_map[output_pin::VGA_HSYNC];
  int vga_blank_n = output_map[output_pin::VGA_BLANK_N];
  if(!VGA_POS_EDGE(clk)){
    vga_pre_clk = vga_clk;
    return;
  }
  if(vga_blank_n) {
    int vga_r = output_map[output_pin::VGA_R];
    int vga_g = output_map[output_pin::VGA_G];
    int vga_b = output_map[output_pin::VGA_B];
    uint32_t vga_rgb = (vga_r << 16) | (vga_g << 8) | (vga_b);
    pixels[vga_pos] = vga_rgb;
    vga_pos ++;
  }
  if(VGA_NEG_EDGE(vsync)) {
    vga_pos = 0;
    update_screen();
  }
  vga_pre_vsync = vga_vsync;
  vga_pre_clk = vga_clk;
}
