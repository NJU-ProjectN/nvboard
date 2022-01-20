#include <nboard.h>
#include <map>
#include <string>
#include <Vtop.h>

static TOP_NAME dut;

extern std::map<input_pin, int> input_map;
extern std::map<output_pin, int> output_map;

static inline void update_input() {
  dut.clk = input_map[input_pin::BTNC];
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
}

void dut_update() {
  update_input();
  dut.eval();
  update_output();
}
