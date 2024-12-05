`timescale 1ns / 1ns

module  FA(a, b, c, cout, sum);

    input        a,b,c;

    output       cout, sum;

    wire	     e1, e2, e3, e4;

    // gate level modeling

    xor(e1,a,b);
    xor(sum,e1,c);
    or(e2,a,b);
    and(e3,e2,c);
    and(e4,a,b);
    or(cout,e3,e4);



endmodule


