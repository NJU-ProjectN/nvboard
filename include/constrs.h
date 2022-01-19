#include <SDL2/SDL.h>
#include <topname.h>

#ifndef _VFPGA_CONSTRS_H
#define _VFPGA_CONSTRS_H

void update_input(TOP_NAME *dut_ptr);
void update_output(TOP_NAME *dut_ptr);

void init_input();
void init_output();

#define CLK_INPUT CLK

#define BTN_INPUT BTNC, BTNU, BTND, BTNL, BTNR, RST

#define SW_INPUT SW0, SW1, SW2, SW3, \
                 SW4, SW5, SW6, SW7, \
                 SW8, SW9, SW10, SW11, \
                 SW12, SW13, SW14, SW15

#define NAIVE_LEDS_OUTPUT LD0, LD1, LD2, LD3, \
                          LD4, LD5, LD6, LD7, \
                          LD8, LD9, LD10, LD11, \
                          LD12, LD13, LD14, LD15

#define RGB_LEDS_OUTPUT R16, G16, B16, R17, G17, B17

#define SEG7_ENBS_OUTPUT AN0, AN1, AN2, AN3, \
                         AN4, AN5, AN6, AN7

#define SEG7_SEGS_OUTPUT SEGA, SEGB, SEGC, SEGD, \
                         SEGE, SEGF, SEGG, DECP

//#define UART_OUTPUT

//#define VGA_OUTPUT

enum class input_pin{
  BTN_INPUT,
  SW_INPUT,
  CLK_INPUT
};

enum class output_pin{
  NAIVE_LEDS_OUTPUT,
  RGB_LEDS_OUTPUT,
  SEG7_ENBS_OUTPUT,
  SEG7_SEGS_OUTPUT
};

#endif