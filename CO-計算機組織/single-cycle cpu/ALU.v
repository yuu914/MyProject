`timescale 1ns/1ns
module ALU( dataA, dataB, Signal, dataOut, reset, zero, shamtin);


// signal: control給ALU的
// reset:  外部給的

input reset ;
input [31:0] dataA ;
input [31:0] dataB ;
input [2:0] Signal ;
output [31:0] dataOut ;
output zero;            // zero signal --- beq instruction
input [4:0] shamtin;    // shift amount


wire [31:0] temp ;
wire [31:0] temp_shifter;
wire [31:0] temp_addiu;
wire [31:0] cout;
wire slt;
wire cin;



parameter AND = 3'b000; // 0
parameter OR  = 3'b001; // 1
parameter ADD = 3'b010; // 2
parameter SUB = 3'b110; // 6
parameter SLT = 3'b111; // 7
parameter SRL = 3'b011; // 3
parameter ADDIU =  3'b100; // 4



		// 先判斷cin是什做甚麼事(加or減 others不用檢查 只要檢查加減就好 其他跟加的值相同)
		assign cin = ( Signal == 3'b110 ) ? 1'b1 : 1'b0 ;  // signal is sub, cin is 1 ( ref: 3-1 p16 )
        assign slt = ( dataA > dataB ) ? 1'b1 :1'b0 ;


		barrel_shifter shifter( .dataA(dataA), .dataB(shamtin), .SignaltoSHT(Signal), .dataOut(temp_shifter), .reset(reset) ); // srl
        add32 addiu( .a(dataA), .b(dataB), .result(temp_addiu) ); // addiu

     	one_bit_ALU one_bit_ALU0(.A(dataA[0]), .B(dataB[0]), .Cin(cin), .ALU_Cout(cout[0]), .ALU_MUX_Out(temp[0]), .Signal(Signal), .slt(slt));
     	one_bit_ALU one_bit_ALU1(.A(dataA[1]), .B(dataB[1]), .Cin(cout[0]), .ALU_Cout(cout[1]), .ALU_MUX_Out(temp[1]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU2(.A(dataA[2]), .B(dataB[2]), .Cin(cout[1]), .ALU_Cout(cout[2]), .ALU_MUX_Out(temp[2]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU3(.A(dataA[3]), .B(dataB[3]), .Cin(cout[2]), .ALU_Cout(cout[3]), .ALU_MUX_Out(temp[3]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU4(.A(dataA[4]), .B(dataB[4]), .Cin(cout[3]), .ALU_Cout(cout[4]), .ALU_MUX_Out(temp[4]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU5(.A(dataA[5]), .B(dataB[5]), .Cin(cout[4]), .ALU_Cout(cout[5]), .ALU_MUX_Out(temp[5]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU6(.A(dataA[6]), .B(dataB[6]), .Cin(cout[5]), .ALU_Cout(cout[6]), .ALU_MUX_Out(temp[6]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU7(.A(dataA[7]), .B(dataB[7]), .Cin(cout[6]), .ALU_Cout(cout[7]), .ALU_MUX_Out(temp[7]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU8(.A(dataA[8]), .B(dataB[8]), .Cin(cout[7]), .ALU_Cout(cout[8]), .ALU_MUX_Out(temp[8]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU9(.A(dataA[9]), .B(dataB[9]), .Cin(cout[8]), .ALU_Cout(cout[9]), .ALU_MUX_Out(temp[9]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU10(.A(dataA[10]), .B(dataB[10]), .Cin(cout[9]), .ALU_Cout(cout[10]), .ALU_MUX_Out(temp[10]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU11(.A(dataA[11]), .B(dataB[11]), .Cin(cout[10]), .ALU_Cout(cout[11]), .ALU_MUX_Out(temp[11]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU12(.A(dataA[12]), .B(dataB[12]), .Cin(cout[11]), .ALU_Cout(cout[12]), .ALU_MUX_Out(temp[12]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU13(.A(dataA[13]), .B(dataB[13]), .Cin(cout[12]), .ALU_Cout(cout[13]), .ALU_MUX_Out(temp[13]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU14(.A(dataA[14]), .B(dataB[14]), .Cin(cout[13]), .ALU_Cout(cout[14]), .ALU_MUX_Out(temp[14]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU15(.A(dataA[15]), .B(dataB[15]), .Cin(cout[14]), .ALU_Cout(cout[15]), .ALU_MUX_Out(temp[15]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU16(.A(dataA[16]), .B(dataB[16]), .Cin(cout[15]), .ALU_Cout(cout[16]), .ALU_MUX_Out(temp[16]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU17(.A(dataA[17]), .B(dataB[17]), .Cin(cout[16]), .ALU_Cout(cout[17]), .ALU_MUX_Out(temp[17]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU18(.A(dataA[18]), .B(dataB[18]), .Cin(cout[17]), .ALU_Cout(cout[18]), .ALU_MUX_Out(temp[18]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU19(.A(dataA[19]), .B(dataB[19]), .Cin(cout[18]), .ALU_Cout(cout[19]), .ALU_MUX_Out(temp[19]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU20(.A(dataA[20]), .B(dataB[20]), .Cin(cout[19]), .ALU_Cout(cout[20]), .ALU_MUX_Out(temp[20]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU21(.A(dataA[21]), .B(dataB[21]), .Cin(cout[20]), .ALU_Cout(cout[21]), .ALU_MUX_Out(temp[21]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU22(.A(dataA[22]), .B(dataB[22]), .Cin(cout[21]), .ALU_Cout(cout[22]), .ALU_MUX_Out(temp[22]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU23(.A(dataA[23]), .B(dataB[23]), .Cin(cout[22]), .ALU_Cout(cout[23]), .ALU_MUX_Out(temp[23]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU24(.A(dataA[24]), .B(dataB[24]), .Cin(cout[23]), .ALU_Cout(cout[24]), .ALU_MUX_Out(temp[24]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU25(.A(dataA[25]), .B(dataB[25]), .Cin(cout[24]), .ALU_Cout(cout[25]), .ALU_MUX_Out(temp[25]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU26(.A(dataA[26]), .B(dataB[26]), .Cin(cout[25]), .ALU_Cout(cout[26]), .ALU_MUX_Out(temp[26]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU27(.A(dataA[27]), .B(dataB[27]), .Cin(cout[26]), .ALU_Cout(cout[27]), .ALU_MUX_Out(temp[27]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU28(.A(dataA[28]), .B(dataB[28]), .Cin(cout[27]), .ALU_Cout(cout[28]), .ALU_MUX_Out(temp[28]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU29(.A(dataA[29]), .B(dataB[29]), .Cin(cout[28]), .ALU_Cout(cout[29]), .ALU_MUX_Out(temp[29]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU30(.A(dataA[30]), .B(dataB[30]), .Cin(cout[29]), .ALU_Cout(cout[30]), .ALU_MUX_Out(temp[30]), .Signal(Signal), .slt(1'b0));
     	one_bit_ALU one_bit_ALU31(.A(dataA[31]), .B(dataB[31]), .Cin(cout[30]), .ALU_Cout(cout[31]), .ALU_MUX_Out(temp[31]), .Signal(Signal), .slt(1'b0));


		assign dataOut = ( reset == 1'b0 ) ? ( Signal == 3'b011 ) ? temp_shifter : ( Signal == 3'b100 ) ? temp_addiu : temp : 32'b0 ; 
		assign zero = ( dataOut == 32'b0 ) ? 1 : 0 ;      // zero signal --- check beq instruction


		

endmodule