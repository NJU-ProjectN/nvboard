#include <interface.h>
#include <map>
#include <string>
#include <Vtop.h>

static TOP_NAME dut;

extern std::map<std::string, bool> input_map, output_map;

void dut_eval() {
  dut.eval();
}

void update_input() {
  dut.clk = input_map["btnc"];
  uint32_t val_sw = 0;
  for(int i = 0; i < 8; i++) {
    char sw_bit[8];
    sprintf(sw_bit, "sw%d", i);
    val_sw |= (uint32_t)input_map[sw_bit] << i;
  }
  dut.sw = val_sw;
}

void update_output() {
  for(int i = 0; i < 16; i++) {
    char ledr_bit[8];
    sprintf(ledr_bit, "ld%d", i);
    output_map[ledr_bit] = (dut.ledr >> i) & 1;
  }
}