#ifndef __NVBOARD_H__
#define __NVBOARD_H__

#include <pins.h>

void nvboard_init(int vga_clk_cycle = 1);
void nvboard_quit();
void nvboard_bind_pin(void *signal, int len, ...);
void nvboard_update();

#endif
