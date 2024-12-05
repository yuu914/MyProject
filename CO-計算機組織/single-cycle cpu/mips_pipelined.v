 module mips_pipelined( clk, reset ) ;


input clk, reset ;

wire [31:0] pc_in, PCout, pc_add, ins_memout, IFID_pc, IFID_inst, regfile_rd1out, regfile_rd2out, SignEXTNDout, UNEXTNDout, extendmuxout, IDEXout,
			IDEX_Rd1out, IDEX_Rd2out, IDEX_extndout, ALUsrc_muxout, aluout, entndout_shifted, addout, hiout, loout, WD, pcsrcout, jumpaddr, jumpaddr1, jumpaddr2 ;
wire [63:0] multout ;
wire [4:0] rs, rt, rd, shamt, shamtout, rtout, rdout, regdst_muxout ;
wire [5:0] functout ;
wire [1:0] aluctl_muxsel, IDEX_ctlbrnout ; // controlbranch
wire [2:0] ALUcontrol_out ;
wire [25:0] joffset ;
wire branchout ;

// Control
wire regdst, ALUsrc, Memtoreg, Regwrite, MemRead, MemWrite, Branch, Jump, extendselect, MULTUsel, MADDUsel ;
wire [1:0] ALUop ;

//ID_EX
wire [1:0] WBreg, WBreg1, WBreg2, WBreg3 ;
wire [2:0] MEMreg, MEMreg1, MEMreg2 ;
wire [3:0] EXreg, EXreg1 ;
wire IDEX_jump, IDEX_jump1 ;
assign WBreg = { Regwrite, Memtoreg } ;
assign MEMreg = { MemRead, MemWrite, Branch } ;
assign EXreg = { regdst, ALUop, ALUsrc } ; // ALUop => 2bits

//EX_MEM
wire [31:0] EXMEM_aluin, EXMEM_aluout, EXMEM_Rd2out, EXMEM_addout ;
wire [4:0] EXMEM_WNout ;
wire EXMEM_ZEROin, EXMEM_ZEROout ;

//MEM_WB
wire [31:0] MEMWB_RDin, MEMWB_RDout, MEMWB_addrout ;
wire [4:0] MEMWB_WNout ;

wire [5:0] op, funct ; 
wire [15:0] offset ; // immediate


assign op = IFID_inst[31:26] ;
assign rs = IFID_inst[25:21] ;
assign rt = IFID_inst[20:16] ;
assign rd = IFID_inst[15:11] ;
assign shamt = IFID_inst[10:6] ;
assign funct = IFID_inst[5:0] ;
assign offset = IFID_inst[15:0] ;
assign entndout_shifted = IDEX_extndout << 2 ;
assign joffset = IFID_inst[25:0] ;
assign jumpaddr = { IFID_pc[31:28], joffset << 2 } ;



