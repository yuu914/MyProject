module MUX5bits_2to1( out, sel, in0, in1 ) ;


input [4:0] in0, in1 ;
input sel ;
output [4:0] out ;

assign out = ( sel == 0 ) ? in0 : in1 ;



endmodule