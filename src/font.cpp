#include <nvboard.h>
#include <SDL_ttf.h>

static TTF_Font *font;
static SDL_Texture* font_texture[128] = { NULL };

void init_font(SDL_Renderer *renderer) {
  int ret = TTF_Init();
  assert(ret != -1);
  std::string nvboard_home = getenv("NVBOARD_HOME");

  font = TTF_OpenFont((nvboard_home + "/resources/font/" + "FreeMono.ttf").c_str(), 16);
  assert(font != NULL);
  TTF_SetFontHinting(font, TTF_HINTING_MONO);
  TTF_SetFontStyle(font, TTF_STYLE_BOLD);

  SDL_Color fg = {.r = 0x00, .g = 0x00, .b = 0x00 };
  SDL_Color bg = {.r = 0xff, .g = 0xff, .b = 0xff };
  for (int i = 1; i < 128; i ++) {
    SDL_Surface *s = TTF_RenderGlyph_Shaded(font, i, fg, bg);
    if (s == NULL) {
      printf("error = %s\n", SDL_GetError());
      assert(s != NULL);
    }
    assert(s->w == 10);
    assert(s->h == 16);
    SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
    assert(t != NULL);
    font_texture[i] = t;
    SDL_FreeSurface(s);
  }
}

SDL_Surface* get_font_surface(const char *str) {
  SDL_Color fg = {.r = 0x00, .g = 0x00, .b = 0x00 };
  SDL_Surface *s = TTF_RenderText_Blended_Wrapped(font, str, fg, 0);
  if (s == NULL) {
    printf("error = %s\n", SDL_GetError());
    assert(s != NULL);
  }
  return s;
}

SDL_Texture* get_font_texture(uint8_t ch) {
  assert(ch < 128);
  return font_texture[ch == 0 ? ' ' : ch];
}

void close_font() {
  TTF_CloseFont(font);
  TTF_Quit();
}
