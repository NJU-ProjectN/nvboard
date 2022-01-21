#include <constrs.h>
#include <map>
#include <string>

std::map<input_pin, int> input_map;
std::map<output_pin, int> output_map;

void init_input() {
  for (input_pin i = input_pin::BTNC; i < input_pin::NR_INPUT_PINS; i = input_pin(int(i) + 1)) {
    input_map[i] = false;
  }
}

void init_output() {
  for (output_pin i = output_pin::LD0; i < output_pin::NR_OUTPUT_PINS; i = output_pin(int(i) + 1)) {
    output_map[i] = false;
  }
}