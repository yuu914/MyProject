module MUX_3to1( out, sel, in0, in1, in2 ) ;


input [31:0] in0, in1, in2 ;
input [1:0] sel ;
output [31:0] out ;

assign out = (sel == 00) ? in0 : (sel == 01) ? in1 : in2 ;



endmodule

































