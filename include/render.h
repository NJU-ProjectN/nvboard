#ifndef __RENDER_H__
#define __RENDER_H__

#include <SDL.h>

#define BOARD_BG_COLOR 0x00008060

static inline SDL_Point Point(int x, int y) {
  return (SDL_Point){ .x = x, .y = y };
}

static inline SDL_Point operator+(const SDL_Point &A, const SDL_Point &B) {
  return Point(A.x + B.x, A.y + B.y);
}

static inline SDL_Point operator-(const SDL_Point &A, const SDL_Point &B) {
  return Point(A.x - B.x, A.y - B.y);
}

static inline SDL_Rect Rect(int x, int y, int w, int h) {
  return (SDL_Rect){ .x = x, .y = y, .w = w, .h = h };
}

static inline SDL_Rect Rect(const SDL_Point &top_left, int w, int h) {
  return Rect(top_left.x, top_left.y, w, h);
}

static inline SDL_Rect Rect(const SDL_Point &top_left, const SDL_Point &size) {
  return Rect(top_left, size.x, size.y);
}

static inline SDL_Rect operator+(const SDL_Rect &A, const SDL_Rect &B) {
  return Rect(A.x + B.x, A.y + B.y, A.w + B.w, A.h + B.h);
}

void draw_thicker_line(SDL_Renderer *renderer, const SDL_Point *point, int n);
void draw_surrounding_line(SDL_Renderer *renderer, SDL_Rect r, int gap);
void draw_str(SDL_Renderer *renderer, const char *str, int x, int y, uint32_t fg);
void draw_str(SDL_Renderer *renderer, const char *str, int x, int y, uint32_t fg, uint32_t bg);

#endif
