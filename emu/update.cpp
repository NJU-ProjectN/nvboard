#include <nboard.h>
#include <map>
#include <string>
#include <topname.h>

extern std::map<input_pin, bool> input_map;
extern std::map<output_pin, bool> output_map;

void update_input(TOP_NAME *dut_ptr) {
  dut_ptr->clk = input_map[input_pin::BTNC];
  dut_ptr->reset = input_map[input_pin::RST];
}

void update_output(TOP_NAME *dut_ptr) {
  output_map[output_pin::LD0] = (dut_ptr->out >> 0) & 1;
  output_map[output_pin::LD1] = (dut_ptr->out >> 1) & 1;
  output_map[output_pin::LD2] = (dut_ptr->out >> 2) & 1;
  output_map[output_pin::LD3] = (dut_ptr->out >> 3) & 1;
  output_map[output_pin::AN0] = 1;
  output_map[output_pin::SEGA] = 0;
  output_map[output_pin::SEGB] = 1;
  output_map[output_pin::SEGC] = 0;
  output_map[output_pin::SEGD] = 0;
  output_map[output_pin::SEGE] = 0;
  output_map[output_pin::SEGF] = 0;
  output_map[output_pin::SEGG] = 0;
  output_map[output_pin::DECP] = 0;
}