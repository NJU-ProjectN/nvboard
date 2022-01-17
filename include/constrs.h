#include <SDL2/SDL.h>
#include <topname.h>

#ifndef _VFPGA_CONSTRS_H
#define _VFPGA_CONSTRS_H

void update_input(TOP_NAME *dut_ptr);
void update_output(TOP_NAME *dut_ptr);

void init_input();
void init_output();

#define CLK_INPUT "clk"

#define RST_INPUT "rst"

#define BTN_INPUT "btnc", "btnu", "btnd", "btnl", "btnr" 

#define SWITCH_INPUT "sw0", "sw1", "sw2", "sw3", \
                     "sw4", "sw5", "sw6", "sw7", \
                     "sw8", "sw9", "sw10", "sw11", \
                     "sw12", "sw13", "sw14", "sw15"

#define NAIVE_LEDS "ld0", "ld1", "ld2", "ld3", \
                   "ld4", "ld5", "ld6", "ld7", \
                   "ld8", "ld9", "ld10", "ld11", \
                   "ld12", "ld13", "ld14", "ld15"

#define RGB_LEDS "r16", "g16", "b16", "r17", "g17", "b17"

#define SEG7_ENBS "an0", "an1", "an2", "an3", \
                  "an4", "an5", "an6", "an7"

#define SEG7_SEGS "sega", "segb", "segc", "segd", \
                  "sege", "segf", "segg", "decp"

//#define UART_OUTPUT

//#define VGA_OUTPUT

#endif