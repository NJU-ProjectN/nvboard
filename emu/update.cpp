#include <nboard.h>
#include <map>
#include <string>

TOP_NAME dut;

extern std::map<std::string, bool> input_map, output_map;

void dut_eval() {
  dut.eval();
}

void update_input() {
  dut.clk = input_map["btnc"];
  dut.reset = input_map["rst"];
}

void update_output() {
  output_map["ld0"] = (dut.out >> 0) & 1;
  output_map["ld1"] = (dut.out >> 1) & 1;
  output_map["ld2"] = (dut.out >> 2) & 1;
  output_map["ld3"] = (dut.out >> 3) & 1;
}