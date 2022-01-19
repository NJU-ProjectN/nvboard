#include <map>
#include <string>

#include <nboard.h>
#include <SDL2/SDL.h>

std::map<input_pin, bool> input_map;
std::map<output_pin, bool> output_map;

void init_input() {
  for (input_pin i = input_pin::BTNC; i <= input_pin::CLK; i = input_pin(int(i) + 1)) {
    input_map[i] = false;
  }
}

void init_output() {
  for (output_pin i = output_pin::LD0; i <= output_pin::DECP; i = output_pin(int(i) + 1)) {
    output_map[i] = false;
  }
}