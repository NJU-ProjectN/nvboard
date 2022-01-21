module top (
    input clk,
    input [7:0] sw,
    output [15:0] ledr,
    output VGA_CLK,
    output VGA_HSYNC,
    output VGA_VSYNC,
    output VGA_BLANK_N,
    output [7:0] VGA_R,
    output [7:0] VGA_G,
	output [7:0] VGA_B
);

test test1(
    .clk(clk),
    .sw(sw),
    .ledr(ledr)
);

assign VGA_CLK = clk;

vga_ctrl my_vga_ctrl(
    .pclk(clk),
    .reset(1'b0),
    .vga_data(24'hff00ff),
    /*
    .h_addr(h_addr),
    .v_addr(v_addr),
    */
    .hsync(VGA_HSYNC),
    .vsync(VGA_VSYNC),
    .valid(VGA_BLANK_N),
    .vga_r(VGA_R),
    .vga_g(VGA_G),
    .vga_b(VGA_B)
);


endmodule
