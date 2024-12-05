module EX_MEM( 
	input clk, 
	input reset, 
	input enable,
	input [1:0] WBin,
	input [2:0] MEMORYin,
	input [31:0] totalALUin, 
	input [31:0] RD2in, 
	input [31:0] addin, 
	input [4:0] WNin,
	input ZEROin, 
	input jumpin,
	input [31:0] jumpaddrin,

	output reg [1:0] WBout,
	output reg [2:0] MEMORYout,
	output reg [31:0] totalALUout, 
	output reg [31:0] RD2out, 
	output reg [31:0] addout, 
	output reg [4:0] WNout,
	output reg ZEROout, 
	output reg jumpout, 
	output reg [31:0] jumpaddrout
	);

	always @( posedge clk ) begin

		if ( reset ) begin
			jumpaddrout <= 32'b0 ;
			jumpout <= 1'b0 ;
			ZEROout <= 1'b0 ;
			WBout <= 2'b0 ;
			MEMORYout <= 3'b0 ;
			totalALUout <= 32'b0 ;
			RD2out <= 32'b0 ;
			addout <= 32'b0 ;
			WNout <= 5'b0 ;
		end
		
		else if ( enable ) begin
			jumpaddrout <= jumpaddrin ;
			jumpout <= jumpin ;
			ZEROout <= ZEROin ;
			WBout <= WBin ;
			MEMORYout <= MEMORYin ;
			totalALUout <= totalALUin ;
			RD2out <= RD2in ;
			addout <= addin ;
			WNout <= WNin ;
		end
	end
endmodule



