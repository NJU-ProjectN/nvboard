#include <nvboard.h>

#define SWITCH_X       60
#define SWITCH_Y       400
#define SWITCH_SEP     10
#define SWITCH_WIDTH   20
#define SWITCH_HEIGHT  40

static void init_render_local(SDL_Renderer *renderer) {
  // draw line
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0);
  SDL_Point p[2];
  const int gap2 = 16;
  int w = (SWITCH_WIDTH + SWITCH_SEP) * 16 - SWITCH_SEP;
  p[0] = Point(SWITCH_X, SWITCH_Y) + Point(-gap2, SWITCH_HEIGHT / 2);
  p[1] = p[0] + Point(w + gap2 * 2, 0);
  draw_thicker_line(renderer, p, 2);

  int w_group4 = (SWITCH_WIDTH + SWITCH_SEP) * 4;
  p[0] = Point(SWITCH_X, SWITCH_Y) + Point(w_group4, 0) - Point(SWITCH_SEP / 2, 0)
         + Point(0, SWITCH_HEIGHT / 2) + Point(0, -SWITCH_SEP / 2);
  p[1] = p[0] + Point(0, SWITCH_SEP);
  for (int i = 0; i < 3; i ++) {
    draw_thicker_line(renderer, p, 2);
    p[0] = p[0] + Point(w_group4, 0);
    p[1] = p[1] + Point(w_group4, 0);
  }

  // draw label
  const char *str = "SW";
  SDL_Point p0 = Point(SWITCH_X, SWITCH_Y) - Point(gap2 + 4, 0) + Point(0, SWITCH_HEIGHT / 2)
                 - Point(0, CH_HEIGHT / 2) - Point(CH_WIDTH * strlen(str), 0);
  draw_str(renderer, str, p0.x, p0.y, 0xffffff);
}

void init_switch(SDL_Renderer *renderer) {
  SDL_Texture *tswitch_on  = load_pic_texture(renderer, VSW_ON_PATH);
  SDL_Texture *tswitch_off = load_pic_texture(renderer, VSW_OFF_PATH);
  init_render_local(renderer);
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
