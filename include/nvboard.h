#ifndef __NVBOARD_H__
#define __NVBOARD_H__

#include <configs.h>
#include <constrs.h>
#include <render.h>
#include <component.h>
#include <vga.h>
#include <keyboard.h>

#define BIND_RATE_RT  true
#define BIND_RATE_SCR false
#define BIND_DIR_OUT  true
#define BIND_DIR_IN   false

void nvboard_init(int vga_clk_cycle = 1);
void nvboard_quit();
void nvboard_bind_pin(void *signal, bool is_rt, bool is_output, int len, ...);
void nvboard_update();

#endif
