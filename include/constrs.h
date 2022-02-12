#ifndef _VFPGA_CONSTRS_H
#define _VFPGA_CONSTRS_H

#include <map>
#include <vector>

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

#define VGA_OUTPUT VGA_CLK, VGA_VSYNC, VGA_HSYNC, VGA_BLANK_N,  \
                   VGA_R0, VGA_R1, VGA_R2, VGA_R3, VGA_R4, VGA_R5, VGA_R6, VGA_R7, \
                   VGA_G0, VGA_G1, VGA_G2, VGA_G3, VGA_G4, VGA_G5, VGA_G6, VGA_G7, \
                   VGA_B0, VGA_B1, VGA_B2, VGA_B3, VGA_B4, VGA_B5, VGA_B6, VGA_B7

enum class input_pin{
  BTN_INPUT,
  SW_INPUT,
  CLK_INPUT,
  NR_INPUT_PINS
};

enum class output_pin{
  NAIVE_LEDS_OUTPUT,
  RGB_LEDS_OUTPUT,
  SEG7_ENBS_OUTPUT,
  SEG7_SEGS_OUTPUT,
  VGA_OUTPUT,
  NR_OUTPUT_PINS
};

extern std::map<input_pin, int> input_map;
extern std::map<output_pin, int> output_map;

using namespace std;

void nvboard_bind_pin(vector <output_pin> &pin, void *signal);
void nvboard_bind_pin(vector <input_pin> &pin, void *signal);
void nvboard_bind_pin(output_pin pin, void *signal);
void nvboard_bind_pin(input_pin pin, void *signal);

#endif
