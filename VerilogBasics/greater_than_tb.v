`timescale 1ns/1ns
`include "greater_than.v"

module greater_than_tb;
    reg[1:0] A,B;
    wire F;

    greater_than unit_test(A,B,F);

    initial begin
        $dumpfile("greater_than_tb.vcd");
        $dumpvars(0,greater_than_tb);

        {A,B} = 4'd0; #20; // all cases in truth table
        {A,B} = 4'd1; #20;
        {A,B} = 4'd2; #20;
        {A,B} = 4'd3; #20;
        {A,B} = 4'd4; #20;
        {A,B} = 4'd5; #20;
        {A,B} = 4'd6; #20;
        {A,B} = 4'd7; #20;
        {A,B} = 4'd8; #20;
        {A,B} = 4'd9; #20;
        {A,B} = 4'd10; #20;
        {A,B} = 4'd11; #20;
        {A,B} = 4'd12; #20;
        {A,B} = 4'd13; #20;
        {A,B} = 4'd14; #20;
        {A,B} = 4'd15; #20;
        $display("Test finished...");
    end

endmodule

/*
    {A,B} is a vector notaion it means we treat A (2 bit number) and B(also a 2 bit number) as one sinlge 
    4 bit number 4' means we have a 4 bits as result of vectorizing them d means we specify in deicmal and specify the value 0 in decimal we could do it in binary by using the prefix b followed by 0000 ( since we have 4 bit vector and our value is 0 ) so 4'd0 in decimal, 4'b0000 in binary and #20 is 20 ns delay
*/