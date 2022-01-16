#include <nboard.h>
#include <map>
#include <string>
#include <topname.h>

extern std::map<std::string, bool> input_map, output_map;

void update_input(TOP_NAME *dut_ptr) {
  dut_ptr->clk = input_map["btnc"];
  dut_ptr->reset = input_map["rst"];
}

void update_output(TOP_NAME *dut_ptr) {
  output_map["ld0"] = (dut_ptr->out >> 0) & 1;
  output_map["ld1"] = (dut_ptr->out >> 1) & 1;
  output_map["ld2"] = (dut_ptr->out >> 2) & 1;
  output_map["ld3"] = (dut_ptr->out >> 3) & 1;
}