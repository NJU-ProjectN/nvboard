#include <nvboard.h>
#include <SDL_ttf.h>

static SDL_Texture* font[128] = { NULL };

void init_font(SDL_Renderer *renderer) {
  int ret = TTF_Init();
  assert(ret != -1);
  std::string nvboard_home = getenv("NVBOARD_HOME");

  TTF_Font *f = TTF_OpenFont((nvboard_home + "/resources/font/" + "FreeMono.ttf").c_str(), 16);
  assert(f != NULL);
  TTF_SetFontHinting(f, TTF_HINTING_MONO);
  TTF_SetFontStyle(f, TTF_STYLE_BOLD);

  SDL_Color c = {.r = 0x00, .g = 0x00, .b = 0x00 };
  for (int i = 1; i < 128; i ++) {
    SDL_Surface *s = TTF_RenderGlyph_Solid(f, i, c);
    if (s == NULL) {
      printf("error = %s\n", SDL_GetError());
    }
    assert(s != NULL);
    assert(s->w == 10);
    assert(s->h == 16);
    SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
    assert(t != NULL);
    font[i] = t;
    SDL_FreeSurface(s);
  }

  TTF_CloseFont(f);
}

SDL_Texture* get_font_texture(uint8_t ch) {
  assert(ch < 128);
  return font[ch == 0 ? ' ' : ch];
}

void close_font() {
  TTF_Quit();
}
