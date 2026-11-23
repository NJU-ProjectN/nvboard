#ifndef __NVBOARD_H__
#define __NVBOARD_H__

#include <pins.h>

void nvboard_init(int vga_clk_cycle = 1);
void nvboard_quit();
void nvboard_bind_pin(void *signal, int len, ...);
// Update the state of every component, call it once per simulated cycle.
// It returns false once the user has closed the board window; the caller
// should stop the simulation and call nvboard_quit() afterwards.
bool nvboard_update();

#endif
