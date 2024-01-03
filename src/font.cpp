#include <nvboard.h>
#include <SDL_ttf.h>

static TTF_Font *font = NULL;

void init_font() {
  int ret = TTF_Init();
  assert(ret != -1);
  std::string nvboard_home = getenv("NVBOARD_HOME");
  font = TTF_OpenFont((nvboard_home + "/resources/font/" + "FreeMono.ttf").c_str(), 16);
  assert(font != NULL);
}

SDL_Texture* render_str(SDL_Renderer *renderer, std::string str, int wrap_len_in_pixel, int *w, int *h) {
  SDL_Color c = {.r = 0x00, .g = 0x00, .b = 0x00 };
  SDL_Surface *s = TTF_RenderText_Solid_Wrapped(font, str.c_str(), c, wrap_len_in_pixel);
  assert(s != NULL);
  SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
  assert(t != NULL);
  *w = s->w; *h = s->h;
  SDL_FreeSurface(s);
  return t;
}

void close_font() {
  TTF_CloseFont(font);
  font = NULL;
  TTF_Quit();
}
