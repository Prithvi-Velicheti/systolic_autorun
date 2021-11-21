`timescale 1 ps / 1 ps
module float_mul (
		input   clock,
		input   resetn,
		input   ivalid, 
		input   iready,
		output  ovalid, 
		output  oready,
		
		input  wire [31:0]  in1, // dataa_0.dataa_0
		input  wire [31:0]  in2, // datab_0.datab_0
		
		output wire [31:0] result  //  result.result
	);


	assign ovalid = 1'b1;
	assign oready = 1'b1;
	//Reset has to be assigned.
//	assign resetn = 1'b1 ; //doubt

	// ivalid, iready, resetn are ignored

	//assign result = {14'd0, result_18b};


	 float_dspmul float_dspmul_inst (
                 .clk(clock),    //    clk.clk
                 .areset(resetn), // areset.reset
                 .a(in1),      //      a.a
                 .b(in2),      //      b.b
                 .q(result)       //      q.q
        );

endmodule
