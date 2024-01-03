#include <nvboard.h>

#define SWITCH_X       60
#define SWITCH_Y       400
#define SWITCH_SEP     10
#define SWITCH_WIDTH   20
#define SWITCH_HEIGHT  40

static SDL_Surface *sswitch_on, *sswitch_off;
static SDL_Texture *tswitch_on, *tswitch_off;

static void load_texture(SDL_Renderer *renderer) {
  // switches
  sswitch_on = IMG_Load((nvboard_home + "/pic/" + VSW_ON_PATH).c_str());
  tswitch_on = SDL_CreateTextureFromSurface(renderer, sswitch_on);
  sswitch_off = IMG_Load((nvboard_home + "/pic/" + VSW_OFF_PATH).c_str());
  tswitch_off = SDL_CreateTextureFromSurface(renderer, sswitch_off);
}

void init_switch(SDL_Renderer *renderer) {
  load_texture(renderer);
  for (int i = 0; i < 16; ++i) {
    Component *ptr = new Component(renderer, 2, 0, SWITCH_TYPE);

    // off
    SDL_Rect *rect_ptr = new SDL_Rect;
    *rect_ptr = (SDL_Rect){SWITCH_X + (15 - i) * (SWITCH_WIDTH + SWITCH_SEP), SWITCH_Y, SWITCH_WIDTH, SWITCH_HEIGHT};
    ptr->set_rect(rect_ptr, 0);
    ptr->set_texture(tswitch_off, 0);

    // on
    rect_ptr = new SDL_Rect;
    *rect_ptr = (SDL_Rect){SWITCH_X + (15 - i) * (SWITCH_WIDTH + SWITCH_SEP), SWITCH_Y, SWITCH_WIDTH, SWITCH_HEIGHT};
    ptr->set_rect(rect_ptr, 1);
    ptr->set_texture(tswitch_on, 1);

    ptr->add_pin(SW0 + i);
    add_component(ptr);
  }
}
