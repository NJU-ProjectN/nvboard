#ifndef __FONT_H__
#define __FONT_H__

#include <SDL.h>

#define CH_WIDTH  10
#define CH_HEIGHT 16

SDL_Surface* str2surface(const char *str, uint32_t fg);
SDL_Surface* str2surface(const char *str, uint32_t fg, uint32_t bg);
SDL_Texture* str2texture(SDL_Renderer *renderer, const char *str, uint32_t fg);
SDL_Texture* str2texture(SDL_Renderer *renderer, const char *str, uint32_t fg, uint32_t bg);
SDL_Surface* ch2surface(uint8_t ch, uint32_t fg);
SDL_Surface* ch2surface(uint8_t ch, uint32_t fg, uint32_t bg);
SDL_Texture* ch2texture(SDL_Renderer *renderer, uint8_t ch, uint32_t fg);
SDL_Texture* ch2texture(SDL_Renderer *renderer, uint8_t ch, uint32_t fg, uint32_t bg);
SDL_Texture* ch2texture_term(uint8_t ch);

#endif
