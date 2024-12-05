`timescale 1ns/1ns
module one_bit_ALU( A, B, Cin, ALU_Cout, ALU_MUX_Out, Signal, slt );        
  input A, B, Cin, slt;
  input [2:0] Signal;
  wire  in2, in1, in0;
  wire addorsub;
  output ALU_Cout, ALU_MUX_Out;

  parameter AND = 3'b000; // 0
  parameter OR  = 3'b001; // 1
  parameter ADD = 3'b010; // 2
  parameter SUB = 3'b110; // 6
  parameter SLT = 3'b111; // 7
  parameter SRL = 3'b011; // 3
  parameter ADDIU =  3'b100; // 4

  assign in0 = A & B;
  assign in1 = A | B;
  // in2 
  assign addorsub = (Signal == 3'b110) ? 1'b1 : 1'b0;    //  sub == 1
  FA FA(.a(A), .b((B ^ addorsub)), .c(Cin), .cout(ALU_Cout), .sum(in2));     // + or - result     here cout:c[i+1], sum:s[i] 接到多工器2的訊號線

  //assign in3 = ( Signal == 3'b111 ) ? ( A < B ? 1 : 0 ) : 0; // slt

  Mux_4to1 Mux_4to1(.ALU_MUX_Out(ALU_MUX_Out), .in4(ALU_Cout), .in3(slt), .in2(in2), .in1(in1), .in0(in0), .Signal(Signal));



endmodule