`timescale 1ns/1ns
`include "HelloWorld.v"

module HelloWorld_Tb;
    reg A;
    wire B;

    HelloWorld exect(A,B);

    initial begin
        $dumpfile("HelloWorld_Tb.vcd");
        $dumpvars(0,HelloWorld_Tb);

        A = 0;
        #20;
        A = 1;
        #20;
        A = 0;
        #20;

        $display("First HDL code ran");


    end

endmodule