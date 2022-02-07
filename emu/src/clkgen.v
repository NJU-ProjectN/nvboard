module clkgen (
    input clkin,
    input rst,
    input clken,
    output reg clkout
);
    
    parameter clk_freq = 1000;
    parameter countlimit = 50000000/2/clk_freq;

    reg [31:0] clkcount;
    always @(posedge clkin) begin
        if(rst)begin
            clkcount <= 0;
            clkout <= 1'b0;
        end
        else begin
            if(clken)begin
                if(clkcount + 1 == countlimit)begin
                    clkcount <= 32'd0;
                    clkout <= ~clkout;
                end
                else begin
                    clkcount <= clkcount + 1;
                end
            end
        end
    end

endmodule
