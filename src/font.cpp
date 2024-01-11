#include <nvboard.h>
#include <SDL_ttf.h>

static TTF_Font *font = NULL;
static SDL_Texture* font_texture_term[128] = { NULL };
SDL_Texture* surface2texture(SDL_Renderer *renderer, SDL_Surface *s);
SDL_Texture *nvboard_texture = NULL;

void init_font(SDL_Renderer *renderer) {
  int ret = TTF_Init();
  assert(ret != -1);
  std::string nvboard_home = getenv("NVBOARD_HOME");

  font = TTF_OpenFont((nvboard_home + "/resources/font/" + "FreeMono.ttf").c_str(), 48);
  assert(font != NULL);
  TTF_SetFontHinting(font, TTF_HINTING_MONO);
  TTF_SetFontStyle(font, TTF_STYLE_BOLD);
  nvboard_texture = str2texture(renderer, "NVBoard", 0xffffff, BOARD_BG_COLOR);

  TTF_SetFontSize(font, CH_HEIGHT);
  SDL_Color fg = {.r = 0x00, .g = 0x00, .b = 0x00 };
  SDL_Color bg = {.r = 0xff, .g = 0xff, .b = 0xff };
  for (int i = 1; i < 128; i ++) {
    SDL_Surface *s = TTF_RenderGlyph_Shaded(font, i, fg, bg);
    assert(s->w == CH_WIDTH);
    assert(s->h == CH_HEIGHT);
    font_texture_term[i] = surface2texture(renderer, s);
  }
}

SDL_Surface* str2surface(const char *str, uint32_t fg) {
  SDL_Color c_fg = {.r = (uint8_t)(fg >> 16), .g = (uint8_t)(fg >> 8), .b = (uint8_t)fg };
  SDL_Surface *s = TTF_RenderText_Blended_Wrapped(font, str, c_fg, 0);
  assert(s != NULL);
  return s;
}

SDL_Surface* str2surface(const char *str, uint32_t fg, uint32_t bg) {
  SDL_Color c_fg = {.r = (uint8_t)(fg >> 16), .g = (uint8_t)(fg >> 8), .b = (uint8_t)fg };
  SDL_Color c_bg = {.r = (uint8_t)(bg >> 16), .g = (uint8_t)(bg >> 8), .b = (uint8_t)bg };
  SDL_Surface *s = TTF_RenderText_Shaded_Wrapped(font, str, c_fg, c_bg, 0);
  assert(s != NULL);
  return s;
}

SDL_Texture* str2texture(SDL_Renderer *renderer, const char *str, uint32_t fg) {
  return surface2texture(renderer, str2surface(str, fg));
}

SDL_Texture* str2texture(SDL_Renderer *renderer, const char *str, uint32_t fg, uint32_t bg) {
  return surface2texture(renderer, str2surface(str, fg, bg));
}

SDL_Surface* ch2surface(uint8_t ch, uint32_t fg) {
  SDL_Color c_fg = {.r = (uint8_t)(fg >> 16), .g = (uint8_t)(fg >> 8), .b = (uint8_t)fg };
  SDL_Surface *s = TTF_RenderGlyph_Blended(font, ch, c_fg);
  assert(s != NULL);
  return s;
}

SDL_Surface* ch2surface(uint8_t ch, uint32_t fg, uint32_t bg) {
  SDL_Color c_fg = {.r = (uint8_t)(fg >> 16), .g = (uint8_t)(fg >> 8), .b = (uint8_t)fg };
  SDL_Color c_bg = {.r = (uint8_t)(bg >> 16), .g = (uint8_t)(bg >> 8), .b = (uint8_t)bg };
  SDL_Surface *s = TTF_RenderGlyph_Shaded(font, ch, c_fg, c_bg);
  assert(s != NULL);
  return s;
}

SDL_Texture* ch2texture(SDL_Renderer *renderer, uint8_t ch, uint32_t fg) {
  return surface2texture(renderer, ch2surface(ch, fg));
}

SDL_Texture* ch2texture(SDL_Renderer *renderer, uint8_t ch, uint32_t fg, uint32_t bg) {
  return surface2texture(renderer, ch2surface(ch, fg, bg));
}

SDL_Texture* ch2texture_term(uint8_t ch) {
  assert(ch < 128);
  return font_texture_term[ch == 0 ? ' ' : ch];
}

void close_font() {
  TTF_CloseFont(font);
  TTF_Quit();
}
