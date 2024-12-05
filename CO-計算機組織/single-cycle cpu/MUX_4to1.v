`timescale 1ns / 1ns

module  Mux_4to1(ALU_MUX_Out, in4, in3, in2, in1, in0, Signal);

input        in4, in3, in2, in1, in0;
input	    [2:0] Signal;

output       ALU_MUX_Out;

wire	     ALU_MUX_Out;

// data flow modeling
assign ALU_MUX_Out = ( Signal == 3'b000 ) ? in0 :   //    A & B
	         		 ( Signal == 3'b001 ) ? in1 :	//    A | B
	         		 ( Signal == 3'b010 ) ? in2 :	//    FA的sum --- add
	         		 ( Signal == 3'b110 ) ? in2 :	//    FA的sum --- sub
	         		 ( Signal == 3'b111 ) ? in3 :	//    SLT

	     	        	            		   0 ; 	


endmodule