reg32 pc( .clk(clk), .rst(reset), .en_reg(1'b1), .d_in(pc_in), .d_out(PCout) );

add32 pcadd4( .result(pc_add), .a(PCout), .b(32'd4) ); // PC = PC + 4

Memory instruction_memory( .rd(ins_memout), .MemRead(1'b1), .MemWrite(1'b0), .wd(32'd0), .addr(PCout), .clk(clk) ) ;


IF_ID IFID( .clk(clk), .reset(reset), .enable(1'b1), .pc_in(pc_add), .inst_in(ins_memout), .pc_out(IFID_pc), .inst_out(IFID_inst) ) ;

reg_file register_file( .RD1(regfile_rd1out), .RD2(regfile_rd2out), .RegWrite(WBreg3[1]), .RN1(rs), .RN2(rt), .WN(MEMWB_WNout), .WD(WD), .clk(clk) ) ; // wdout-input

sign_extend signextend( .immed_in(offset), .ext_immed_out(SignEXTNDout) );


unsign_extend unsignextend( .immed_in(offset), .ext_immed_out(UNEXTNDout) ) ;

MUX32bits_2to1 EXTNDMUX( .out(extendmuxout), .sel(extendselect), .in0(UNEXTNDout), .in1(SignEXTNDout) ) ;

control_single control( .regdst(regdst), .ALUsrc(ALUsrc), .Branch(Branch), .Memread(MemRead), .Memwrite(MemWrite), .regwrite(Regwrite),
				.Memtoreg(Memtoreg), .Jump(Jump), .extendselect(extendselect), .ALUop(ALUop), .op(op) ) ;

ID_EX IDEX( .clk(clk), .reset(reset), .enable(1'b1), .WBin(WBreg), .MEMORYin(MEMreg), .EXin(EXreg),.IDEXin(IFID_pc), .RD1in(regfile_rd1out), .RD2in(regfile_rd2out), .Extndin(extendmuxout),
		    .shamtin(shamt), .RTin(rt), .RDin(rd), .functin(funct), .jumpaddrin(jumpaddr), .jumpin(Jump), .WBout(WBreg1), .MEMORYout(MEMreg1), .EXout(EXreg1), .IDEXout(IDEXout), 
			.RD1out(IDEX_Rd1out), .RD2out(IDEX_Rd2out), .Extndout(IDEX_extndout), .RTout(rtout), .RDout(rdout), .shamtout(shamtout), .functout(functout), .jumpout(IDEX_jump), .jumpaddrout(jumpaddr1) ) ;

MUX5bits_2to1 regdstmux( .out(regdst_muxout), .sel(EXreg1[3]), .in0(rtout), .in1(rdout) ) ;


alu_ctl ALUcontrol(.ALUOp(EXreg1[2:1]), .Funct(functout), .MULTU(MULTUsel), .MADDU(MADDUsel), .ALUOperation(ALUcontrol_out), .sel(aluctl_muxsel));

MUX32bits_2to1 ALUsrcMux( .out(ALUsrc_muxout), .sel(EXreg1[0]), .in0(IDEX_Rd2out), .in1(IDEX_extndout) ) ;


ALU alu( .dataA(IDEX_Rd1out), .dataB(ALUsrc_muxout), .Signal(ALUcontrol_out), .dataOut(aluout), .reset(reset), .zero(EXMEM_ZEROin), .shamtin(shamtout));


add32 add( .result(addout), .a(entndout_shifted), .b(IDEXout) ); // entndout_shifted = IDEX_extndout << 2 ;


Multiplier multiplier( .clk(clk), .dataA(IDEX_Rd1out), .dataB(IDEX_Rd2out), .SignaltoMUL(MULTUsel), .MULAns(multout), .reset(reset));

HiLo hilo( .clk(clk), .MULAns(multout), .HiOut(hiout), .LoOut(loout), .reset(reset), .MULTU(MULTUsel), .MADDU(MADDUsel) );

MUX_3to1 alu_totalmux( .out(EXMEM_aluin), .sel(aluctl_muxsel), .in0(aluout), .in1(hiout), .in2(loout) ) ;

EX_MEM EXMEM( .clk(clk), .reset(reset), .enable(1'b1), .WBin(WBreg1), .MEMORYin(MEMreg1), .totalALUin(EXMEM_aluin), .RD2in(IDEX_Rd2out), .addin(addout), .WNin(regdst_muxout), .ZEROin(EXMEM_ZEROin), .jumpin(IDEX_jump), .jumpaddrin(jumpaddr1),
			  .WBout(WBreg2), .MEMORYout(MEMreg2), .totalALUout(EXMEM_aluout), .RD2out(EXMEM_Rd2out), .addout(EXMEM_addout), .WNout(EXMEM_WNout), .ZEROout(EXMEM_ZEROout), .jumpout(IDEX_jump1), .jumpaddrout(jumpaddr2) ) ;

Memory Data_memory( .rd(MEMWB_RDin), .MemRead(MEMreg2[2]), .MemWrite(MEMreg2[1]), .wd(EXMEM_Rd2out), .addr(EXMEM_aluout), .clk(clk) ) ;

and( branchout, EXMEM_ZEROout, MEMreg2[0] ) ;

MEM_WB MEMWB( .clk(clk), .reset(reset), .enable(1'b1), .WBin(WBreg2), .DMin(MEMWB_RDin), .ADDRin(EXMEM_aluout), .WNin(EXMEM_WNout), .WBout(WBreg3), .DMout(MEMWB_RDout), .ADDRout(MEMWB_addrout), .WNout(MEMWB_WNout) ) ;

MUX32bits_2to1 memtoregmux( .out(WD), .sel(WBreg3[0]), .in0(MEMWB_RDout), .in1(MEMWB_addrout) ) ; // wd-output

MUX32bits_2to1 pcsrcmux( .out(pcsrcout), .sel(branchout), .in0(pc_add), .in1(EXMEM_addout) ) ;

MUX32bits_2to1 jumpmux( .out(pc_in), .sel(IDEX_jump1), .in0(pcsrcout), .in1(jumpaddr2) ) ;

endmodule










