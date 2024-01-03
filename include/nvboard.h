#ifndef __NVBOARD_H__
#define __NVBOARD_H__

#include <pins.h>
#include <string>
#include <component.h>
#include <configs.h>
#include <SDL.h>
#include <SDL_image.h>

extern std::string nvboard_home;
void set_redraw();

static inline void fill_rect_texture(SDL_Renderer *renderer, SDL_Surface **surface, SDL_Texture **texture, int r, int g, int b) {
  SDL_FillRect(*surface, NULL, SDL_MapRGB((*surface)->format, r, g, b));
  *texture = SDL_CreateTextureFromSurface(renderer, *surface);
}

#endif
