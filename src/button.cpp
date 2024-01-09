#include <nvboard.h>

#define BTNC_X         520
#define BTNC_Y         250
#define BTNC_SEP       20
#define BTNC_WIDTH     30
#define BTNC_HEIGHT    30

static const SDL_Rect btn_rects[6] = {
  {BTNC_X,                            BTNC_Y,                                 BTNC_WIDTH, BTNC_HEIGHT }, // C
  {BTNC_X,                            BTNC_Y - BTNC_HEIGHT - BTNC_SEP,        BTNC_WIDTH, BTNC_HEIGHT }, // U
  {BTNC_X,                            BTNC_Y + BTNC_HEIGHT + BTNC_SEP,        BTNC_WIDTH, BTNC_HEIGHT }, // D
  {BTNC_X - BTNC_WIDTH - BTNC_SEP,    BTNC_Y,                                 BTNC_WIDTH, BTNC_HEIGHT }, // L
  {BTNC_X + BTNC_WIDTH + BTNC_SEP,    BTNC_Y,                                 BTNC_WIDTH, BTNC_HEIGHT }, // R
  {BTNC_X + BTNC_WIDTH + BTNC_SEP/2,  BTNC_Y - 2 * (BTNC_HEIGHT + BTNC_SEP),  BTNC_WIDTH, BTNC_HEIGHT }  // RST, but not draw
};

static void init_render_local(SDL_Renderer *renderer) {
  // draw surrounding lines
  const int gap = 8;
  int w = (BTNC_WIDTH + BTNC_SEP) * 3 - BTNC_SEP;
  draw_surrounding_line(renderer, Rect(btn_rects[3].x, btn_rects[1].y, w, w), gap);

  // draw indices for each button
  const char *str = "CUDLR";
  char buf[2] = "?";
  for (int i = 0; i < 5; i ++) {
    SDL_Point p = Point(btn_rects[i].x, btn_rects[i].y) + Point(BTNC_WIDTH + 2, BTNC_HEIGHT / 2)
                  - Point(0, CH_HEIGHT / 2);
    buf[0] = str[i];
    draw_str(renderer, buf, p.x, p.y, 0xffffff, BOARD_BG_COLOR);
  }

  // draw label
  str = "Button Pad";
  SDL_Point p = Point(btn_rects[3].x, btn_rects[1].y) - Point(0, gap) - Point(0, CH_HEIGHT / 2)
                + Point(w / 2, 0) - Point(CH_WIDTH * strlen(str) / 2, 0);
  draw_str(renderer, str, p.x, p.y, 0xffffff, BOARD_BG_COLOR);
}

void init_button(SDL_Renderer *renderer) {
  SDL_Texture *tbutton_on  = load_pic_texture(renderer, VBTN_ON_PATH);
  SDL_Texture *tbutton_off = load_pic_texture(renderer, VBTN_OFF_PATH);
  init_render_local(renderer);

  for (int i = 0; i < 5; ++i) {
    Component *ptr = new Component(renderer, 2, 0, BUTTON_TYPE);

    // off
    SDL_Rect *rect_ptr = new SDL_Rect;
    *rect_ptr = btn_rects[i];
    ptr->set_rect(rect_ptr, 0);
    ptr->set_texture(tbutton_off, 0);

    // on
    rect_ptr = new SDL_Rect;
    *rect_ptr = btn_rects[i];
    ptr->set_rect(rect_ptr, 1);
    ptr->set_texture(tbutton_on, 1);

    ptr->add_pin(BTNC + i);
    add_component(ptr);
  }
}
