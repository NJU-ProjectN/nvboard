#include <configs.h>
#include <constrs.h>
#include <render.h>
#include <component.h>
#include <vga.h>
#include <keyboard.h>

using namespace std;

void nvboard_init();
void nvboard_quit();
void nvboard_bind_pin(vector <output_pin> &pin, void *signal);
void nvboard_bind_pin(vector <input_pin> &pin, void *signal);
void nvboard_bind_pin(output_pin pin, void *signal);
void nvboard_bind_pin(input_pin pin, void *signal);
void nvboard_update();
