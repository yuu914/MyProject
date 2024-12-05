module MUX32bits_2to1( out, sel, in0, in1 ) ;


input [31:0] in0, in1 ;
input sel ;
output [31:0] out ;

assign out = ( sel == 0 ) ? in0 : in1 ;


endmodule
