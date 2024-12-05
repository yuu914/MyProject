module Memory( rd, MemRead, MemWrite, wd, addr, clk ) ;

	input clk, MemRead, MemWrite;
	input[31:0] addr, wd;
	output[31:0] rd;
	reg[31:0] rd;
	
	// Memory size: 1KB.
	reg [7:0] mem_array [0:1023];
	
	// 以下為至指定記憶體位置讀取資料
	always @( MemRead or mem_array[addr] or mem_array[addr+1] or mem_array[addr+2] or mem_array[addr+3] ) begin
		if ( MemRead == 1'b1 ) begin
			rd[7:0] = mem_array[addr];
			rd[15:8] = mem_array[addr+1];
			rd[23:16] = mem_array[addr+2];
			rd[31:24] = mem_array[addr+3];
			$display( "%d, reading data: Mem[%d] => %d", $time/10, addr, rd );
		end
		else rd = 32'hxxxxxxxx;
	end
	
	// 以下將資料寫入指定記憶體位置
	always @( posedge clk ) begin
		if ( MemWrite == 1'b1 ) begin
			$display( "%d, writing data: Mem[%d] <= %d", $time/10, addr, wd );
			mem_array[addr] <= wd[7:0];
			mem_array[addr+1] <= wd[15:8];
			mem_array[addr+2] <= wd[23:16];
			mem_array[addr+3] <= wd[31:24];
		end
	end
	
endmodule




















































