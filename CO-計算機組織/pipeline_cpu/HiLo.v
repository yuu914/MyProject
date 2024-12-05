`timescale 1ns/1ns
module HiLo( clk, MULAns, HiOut, LoOut, reset, MULTU, MADDU );
  input clk ;
  input reset ;
  input [63:0] MULAns ;
  input MULTU;
  input MADDU;
  output reg [31:0] HiOut ;
  output reg [31:0] LoOut ;

  reg [63:0] HiLo ;


  always@( posedge clk or reset )
  begin
    if ( reset )
    begin
      HiLo <= 64'b0 ;
    end

    else if ( MULTU )
    begin
      HiLo <= MULAns;
    end

    else if ( MADDU )
    begin
      HiLo <= MULAns + HiLo;
    end
  end


  always@(posedge clk)
  begin
    HiOut <= HiLo[63:32]; 
    LoOut <= HiLo[31:0];
  end
endmodule