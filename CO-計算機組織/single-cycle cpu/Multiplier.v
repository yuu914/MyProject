module Multiplier(
    input clk, // 時鐘輸入
    input [31:0] dataA, // 32 位元無號被乘數輸入
    input [31:0] dataB, // 32 位元無號乘數輸入
    input SignaltoMUL, // 指令輸入，用於控制乘法器的運算
    output reg [63:0] MULAns, // 64 位元無號積數輸出
    input reset // 重置輸入
);



reg [63:0] temp_dataA; // 左移後的被乘數
reg [31:0] temp_dataB; // 右移後的乘數
reg [63:0] temp_MULAns; // 暫存的積數

reg [6:0] count ;
reg doIt;


always @(posedge clk or reset ) begin // 在每個時脈上升緣觸發時

    if ( reset ) // 如果重置信號為 1
        begin
                temp_dataA = 64'b0 ; // 將左移後的被乘數歸零
                temp_dataB = 32'b0 ; // 將右移後的乘數歸零
                temp_MULAns = 64'b0 ; // 將暫存的積數歸零
                MULAns = 64'b0 ; // 將輸出的積數歸零
                doIt = 1'b0;
                count = 7'b0;
        end


    if ( SignaltoMUL ) begin
        doIt = 1'b1;
        temp_dataA <= dataA; // 將被乘數賦值給左移後的被乘數
        temp_dataB <= dataB; // 將乘數賦值給右移後的乘數
    end
      
    

    if( doIt )
        begin

        if(count == 7'd32) // 32
        begin			
            MULAns <= temp_MULAns; // 將暫存的積數賦值給輸出的積數
            temp_dataA = 64'b0 ; // 將左移後的被乘數歸零
            temp_dataB = 32'b0 ; // 將右移後的乘數歸零
            temp_MULAns = 64'b0 ; // 將暫存的積數歸零
            doIt = 1'b0;
            count = 7'b0;
        end

        else if( count < 7'd32 )
        begin
		
          if (temp_dataB[0] == 1'b1)  // 如果乘數的最低位元為 1
            temp_MULAns = temp_MULAns + temp_dataA; // 將被乘數加到積數上
          

          temp_dataA = temp_dataA << 1; // 左移被乘數
          temp_dataB = temp_dataB >> 1; // 右移乘數
		

        end
          count = count + 1; // 計數器加 1

    end
end

endmodule

