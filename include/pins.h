#ifndef __PINS_H__
#define __PINS_H__

#include <../usr/include/pins.h>
#include <stdint.h>
#include <assert.h>

typedef struct PinNode {
  void *ptr;
  uint8_t data;
  uint8_t vector_len;
  uint8_t bit_offset;
} PinNode;
extern PinNode pin_array[];

static inline uint8_t pin_peek(int pin) {
  PinNode *p = &pin_array[pin];
  if (p->vector_len == 1) {
    return *(uint8_t *)p->ptr & 1;
  } else {
    uint64_t v = *(uint64_t *)p->ptr;
    return (v >> p->bit_offset) & 1;
  }
}

static inline uint8_t pin_peek8(int pin) {
  PinNode *p = &pin_array[pin];
  assert(p->vector_len == 8);
  return *(uint8_t *)p->ptr;
}

static inline void pin_poke(int pin, uint64_t v) {
  PinNode *p = &pin_array[pin];
  if (p->vector_len == 1) {
    *(uint8_t *)p->ptr = v & 1;
  } else {
    uint64_t x = *(uint64_t *)p->ptr;
    uint64_t mask = 1 << p->bit_offset;
    *(uint64_t *)p->ptr = (x & ~mask) | ((v & 1) << p->bit_offset);
  }
}

#endif
