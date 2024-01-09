#ifndef __NVBOARD_H__
#define __NVBOARD_H__

#include <pins.h>
#include <font.h>
#include <component.h>
#include <configs.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

void set_redraw();
uint64_t nvboard_get_time();

void init_render(SDL_Renderer *renderer);
SDL_Texture* load_pic_texture(SDL_Renderer *renderer, std::string path);
SDL_Texture* new_texture(SDL_Renderer *renderer, int w, int h, int r, int g, int b);

#endif
