`timescale 1ns/1ns
module barrel_shifter(
  input [31:0] dataA,//in
  input [4:0] dataB,//shift amt
  input [2:0] SignaltoSHT,
  output [31:0] dataOut,
  input reset);

  wire [160:1] s;
  wire [31:0] tempOut;

  // shift1
  MUX_2to1 m1(dataA[1], dataA[0], dataB[0], s[1] );
  MUX_2to1 m2(dataA[2], dataA[1], dataB[0], s[2]  );
  MUX_2to1 m3(dataA[3], dataA[2], dataB[0], s[3]  );
  MUX_2to1 m4(dataA[4], dataA[3], dataB[0], s[4]  );
  MUX_2to1 m5(dataA[5], dataA[4], dataB[0], s[5]  );
  MUX_2to1 m6(dataA[6], dataA[5], dataB[0], s[6]  );
  MUX_2to1 m7(dataA[7], dataA[6], dataB[0], s[7]  );
  MUX_2to1 m8(dataA[8], dataA[7], dataB[0], s[8]  );
  MUX_2to1 m9(dataA[9], dataA[8], dataB[0], s[9]  );
  MUX_2to1 m10(dataA[10], dataA[9], dataB[0], s[10] );
  MUX_2to1 m11(dataA[11], dataA[10], dataB[0], s[11] );
  MUX_2to1 m12(dataA[12], dataA[11], dataB[0], s[12] );
  MUX_2to1 m13(dataA[13], dataA[12], dataB[0], s[13] );
  MUX_2to1 m14(dataA[14], dataA[13], dataB[0], s[14] );
  MUX_2to1 m15(dataA[15], dataA[14], dataB[0], s[15] );
  MUX_2to1 m16(dataA[16], dataA[15], dataB[0], s[16] );
  MUX_2to1 m17(dataA[17], dataA[16], dataB[0], s[17] );
  MUX_2to1 m18(dataA[18], dataA[17], dataB[0], s[18] );
  MUX_2to1 m19(dataA[19], dataA[18], dataB[0], s[19] );
  MUX_2to1 m20(dataA[20], dataA[19], dataB[0], s[20] );
  MUX_2to1 m21(dataA[21], dataA[20], dataB[0], s[21] );
  MUX_2to1 m22(dataA[22], dataA[21], dataB[0], s[22] );
  MUX_2to1 m23(dataA[23], dataA[22], dataB[0], s[23] );
  MUX_2to1 m24(dataA[24], dataA[23], dataB[0], s[24] );
  MUX_2to1 m25(dataA[25], dataA[24], dataB[0], s[25] );
  MUX_2to1 m26(dataA[26], dataA[25], dataB[0], s[26] );
  MUX_2to1 m27(dataA[27], dataA[26], dataB[0], s[27] );
  MUX_2to1 m28(dataA[28], dataA[27], dataB[0], s[28] );
  MUX_2to1 m29(dataA[29], dataA[28], dataB[0], s[29] );
  MUX_2to1 m30(dataA[30], dataA[29], dataB[0], s[30] );
  MUX_2to1 m31(dataA[31], dataA[30], dataB[0], s[31] );
  MUX_2to1 m32(1'b0, dataA[31], dataB[0], s[32] );

  // shift2
  MUX_2to1 m33(s[3], s[1], dataB[1], s[33]  );
  MUX_2to1 m34(s[4], s[2], dataB[1], s[34]  );
  MUX_2to1 m35(s[5], s[3], dataB[1], s[35]  );
  MUX_2to1 m36(s[6], s[4], dataB[1], s[36]  );
  MUX_2to1 m37(s[7], s[5], dataB[1], s[37]  );
  MUX_2to1 m38(s[8], s[6], dataB[1], s[38] );
  MUX_2to1 m39(s[9], s[7], dataB[1], s[39]  );
  MUX_2to1 m40(s[10], s[8], dataB[1], s[40]  );
  MUX_2to1 m41(s[11], s[9], dataB[1], s[41]  );
  MUX_2to1 m42(s[12], s[10], dataB[1], s[42] );
  MUX_2to1 m43(s[13], s[11], dataB[1], s[43] );
  MUX_2to1 m44(s[14], s[12], dataB[1], s[44] );
  MUX_2to1 m45(s[15], s[13], dataB[1], s[45] );
  MUX_2to1 m46(s[16], s[14], dataB[1], s[46] );
  MUX_2to1 m47(s[17], s[15], dataB[1], s[47] );
  MUX_2to1 m48(s[18], s[16], dataB[1], s[48] );
  MUX_2to1 m49(s[19], s[17], dataB[1], s[49] );
  MUX_2to1 m50(s[20], s[18], dataB[1], s[50] );
  MUX_2to1 m51(s[21], s[19], dataB[1], s[51] );
  MUX_2to1 m52(s[22], s[20], dataB[1], s[52] );
  MUX_2to1 m53(s[23], s[21], dataB[1], s[53] );
  MUX_2to1 m54(s[24], s[22], dataB[1], s[54] );
  MUX_2to1 m55(s[25], s[23], dataB[1], s[55] );
  MUX_2to1 m56(s[26], s[24], dataB[1], s[56] );
  MUX_2to1 m57(s[27], s[25], dataB[1], s[57] );
  MUX_2to1 m58(s[28], s[26], dataB[1], s[58] );
  MUX_2to1 m59(s[29], s[27], dataB[1], s[59] );
  MUX_2to1 m60(s[30], s[28], dataB[1], s[60] );
  MUX_2to1 m61(s[31], s[29], dataB[1], s[61] );
  MUX_2to1 m62(s[32], s[30], dataB[1], s[62] );
  MUX_2to1 m63(1'b0, s[31], dataB[1], s[63] );
  MUX_2to1 m64(1'b0, s[32], dataB[1], s[64] );

 //shift3
  MUX_2to1 m65(s[37], s[33], dataB[2], s[65]  );
  MUX_2to1 m66(s[38], s[34], dataB[2], s[66]  );
  MUX_2to1 m67(s[39], s[35], dataB[2], s[67]  );
  MUX_2to1 m68(s[40], s[36], dataB[2], s[68]  );
  MUX_2to1 m69(s[41], s[37], dataB[2], s[69]  );
  MUX_2to1 m70(s[42], s[38], dataB[2], s[70]);
  MUX_2to1 m71(s[43], s[39], dataB[2], s[71]  );
  MUX_2to1 m72(s[44], s[40], dataB[2], s[72]  );
  MUX_2to1 m73(s[45], s[41], dataB[2], s[73]  );
  MUX_2to1 m74(s[46], s[42], dataB[2], s[74] );
  MUX_2to1 m75(s[47], s[43], dataB[2], s[75] );
  MUX_2to1 m76(s[48], s[44], dataB[2], s[76] );
  MUX_2to1 m77(s[49], s[45], dataB[2], s[77] );
  MUX_2to1 m78(s[50], s[46], dataB[2], s[78] );
  MUX_2to1 m79(s[51], s[47], dataB[2], s[79] );
  MUX_2to1 m80(s[52], s[48], dataB[2], s[80] );
  MUX_2to1 m81(s[53], s[49], dataB[2], s[81] );
  MUX_2to1 m82(s[54], s[50], dataB[2], s[82] );
  MUX_2to1 m83(s[55], s[51], dataB[2], s[83] );
  MUX_2to1 m84(s[56], s[52], dataB[2], s[84] );
  MUX_2to1 m85(s[57], s[53], dataB[2], s[85] );
  MUX_2to1 m86(s[58], s[54], dataB[2], s[86] );
  MUX_2to1 m87(s[59], s[55], dataB[2], s[87] );
  MUX_2to1 m88(s[60], s[56], dataB[2], s[88] );
  MUX_2to1 m89(s[61], s[57], dataB[2], s[89] );
  MUX_2to1 m90(s[62], s[58], dataB[2], s[90] );
  MUX_2to1 m92(s[63], s[59], dataB[2], s[91] );
  MUX_2to1 m93(s[64], s[60], dataB[2], s[92] );
  MUX_2to1 m94(1'b0, s[61], dataB[2], s[93] );
  MUX_2to1 m95(1'b0, s[62], dataB[2], s[94] );
  MUX_2to1 m96(1'b0, s[63], dataB[2], s[95] );
  MUX_2to1 m97(1'b0, s[64], dataB[2], s[96] );  

//shift4
  MUX_2to1 m98(s[73], s[65], dataB[3], s[97]  );
  MUX_2to1 m99(s[74], s[66], dataB[3], s[98]  );
  MUX_2to1 m100(s[75], s[67], dataB[3], s[99]  );
  MUX_2to1 m101(s[76], s[68], dataB[3], s[100]  );
  MUX_2to1 m102(s[77], s[69], dataB[3], s[101]  );
  MUX_2to1 m103(s[78], s[70], dataB[3], s[102] );
  MUX_2to1 m104(s[79], s[71], dataB[3], s[103]  );
  MUX_2to1 m105(s[80], s[72], dataB[3], s[104]  );
  MUX_2to1 m106(s[81], s[73], dataB[3], s[105]  );
  MUX_2to1 m107(s[82], s[74], dataB[3], s[106] );
  MUX_2to1 m108(s[83], s[75], dataB[3], s[107] );
  MUX_2to1 m109(s[84], s[76], dataB[3], s[108] );
  MUX_2to1 m110(s[85], s[77], dataB[3], s[109] );
  MUX_2to1 m111(s[86], s[78], dataB[3], s[110] );
  MUX_2to1 m112(s[87], s[79], dataB[3], s[111] );
  MUX_2to1 m113(s[88], s[80], dataB[3], s[112] );
  MUX_2to1 m114(s[89], s[81], dataB[3], s[113] );
  MUX_2to1 m115(s[90], s[82], dataB[3], s[114] );
  MUX_2to1 m116(s[91], s[83], dataB[3], s[115] );
  MUX_2to1 m117(s[92], s[84], dataB[3], s[116] );
  MUX_2to1 m118(s[93], s[85], dataB[3], s[117] );
  MUX_2to1 m119(s[94], s[86], dataB[3], s[118] );
  MUX_2to1 m120(s[95], s[87], dataB[3], s[119] );
  MUX_2to1 m121(s[96], s[88], dataB[3], s[120] );
  MUX_2to1 m122(1'b0, s[89], dataB[3], s[121] );
  MUX_2to1 m123(1'b0, s[90], dataB[3], s[122] );
  MUX_2to1 m124(1'b0, s[91], dataB[3], s[123] );
  MUX_2to1 m125(1'b0, s[92], dataB[3], s[124] );
  MUX_2to1 m126(1'b0, s[93], dataB[3], s[125] );
  MUX_2to1 m127(1'b0, s[94], dataB[3], s[126] );
  MUX_2to1 m128(1'b0, s[95], dataB[3], s[127] );
  MUX_2to1 m129(1'b0, s[96], dataB[3], s[128] ); 

//shift5
  MUX_2to1 m130(s[113], s[97], dataB[4], tempOut[0]);
  MUX_2to1 m131(s[114], s[98], dataB[4], tempOut[1]);
  MUX_2to1 m132(s[115], s[99], dataB[4], tempOut[2]);
  MUX_2to1 m133(s[116], s[100], dataB[4], tempOut[3]);
  MUX_2to1 m134(s[117], s[101], dataB[4], tempOut[4]);
  MUX_2to1 m135(s[118], s[102], dataB[4], tempOut[5]);
  MUX_2to1 m136(s[119], s[103], dataB[4], tempOut[6]);
  MUX_2to1 m137(s[120], s[104], dataB[4], tempOut[7]);
  MUX_2to1 m138(s[121], s[105], dataB[4], tempOut[8]);
  MUX_2to1 m139(s[122], s[106], dataB[4], tempOut[9]);
  MUX_2to1 m140(s[123], s[107], dataB[4], tempOut[10]);
  MUX_2to1 m141(s[124], s[108], dataB[4], tempOut[11]);
  MUX_2to1 m142(s[125], s[109], dataB[4], tempOut[12]);
  MUX_2to1 m143(s[126], s[110], dataB[4], tempOut[13]);
  MUX_2to1 m144(s[127], s[111], dataB[4], tempOut[14]);
  MUX_2to1 m145(s[128], s[112], dataB[4], tempOut[15]);
  MUX_2to1 m146(1'b0, s[113], dataB[4], tempOut[16]);
  MUX_2to1 m147(1'b0, s[114], dataB[4], tempOut[17]);
  MUX_2to1 m148(1'b0, s[115], dataB[4], tempOut[18]);
  MUX_2to1 m149(1'b0, s[116], dataB[4], tempOut[19]);
  MUX_2to1 m150(1'b0, s[117], dataB[4], tempOut[20]);
  MUX_2to1 m151(1'b0, s[118], dataB[4], tempOut[21]);
  MUX_2to1 m152(1'b0, s[119], dataB[4], tempOut[22]);
  MUX_2to1 m153(1'b0, s[120], dataB[4], tempOut[23]);
  MUX_2to1 m154(1'b0, s[121], dataB[4], tempOut[24]);
  MUX_2to1 m155(1'b0, s[122], dataB[4], tempOut[25]);
  MUX_2to1 m156(1'b0, s[123], dataB[4], tempOut[26]);
  MUX_2to1 m157(1'b0, s[124], dataB[4], tempOut[27]);
  MUX_2to1 m158(1'b0, s[125], dataB[4], tempOut[28] );
  MUX_2to1 m159(1'b0, s[126], dataB[4], tempOut[29] );
  MUX_2to1 m160(1'b0, s[127], dataB[4], tempOut[30] );
  MUX_2to1 m161(1'b0, s[128], dataB[4], tempOut[31] ); 

  assign dataOut = (reset == 1'b1) ? 32'b0 : tempOut;

endmodule

