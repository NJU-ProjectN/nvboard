#include <configs.h>
#include <constrs.h>
#include <render.h>
#include <component.h>
#include <vga.h>
#include <keyboard.h>

#define NVBOARD_DIR_input  false
#define NVBOARD_DIR_output true

void nvboard_init();
void nvboard_quit();
void nvboard_bind_pin(void *signal, bool is_output, int len, ...);
void nvboard_update();
