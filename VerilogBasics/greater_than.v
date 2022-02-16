module greater_than (A,B,F);
    input [1:0] A,B; // two bit inputs
    output F;
    // assign F = A>B  easier way to do our comparator logic 

    assign F = A[1]&~B[1] | A[1]&A[0]&~B[0] | A[0]&~B[1]&~B[0]; 
    /* 
    
    our greater_than logic design immplementation in a generic way we could have done simply A>B but here we learn a more generic way of doing any logic.

    our optimized greater_than expression, F = A1B1' + A1A0B0' + A0B1'B0'

    */


endmodule

/*
     for a two bit number we use range of indices ex  [1:0] for two bits 
     [3:0] for 4 bits, [7:0] for 8 bit inputs
     
     & for A.B 
     | for A + B

*/