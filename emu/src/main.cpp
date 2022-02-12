#include <nvboard.h>
#include <Vtop.h>
#include <vga.h>

static TOP_NAME dut;

void nvboard_init();
void nvboard_quit();
void nvboard_update_all();

int main() {
  nvboard_bind_pin(output_pin::VGA_CLK,     &dut.VGA_CLK);
  nvboard_bind_pin(output_pin::VGA_VSYNC,   &dut.VGA_VSYNC);
  nvboard_bind_pin(output_pin::VGA_HSYNC,   &dut.VGA_HSYNC);
  nvboard_bind_pin(output_pin::VGA_BLANK_N, &dut.VGA_BLANK_N);

  vector<output_pin> vgaR_pins = {
    output_pin::VGA_R7, output_pin::VGA_R6, output_pin::VGA_R5, output_pin::VGA_R4,
    output_pin::VGA_R3, output_pin::VGA_R2, output_pin::VGA_R1, output_pin::VGA_R0
  };
  nvboard_bind_pin(vgaR_pins, &dut.VGA_R);

  vector<output_pin> vgaG_pins = {
    output_pin::VGA_G7, output_pin::VGA_G6, output_pin::VGA_G5, output_pin::VGA_G4,
    output_pin::VGA_G3, output_pin::VGA_G2, output_pin::VGA_G1, output_pin::VGA_G0
  };
  nvboard_bind_pin(vgaG_pins, &dut.VGA_G);

  vector<output_pin> vgaB_pins = {
    output_pin::VGA_B7, output_pin::VGA_B6, output_pin::VGA_B5, output_pin::VGA_B4,
    output_pin::VGA_B3, output_pin::VGA_B2, output_pin::VGA_B1, output_pin::VGA_B0
  };
  nvboard_bind_pin(vgaB_pins, &dut.VGA_B);

  vector<output_pin> led_pins = {
    output_pin::LD15, output_pin::LD14, output_pin::LD13, output_pin::LD12,
    output_pin::LD11, output_pin::LD10, output_pin::LD9, output_pin::LD8,
    output_pin::LD7, output_pin::LD6, output_pin::LD5, output_pin::LD4,
    output_pin::LD3, output_pin::LD2, output_pin::LD1, output_pin::LD0
  };
  nvboard_bind_pin(led_pins, &dut.ledr);

  vector<input_pin> sw_pins = {
    input_pin::SW7, input_pin::SW6, input_pin::SW5, input_pin::SW4,
    input_pin::SW3, input_pin::SW2, input_pin::SW1, input_pin::SW0
  };
  nvboard_bind_pin(sw_pins, &dut.sw);

  nvboard_init();

  while(1){
    nvboard_update_all();
    dut.clk = !dut.clk;
    dut.eval();
  }
}
