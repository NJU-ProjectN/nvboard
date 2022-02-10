#include <nboard.h>
#include <map>
#include <string>
#include <Vtop.h>
#include <vga.h>

static TOP_NAME dut;

void nvboard_init();
void nvboard_update(output_pin pin, int val);
void nvboard_quit();
int nvboard_getval(input_pin pin);
void nvboard_render();
int nvboard_event_handler();

int main() {
  nvboard_init();
  while(1){
        dut.clk = !dut.clk;
        uint32_t val_sw = 0;
        for(int i = 0; i < 8; i++) {
            val_sw |= nvboard_getval(input_pin(int(input_pin::SW0) + i)) << i;
        }
        dut.sw = val_sw;
        dut.eval();
        for (int i = 0; i < 16; i++) {
            nvboard_update(output_pin(int(output_pin::LD0) + i), (dut.ledr >> i) & 1);
        }
        nvboard_update(output_pin::VGA_CLK, dut.VGA_CLK);
        nvboard_update(output_pin::VGA_VSYNC, dut.VGA_VSYNC);
        nvboard_update(output_pin::VGA_HSYNC, dut.VGA_HSYNC);
        nvboard_update(output_pin::VGA_BLANK_N, dut.VGA_BLANK_N);
        nvboard_update(output_pin::VGA_R, dut.VGA_R);
        nvboard_update(output_pin::VGA_G, dut.VGA_G);
        nvboard_update(output_pin::VGA_B, dut.VGA_B);
 
        int ev = nvboard_event_handler();
        if(ev == -1) return 0;
  }

}