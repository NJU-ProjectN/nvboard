#include <nvboard.h>
#include <Vtop.h>
#include <vga.h>

static TOP_NAME dut;

void nvboard_init();
void nvboard_quit();
void nvboard_update();
void nvboard_bind_pins(Vtop* top);

int main() {

  nvboard_bind_all_pins(&dut);
  nvboard_init();

  while(1){
    nvboard_update();
    dut.clk = !dut.clk;
    dut.eval();
  }
}
