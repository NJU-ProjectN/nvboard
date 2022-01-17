#include <map>
#include <string>

#include <nboard.h>
#include <SDL2/SDL.h>

std::map<std::string, bool> input_map, output_map;
std::map<std::string, bool> prev_output_map, prev_input_map;

std::string input_pins[] = {
  CLK_INPUT, RST_INPUT, BTN_INPUT, SWITCH_INPUT
};

const int input_size = sizeof(input_pins) / sizeof(std::string);

std::string output_pins[] = {
  NAIVE_LEDS, RGB_LEDS, SEG7_ENBS, SEG7_SEGS
};

const int output_size = sizeof(output_pins) / sizeof(std::string);

void init_input() {
  for (auto str : input_pins) {
    input_map[str] = false;
    prev_input_map[str] = false;
  }
}

void init_output() {
  for (auto str : output_pins) {
    output_map[str] = false;
    prev_output_map[str] = false;
  }
}