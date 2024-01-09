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

static void draw_thicker_line(SDL_Renderer *renderer, const SDL_Point *point, int n) {
  SDL_RenderDrawLines(renderer, point, n);

  SDL_Point *p = new SDL_Point[n];
  for (int i = 0; i < n ; i ++) {
    p[i] = point[i] + Point(-1, -1);
  }
  SDL_RenderDrawLines(renderer, &p[0], n);
  delete [] p;
}

void draw_surrounding_line(SDL_Renderer *renderer, SDL_Rect r,
    int gap) { // gap between surrounding lines and component
  SDL_Point top_left = Point(r.x, r.y) + Point(-gap, -gap);
  const int w = r.w + gap * 2;
  const int h = r.h + gap * 2;
  const int d = 12;
  SDL_Point p[9];
  p[0] = top_left + Point(d, 0);
  p[1] = p[0] + Point(w - 2 * d, 0);
  p[2] = p[1] + Point(d, d);
  p[3] = p[2] + Point(0, h - 2 * d);
  p[4] = p[3] + Point(-d, d);
  p[5] = p[4] - Point(w - 2 * d, 0);
  p[6] = p[5] - Point(d, d);
  p[7] = p[6] - Point(0, h - 2 * d);
  p[8] = p[0];
  draw_thicker_line(renderer, p, 9);
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
