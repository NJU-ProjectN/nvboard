#include <nvboard.h>

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

static void draw_str_internal(SDL_Renderer *renderer, SDL_Texture *t, const char *str, int x, int y) {
  SDL_Rect r = Rect(x, y, CH_WIDTH * strlen(str), CH_HEIGHT);
  SDL_RenderCopy(renderer, t, NULL, &r);
  SDL_DestroyTexture(t);
}

void draw_str(SDL_Renderer *renderer, const char *str, int x, int y, uint32_t fg) {
  SDL_Texture *t = str2texture(renderer, str, fg);
  draw_str_internal(renderer, t, str, x, y);
}

void draw_str(SDL_Renderer *renderer, const char *str, int x, int y, uint32_t fg, uint32_t bg) {
  SDL_Texture *t = str2texture(renderer, str, fg, bg);
  draw_str_internal(renderer, t, str, x, y);
}


void init_render(SDL_Renderer *renderer) {
  nvboard_home = getenv("NVBOARD_HOME");

  SDL_Rect rect_bg = {0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
  SDL_SetRenderDrawColor(renderer,
      (BOARD_BG_COLOR >> 16) & 0xff, (BOARD_BG_COLOR >> 8) & 0xff, BOARD_BG_COLOR & 0xff, 0);
  SDL_RenderFillRect(renderer, &rect_bg);

  extern SDL_Texture *nvboard_texture;
  int w, h;
  SDL_QueryTexture(nvboard_texture, NULL, NULL, &w, &h);
  SDL_Rect r = Rect(60, 140, w, h);
  SDL_RenderCopy(renderer, nvboard_texture, NULL, &r);
  SDL_DestroyTexture(nvboard_texture);

  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0);
  SDL_Point p[2];
  p[0] = Point(60, 140 + h + 4);
  p[1] = Point(WINDOW_WIDTH / 2 / 2 + 120, p[0].y);
  draw_thicker_line(renderer, p, 2);
  p[0].y += 4, p[1].y += 4;
  draw_thicker_line(renderer, p, 2);

  draw_str(renderer, VERSION_STR, 60 + w + CH_WIDTH, 140 + h - CH_HEIGHT, 0xffffff);
}
