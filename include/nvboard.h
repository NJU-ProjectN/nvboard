#include <configs.h>
#include <constrs.h>
#include <render.h>
#include <component.h>
#include <vga.h>
#include <keyboard.h>

void nvboard_init();
void nvboard_quit();
void nvboard_bind_pin(void *signal, bool is_rt, bool is_output, int len, ...);
void nvboard_update();
void nvboard_set_vga_cycles(int cycles); //default: 1
