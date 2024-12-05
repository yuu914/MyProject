

module control_single( regdst, ALUsrc, Branch, Memread, Memwrite, regwrite, Memtoreg, Jump, extendselect, ALUop, op ) ;


	input [5:0] op ;
	output reg regdst, ALUsrc, Branch, Memread, Memwrite, regwrite, Memtoreg, Jump, extendselect ;
	output reg [1:0] ALUop; 

	// opcode
    parameter R_type = 6'd0; //add,sub,slt,and,or,srl
    parameter lw = 6'd35;
    parameter sw = 6'd43;
    parameter beq = 6'd4;
	parameter j = 6'd2;
	parameter maddu = 6'd28;
	parameter addiu = 6'd9;


	always @( op ) begin

		case( op )
			R_type :
			begin
				regdst = 1'b1 ; ALUsrc = 1'b0 ; ALUop = 2'b10 ; Branch = 1'b0 ; 
				Memread = 1'b0 ; Memwrite = 1'b0 ; regwrite = 1'b1 ; 
				Memtoreg = 1'b1 ; Jump = 1'b0 ; extendselect = 1'b1 ; 
			end

			lw :
			begin
				regdst = 1'b0 ; ALUsrc = 1'b1 ; ALUop = 2'b00 ; Branch = 1'b0 ; 
				Memread = 1'b1 ; Memwrite = 1'b0 ; regwrite = 1'b1 ; 
				Memtoreg = 1'b0 ; Jump = 1'b0 ; extendselect = 1'b0 ; 
			end

			sw :
			begin
				regdst = 1'bx ; ALUsrc = 1'b1 ; ALUop = 2'b00 ; Branch = 1'b0 ; 
				Memread = 1'b0 ; Memwrite = 1'b1 ; regwrite = 1'b0 ; 
				Memtoreg = 1'bx ; Jump = 1'b0 ; extendselect = 1'b0 ; 
			end

			beq : 
			begin
				regdst = 1'bx ; ALUsrc = 1'b0 ; ALUop = 2'b01 ; Branch = 1'b1 ; 
				Memread = 1'b0 ; Memwrite = 1'b0 ; regwrite = 1'b0 ; 
				Memtoreg = 1'bx ; Jump = 1'b0 ; extendselect = 1'b0 ; 
			end
			
			j:
			begin
				regdst = 1'bx ; ALUsrc = 1'b0 ; ALUop = 2'b01 ; Branch = 1'b1 ; 
				Memread = 1'b0 ; Memwrite = 1'b0 ; regwrite = 1'b0 ; 
				Memtoreg = 1'bx ; Jump = 1'b1 ; extendselect = 1'b0 ;
			end


	        addiu :
	        begin
				regdst = 1'b0; ALUsrc = 1'b1; Memtoreg = 1'b0; regwrite = 1'b1; Memread = 1'b0; 
				Memwrite = 1'b0; Branch = 1'b0; Jump = 1'b0; ALUop = 2'b00; extendselect = 1'b0;
            end


			maddu:
			begin
				regdst = 1'b1 ; ALUsrc = 1'b0 ; ALUop = 2'b10 ; Branch = 1'b0 ; 
				Memread = 1'b0 ; Memwrite = 1'b0 ; regwrite = 1'b0 ; 
				Memtoreg = 1'b1 ; Jump = 1'b0 ; extendselect = 1'b1 ; 
			end
			
			default  // nop
		    begin
				$display("control_pipelined unimplemented opcode %d", op);
				regdst = 1'bx ; ALUsrc = 1'bx ; ALUop = 2'bxx ; Branch = 1'bx ; 
				Memread = 1'bx ; Memwrite = 1'bx ; regwrite = 1'bx ; 
				Memtoreg = 1'bx ; Jump = 1'bx ; extendselect = 1'bx ; 
		    end
		endcase
	end
endmodule
