module IF_ID( 
	input clk, 
	input reset, 
	input enable,
	input [31:0] pc_in, 
	input [31:0] inst_in,
	output reg [31:0] pc_out, 
	output reg [31:0] inst_out 
	);

	always @( posedge clk ) begin

		if ( reset ) begin
			pc_out <= 32'b0 ;
			inst_out <= 32'b0 ;
		end
		
		else if ( enable ) begin
			pc_out <= pc_in ;
			inst_out <= inst_in ;
		end
	end

endmodule


