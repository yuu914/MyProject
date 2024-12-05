/*
	Title:	ALU Control Unit
	Author: Garfield (Computer System and Architecture Lab, ICE, CYCU)
	Input Port
		1. ALUOp: 控制alu是要用+還是-或是其他指令
		2. Funct: 如果是其他指令則用這邊6碼判斷
	Output Port
		1. ALUOperation: 最後解碼完成之指令
*/

module alu_ctl(ALUOp, Funct, MULTU, MADDU, ALUOperation, sel);
    input [1:0] ALUOp;
    input [5:0] Funct;
    output [2:0] ALUOperation;
    reg    [2:0] ALUOperation;
	output reg [1:0] sel ;
	output reg MULTU, MADDU ;

    // symbolic constants for instruction function code
    parameter F_add = 6'd32;
    parameter F_sub = 6'd34;
    parameter F_and = 6'd36;
    parameter F_or  = 6'd37;
    parameter F_slt = 6'd42;
	
	parameter F_SRL = 6'd2;
    parameter F_ADDIU = 6'd9;


	parameter F_MULTU= 6'd25;
    parameter F_MADDU = 6'd28;


	parameter F_MFHI= 6'd16;
	parameter F_MFLO= 6'd18;

    parameter F_NOP = 6'd0;

    // symbolic constants for ALU Operations
    parameter ALU_add = 3'b010;
    parameter ALU_sub = 3'b110;
    parameter ALU_and = 3'b000;
    parameter ALU_or  = 3'b001;
    parameter ALU_slt = 3'b111;
    parameter ALU_srl = 3'b011;
    parameter ALU_addiu = 3'b100;


    always @(ALUOp or Funct)
    begin

        // init signal
		sel = 2'b00 ;
		MULTU = 1'b0 ;
        MADDU = 1'b0 ;

        case (ALUOp) 
            2'b00 : ALUOperation = ALU_add; // lw sw
            2'b01 : ALUOperation = ALU_sub; // beq
            2'b10 : case (Funct)     
                        F_add : ALUOperation = ALU_add;
                        F_sub : ALUOperation = ALU_sub;
                        F_and : ALUOperation = ALU_and;
                        F_or  : ALUOperation = ALU_or;
                        F_slt : ALUOperation = ALU_slt;
						F_SRL : ALUOperation = ALU_srl;
						F_MULTU : MULTU = 1'b1 ;
                        F_MADDU : MADDU = 1'b1 ;
						F_MFHI : sel = 2'b01 ;
						F_MFLO : sel = 2'b10 ;
                        F_ADDIU : ALUOperation = ALU_addiu;
                        default ALUOperation = 3'bxxx;
                    endcase
            default ALUOperation = 3'bxxx; // nop
        endcase
    end
endmodule

