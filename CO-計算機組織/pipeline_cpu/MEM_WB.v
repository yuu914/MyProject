module MEM_WB(
	input clk, 
	input reset, 
	input enable,
	input [1:0] WBin,
	input [31:0] DMin, 
	input [31:0] ADDRin,
	input [4:0] WNin, 

	output reg [1:0] WBout,
	output reg [31:0] DMout, 
	output reg [31:0] ADDRout,
	output reg [4:0] WNout
	);

	always @( posedge clk ) begin

		if ( reset ) begin
			WBout <= 2'b0 ;
			DMout <= 32'b0 ;
			ADDRout <= 32'b0 ;
			WNout <= 5'b0 ;
		end
		
		else if ( enable ) begin
			WBout <= WBin ;
			DMout <= DMin ;
			ADDRout <= ADDRin ;
			WNout <= WNin ;
		end
	end
endmodule





