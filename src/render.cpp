#include <nvboard.h>

static std::string nvboard_home;
static SDL_Texture *tfpga_background;

static SDL_Texture* load_texture(SDL_Renderer *renderer, std::string path) {
  SDL_Texture *t = IMG_LoadTexture(renderer, (nvboard_home + path).c_str());
  assert(t != NULL);
  return t;
}

SDL_Texture* load_pic_texture(SDL_Renderer *renderer, std::string path) {
  return load_texture(renderer, "/pic/" + path);
}

SDL_Texture* new_texture(SDL_Renderer *renderer, int w, int h, int r, int g, int b) {
  SDL_Surface *s = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
  SDL_FillRect(s, NULL, SDL_MapRGB(s->format, r, g, b));
  SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
  assert(t != NULL);
  SDL_FreeSurface(s);
  return t;
}

void init_render(SDL_Renderer *renderer) {
  nvboard_home = getenv("NVBOARD_HOME");

  tfpga_background = load_pic_texture(renderer, BG_PATH);
  SDL_Rect rect_bg = {0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
  SDL_RenderCopy(renderer, tfpga_background, NULL, &rect_bg);
}
