#include <nvboard.h>
#include <SDL_ttf.h>

static TTF_Font *font = NULL;

void init_font() {
  int ret = TTF_Init();
  assert(ret != -1);
  std::string nvboard_home = getenv("NVBOARD_HOME");
  font = TTF_OpenFont((nvboard_home + "/resources/font/" + "FreeMono.ttf").c_str(), 28);
  assert(font != NULL);
}

void test_font(SDL_Renderer *renderer) {
  SDL_Color c = {.r = 0xff, .g = 0xff, .b = 0xff };
  SDL_Surface *s = TTF_RenderText_Solid_Wrapped(font, "abc", c, 0);
  SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
  SDL_Rect r = { 0, 0, s->w, s->h };
  SDL_RenderCopy(renderer, t, NULL, &r);
  SDL_FreeSurface(s);
  SDL_DestroyTexture(t);
}

void close_font() {
  TTF_CloseFont(font);
  font = NULL;
  TTF_Quit();
}
