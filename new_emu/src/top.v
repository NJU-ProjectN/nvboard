module top (
    input clk,
    input [7:0] sw,
    output [15:0] ledr
);

test test1(
    .clk(clk),
    .sw(sw),
    .ledr(ledr)
);



endmodule
