#include <nvboard.h>
#include <render.h>

static std::string nvboard_home;
static SDL_Texture *tfpga_background;

static SDL_Texture* load_texture(SDL_Renderer *renderer, std::string path) {
  SDL_Texture *t = IMG_LoadTexture(renderer, (nvboard_home + path).c_str());
  assert(t != NULL);
  return t;
}

SDL_Texture* load_pic_texture(SDL_Renderer *renderer, std::string path) {
  return load_texture(renderer, "/resources/pic/" + path);
}

SDL_Texture* surface2texture(SDL_Renderer *renderer, SDL_Surface *s) {
  assert(s != NULL);
  SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
  assert(t != NULL);
  SDL_FreeSurface(s);
  return t;
}

SDL_Texture* new_texture(SDL_Renderer *renderer, int w, int h, int r, int g, int b) {
  SDL_Surface *s = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
  SDL_FillRect(s, NULL, SDL_MapRGB(s->format, r, g, b));
  return surface2texture(renderer, s);
}

void draw_thicker_line(SDL_Renderer *renderer, const SDL_Point *point, int n) {
  SDL_RenderDrawLines(renderer, point, n);

  SDL_Point *p = new SDL_Point[n];
  for (int i = 0; i < n ; i ++) {
    p[i] = point[i] + Point(-1, -1);
  }
  SDL_RenderDrawLines(renderer, &p[0], n);
  delete [] p;
}

void init_render(SDL_Renderer *renderer) {
  nvboard_home = getenv("NVBOARD_HOME");

  SDL_Rect rect_bg = {0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
#if 0
  tfpga_background = load_pic_texture(renderer, BG_PATH);
  SDL_RenderCopy(renderer, tfpga_background, NULL, &rect_bg);
#else
  uint8_t r, g, b, a;
  SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
  SDL_SetRenderDrawColor(renderer,
      (BOARD_BG_COLOR >> 16) & 0xff, (BOARD_BG_COLOR >> 8) & 0xff, BOARD_BG_COLOR & 0xff, 0);
  SDL_RenderFillRect(renderer, &rect_bg);
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
#endif
}
