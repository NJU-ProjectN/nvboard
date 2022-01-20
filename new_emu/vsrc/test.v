module test (
    input clk,
    input [7:0] sw,
    output [15:0] ledr
);

reg [7:0] cnt;

always @(posedge clk) begin
    cnt <= cnt + 8'd1;
end

assign ledr = {cnt, sw};

endmodule
