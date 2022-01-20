#include <nboard.h>
#include <map>
#include <string>
#include <Vtop.h>

static TOP_NAME dut;

extern std::map<input_pin, int> input_map;
extern std::map<output_pin, int> output_map;

void dut_eval() {
  dut.eval();
}

#define CONCAT_TEMP(a, b) a ## b
#define CONCAT(a, b) CONCAT_TEMP(a, b)

void update_input() {
  dut.clk = input_map[input_pin::BTNC];
  uint32_t val_sw = 0;
  #define MAP_SW(id) val_sw |= (uint32_t)input_map[input_pin::CONCAT(SW, id)] << id;
  #define SW_NUM_LIST(_) _(0) _(1) _(2) _(3) _(4) _(5) _(6) _(7)
  SW_NUM_LIST(MAP_SW)
  dut.sw = val_sw;
}

void update_output() {
  #define MAP_LD(id) output_map[output_pin::CONCAT(LD, id)] = (dut.ledr >> id) & 1;
  #define LD_NUM_LIST(_) \
    _(0) _(1) _(2) _(3) _(4) _(5) _(6) _(7) _(8) _(9) _(10) _(11) _(12) _(13) _(14) _(15)
  LD_NUM_LIST(MAP_LD)
}