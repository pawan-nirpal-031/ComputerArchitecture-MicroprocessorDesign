`timescale 1ns/1ns
`include "hello.v"

module hello_tb;
    reg a;
    wire b;

    hello test(a,b);

    initial begin
        $dumpfile("hello_tb.vcd");
        $dumpvars(0,hello_tb);

        a = 0; // initialize a
        #20 // wait for 20 ns to be able to distingush the output

        a = 1;
        #20 

        a = 0; // initialize a
        #20 // wait for 20 ns to be able to distingush the output

        $display("test done...");
    end
endmodule
