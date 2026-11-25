#ifndef __NVBOARD_H__
#define __NVBOARD_H__

#include <pins.h>
#include <stdint.h>

// Initialize the board. `vga_clk_cycle` is the VGA sampling period in
// simulated cycles: 1 (the default) samples the VGA signals every cycle.
void nvboard_init(int vga_clk_cycle = 1);
void nvboard_quit();

// Connect `signal` to the given pins. `len` is the width of the signal in
// bits (1..64); a pin id must be in [0, NR_PINS) (see pins.h), and the pins
// are listed from MSB to LSB.
//
// A vector signal must be stored in an integer just wide enough for it: 1..8
// bits in 1 byte, 9..16 in 2, 17..32 in 4, 33..64 in 8 - the CData/SData/
// IData/QData types Verilator generates for the ports. Binding a vector to a
// narrower object makes NVBoard read and write past the signal.
void nvboard_bind_pin(void *signal, int len, ...);

// Update the state of every component, call it once per simulated cycle.
// It returns false once the user has closed the board window; the caller
// should stop the simulation and call nvboard_quit() afterwards.
bool nvboard_update();

#endif
