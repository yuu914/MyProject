module ID_EX(
	input clk, 
	input reset, 
	input enable,
	input [1:0] WBin, 
	input [2:0] MEMORYin,
	input [3:0] EXin,
	input [31:0] IDEXin, 
	input [31:0] RD1in, 
	input [31:0] RD2in, 
	input [31:0] Extndin,
	input [4:0] shamtin, 
	input [4:0] RTin, 
	input [4:0] RDin,
	input [5:0] functin,
	input [31:0] jumpaddrin,
	input jumpin,

	output reg [1:0] WBout, 
	output reg [2:0] MEMORYout,
	output reg [3:0] EXout,
	output reg [31:0] IDEXout, 
	output reg [31:0] RD1out, 
	output reg [31:0] RD2out, 
	output reg [31:0] Extndout,
	output reg [4:0] RTout, 
	output reg [4:0] RDout, 
	output reg [4:0] shamtout, 
	output reg [5:0] functout,
	output reg jumpout,
	output reg [31:0] jumpaddrout
	);

	always @( posedge clk ) begin

		if ( reset ) begin
			jumpaddrout <= 32'b0 ;
			jumpout <= 1'b0 ;
			WBout <= 2'b0 ;
			MEMORYout <= 3'b0 ;
			EXout <= 4'b0 ;
			IDEXout <= 32'b0 ;
			RD1out <= 32'b0 ;
			RD2out <= 32'b0 ;
			Extndout <= 32'b0 ;
			shamtout <= 5'b0 ;
			RTout <= 5'b0 ;
			RDout <= 5'b0 ;
			functout <= 6'b0 ;
		end
		
		else if ( enable ) begin
			jumpaddrout <= jumpaddrin ;
			jumpout <= jumpin ;
			WBout <= WBin ;
			MEMORYout <= MEMORYin ;
			EXout <= EXin ;
			IDEXout <= IDEXin ;
			RD1out <= RD1in ;
			RD2out <= RD2in ;
			Extndout <= Extndin ;
			shamtout <= shamtin ;
			RTout <= RTin ;
			RDout <= RDin ;
			functout <= functin ;
		end
	end
endmodule



