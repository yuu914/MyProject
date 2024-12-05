`timescale 1ns/1ns
module MUX_2to1(
  input a, 
  input b, 
  input sel, 
  output out 
);
assign out = (sel==1) ? a : b ;
endmodule

