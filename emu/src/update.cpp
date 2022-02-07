#include <nboard.h>
#include <map>
#include <string>
#include <Vtop.h>
#include <vga.h>

static TOP_NAME dut;

extern std::map<input_pin, int> input_map;
extern std::map<output_pin, int> output_map;

static inline void update_input() {
  dut.clk = input_map[input_pin::CLK];
  uint32_t val_sw = 0;
  for (int i = 0; i < 8; i++) {
    val_sw |= input_map[input_pin(int(input_pin::SW0) + i)] << i;
  }
  dut.sw = val_sw;
}

static inline void update_output() {
  for (int i = 0; i < 16; i++) {
    output_map[output_pin(int(output_pin::LD0) + i)] = (dut.ledr >> i) & 1;
  }
  output_map[output_pin::VGA_CLK] = dut.VGA_CLK;
  output_map[output_pin::VGA_VSYNC] = dut.VGA_VSYNC;
  output_map[output_pin::VGA_HSYNC] = dut.VGA_HSYNC;
  output_map[output_pin::VGA_BLANK_N] = dut.VGA_BLANK_N;
  output_map[output_pin::VGA_R] = dut.VGA_R;
  output_map[output_pin::VGA_G] = dut.VGA_G;
  output_map[output_pin::VGA_B] = dut.VGA_B;
}

void dut_update() {
  update_input();
  dut.eval();
  update_output();
}
