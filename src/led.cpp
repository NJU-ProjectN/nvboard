#include <nvboard.h>
#include <render.h>

#define LED_X          60 + (4/2)
#define LED_Y          360
#define LED_SEP        14
#define LED_WIDTH      16
#define LED_HEIGHT     8

static void init_render_local(SDL_Renderer *renderer) {
  // draw rectangle region
  const int gap = 12;
  int w = (LED_WIDTH + LED_SEP) * 16 - LED_SEP;
  SDL_Rect r = Rect(Point(LED_X, LED_Y) - Point(gap, gap), w + gap * 2, 76 + gap * 2);
  SDL_SetRenderDrawColor(renderer, 21, 153, 120, 0);
  SDL_RenderFillRect(renderer, &r);

  // draw line
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0);
  SDL_Point p[2];
  const int gap2 = gap + 4;
  p[0] = Point(LED_X, LED_Y) + Point(-gap2, LED_HEIGHT / 2);
  p[1] = p[0] + Point(w + gap2 * 2, 0);
  draw_thicker_line(renderer, p, 2);

  int w_group4 = (LED_WIDTH + LED_SEP) * 4;
  p[0] = Point(LED_X, LED_Y) + Point(w_group4, 0) - Point(LED_SEP / 2, 0)
         + Point(0, LED_HEIGHT / 2) + Point(0, -LED_SEP / 2);
  p[1] = p[0] + Point(0, LED_SEP);
  for (int i = 0; i < 3; i ++) {
    draw_thicker_line(renderer, p, 2);
    p[0] = p[0] + Point(w_group4, 0);
    p[1] = p[1] + Point(w_group4, 0);
  }

  // draw indices for each LED
  SDL_Point p0 = Point(LED_X, LED_Y) + Point(w, 0) - Point(LED_WIDTH / 2, 0)
                 + Point(0, LED_HEIGHT * 2);
  for (int i = 0; i < 16; i ++) {
    char buf[8];
    int n = snprintf(buf, 8, "%d", i);
    SDL_Texture *t = str2texture(renderer, buf, 0xffffff, (21 << 16) | (153 << 8) | 120);
    int w_texture = CH_WIDTH * n;
    SDL_Rect r = Rect(p0 - Point(w_texture / 2, 0), w_texture, CH_HEIGHT);
    SDL_RenderCopy(renderer, t, NULL, &r);
    SDL_DestroyTexture(t);
    p0.x -= LED_WIDTH + LED_SEP;
  }

  // draw the title
  const char *str = "LED";
  SDL_Texture *t = str2texture(renderer, str, 0xffffff, BOARD_BG_COLOR);
  int w0 = CH_WIDTH * strlen(str);
  p0 = Point(LED_X, LED_Y) - Point(gap2 + 4, 0) + Point(0, LED_HEIGHT / 2)
       - Point(0, CH_HEIGHT / 2) - Point(w0, 0);
  r = Rect(p0, w0, CH_HEIGHT);
  SDL_RenderCopy(renderer, t, NULL, &r);
  SDL_DestroyTexture(t);
}

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
  init_render_local(renderer);

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
