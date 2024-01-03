#include <nvboard.h>

#define LED_X          60 + (4/2)
#define LED_Y          360
#define LED_SEP        14
#define LED_WIDTH      16
#define LED_HEIGHT     8

static SDL_Surface *sled_off, *sled_r, *sled_g, *sled_b,
                   *sled_rg, *sled_rb, *sled_gb, *sled_rgb;
static SDL_Texture *tled_off, *tled_r, *tled_g, *tled_b,
                   *tled_rg, *tled_rb, *tled_gb, *tled_rgb;

static void load_texture(SDL_Renderer *renderer) {
  sled_off = SDL_CreateRGBSurface(0, LED_WIDTH, LED_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &sled_off, &tled_off, 0x7f, 0x7f, 0x7f);

  sled_r = SDL_CreateRGBSurface(0, LED_WIDTH, LED_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &sled_r, &tled_r, 0xff, 0x00, 0x00);

  sled_g = SDL_CreateRGBSurface(0, LED_WIDTH, LED_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &sled_g, &tled_g, 0x00, 0xff, 0x00);

  sled_b = SDL_CreateRGBSurface(0, LED_WIDTH, LED_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &sled_b, &tled_b, 0x00, 0x00, 0xff);

  sled_rg = SDL_CreateRGBSurface(0, LED_WIDTH, LED_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &sled_rg, &tled_rg, 0xff, 0xff, 0x00);

  sled_rb = SDL_CreateRGBSurface(0, LED_WIDTH, LED_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &sled_rb, &tled_rb, 0xff, 0x00, 0xff);

  sled_gb = SDL_CreateRGBSurface(0, LED_WIDTH, LED_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &sled_gb, &tled_gb, 0x00, 0xff, 0xff);

  sled_rgb = SDL_CreateRGBSurface(0, LED_WIDTH, LED_HEIGHT, 32, 0, 0, 0, 0);
  fill_rect_texture(renderer, &sled_rgb, &tled_rgb, 0xff, 0xff, 0xff);
}

void init_led(SDL_Renderer *renderer) {
  load_texture(renderer);
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
