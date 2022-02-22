#include <configs.h>
#include <constrs.h>
#include <render.h>
#include <component.h>
#include <vga.h>
#include <keyboard.h>

using namespace std;

void nvboard_init();
void nvboard_quit();
void nvboard_bind_output_pin(vector <uint16_t> &pin, void *signal);
void nvboard_bind_input_pin(vector <uint16_t> &pin, void *signal);
void nvboard_bind_output_pin(uint16_t pin, void *signal);
void nvboard_bind_input_pin(uint16_t pin, void *signal);
void nvboard_update();
