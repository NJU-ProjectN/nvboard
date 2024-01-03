#include <nvboard.h>

#define LED_X          60 + (4/2)
#define LED_Y          360
#define LED_SEP        14
#define LED_WIDTH      16
#define LED_HEIGHT     8

void init_led(SDL_Renderer *renderer) {
  SDL_Texture *tled_off = new_texture(renderer, LED_WIDTH, LED_HEIGHT, 0x7f, 0x7f, 0x7f);
  SDL_Texture *tled_g   = new_texture(renderer, LED_WIDTH, LED_HEIGHT, 0x00, 0xff, 0x00);
#if 0
  SDL_Texture* tled_r   = new_texture(renderer, LED_WIDTH, LED_HEIGHT, 0xff, 0x00, 0x00);
  SDL_Texture* tled_b   = new_texture(renderer, LED_WIDTH, LED_HEIGHT, 0x00, 0x00, 0xff);
  SDL_Texture* tled_rg  = new_texture(renderer, LED_WIDTH, LED_HEIGHT, 0xff, 0xff, 0x00);
  SDL_Texture* tled_rb  = new_texture(renderer, LED_WIDTH, LED_HEIGHT, 0xff, 0x00, 0xff);
  SDL_Texture* tled_gb  = new_texture(renderer, LED_WIDTH, LED_HEIGHT, 0x00, 0xff, 0xff);
  SDL_Texture* tled_rgb = new_texture(renderer, LED_WIDTH, LED_HEIGHT, 0xff, 0xff, 0xff);
#endif

  for (int i = 0; i < 16; ++i) {
    Component *ptr = new Component(renderer, 2, 0, NAIVE_LED_TYPE);

    // off
    SDL_Rect *rect_ptr = new SDL_Rect;
    *rect_ptr = (SDL_Rect){LED_X + (15 - i) * (LED_WIDTH + LED_SEP), LED_Y, LED_WIDTH, LED_HEIGHT};
    ptr->set_rect(rect_ptr, 0);
    ptr->set_texture(tled_off, 0);

    // on
    rect_ptr = new SDL_Rect;
    *rect_ptr = (SDL_Rect){LED_X + (15 - i) * (LED_WIDTH + LED_SEP), LED_Y, LED_WIDTH, LED_HEIGHT};
    ptr->set_rect(rect_ptr, 1);
    ptr->set_texture(tled_g, 1);

    ptr->add_pin(LD0 + i);
    add_component(ptr);
  }
}
