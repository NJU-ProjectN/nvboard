module seg(
  input clk,
  input rst,
  output reg[7:0] o_seg1,
  output reg[7:0] o_seg2,
  output reg[7:0] o_seg3,
  output reg[7:0] o_seg4,
  output reg[7:0] o_seg5,
  output reg[7:0] o_seg6,
  output reg[7:0] o_seg7,
  output reg[7:0] o_seg8
);

parameter segs = {8'b01100001, 8'b11011010, 8'b11110010, 8'b01100110, 8'b10110110, 8'b10111110, 8'b11100000, 8'b11111110};

parameter CLK_NUM = 500000;

reg [31:0] count;
reg [2:0] out_idx;
reg [2:0] offset;

always @(posedge clk) begin
  if(rst) begin count <= 0; out_idx <= 0; offset <= 0; end
  else begin
    if(count == CLK_NUM) begin offset <= offset + 1; end
    count <= (count == CLK_NUM) ? 0 : count + 1;
  end
end

assign o_seg1 = ~(segs>> (((offset + 3'd0)&7) * 8));
assign o_seg2 = ~(segs>> (((offset + 3'd1)&7) * 8));
assign o_seg3 = ~(segs>> (((offset + 3'd2)&7) * 8));
assign o_seg4 = ~(segs>> (((offset + 3'd3)&7) * 8));
assign o_seg5 = ~(segs>> (((offset + 3'd4)&7) * 8));
assign o_seg6 = ~(segs>> (((offset + 3'd5)&7) * 8));
assign o_seg7 = ~(segs>> (((offset + 3'd6)&7) * 8));
assign o_seg8 = ~(segs>> (((offset + 3'd7)&7) * 8));

endmodule
