#include <nvboard.h>

#define BTNC_X         520
#define BTNC_Y         250
#define BTNC_SEP       20
#define BTNC_WIDTH     30
#define BTNC_HEIGHT    30

static const SDL_Rect btn_rects[6] = {
  {BTNC_X,                            BTNC_Y,                                 BTNC_WIDTH, BTNC_HEIGHT },
  {BTNC_X,                            BTNC_Y - BTNC_HEIGHT - BTNC_SEP,        BTNC_WIDTH, BTNC_HEIGHT },
  {BTNC_X,                            BTNC_Y + BTNC_HEIGHT + BTNC_SEP,        BTNC_WIDTH, BTNC_HEIGHT },
  {BTNC_X - BTNC_WIDTH - BTNC_SEP,    BTNC_Y,                                 BTNC_WIDTH, BTNC_HEIGHT },
  {BTNC_X + BTNC_WIDTH + BTNC_SEP,    BTNC_Y,                                 BTNC_WIDTH, BTNC_HEIGHT },
  {BTNC_X + BTNC_WIDTH + BTNC_SEP/2,  BTNC_Y - 2 * (BTNC_HEIGHT + BTNC_SEP),  BTNC_WIDTH, BTNC_HEIGHT }  // RST, but not draw
};

void init_button(SDL_Renderer *renderer) {
  SDL_Texture *tbutton_on  = load_pic_texture(renderer, VBTN_ON_PATH);
  SDL_Texture *tbutton_off = load_pic_texture(renderer, VBTN_OFF_PATH);

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
