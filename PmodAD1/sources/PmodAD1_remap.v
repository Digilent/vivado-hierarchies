`timescale 1 ns / 1 ps

module PmodAD1_remap (
	output ad1_sdin0,
	output ad1_sdin1,
	input ad1_cs,
	input ad1_sclk,
	
	(* X_INTERFACE_INFO = "xilinx.com:interface:gpio:1.0 GPIO_TOP TRI_T" *)
	output [3:0] gpio_tri_t, // Tristate output enable signal (optional)
	(* X_INTERFACE_INFO = "xilinx.com:interface:gpio:1.0 GPIO_TOP TRI_O" *)
	output [3:0] gpio_tri_o, // Tristate output signal (optional)
	(* X_INTERFACE_INFO = "xilinx.com:interface:gpio:1.0 GPIO_TOP TRI_I" *)
	input [3:0] gpio_tri_i // Tristate input signal (optional)
);
	assign ad1_sdin0 = gpio_tri_i[1];
	assign ad1_sdin1 = gpio_tri_i[2];

	assign gpio_tri_o[0] = ad1_cs;
	assign gpio_tri_o[1] = 1'b0;
	assign gpio_tri_o[2] = 1'b0;
	assign gpio_tri_o[3] = ad1_sclk;
	
	assign gpio_tri_t[0] = 1'b0;
	assign gpio_tri_t[1] = 1'b1;
	assign gpio_tri_t[2] = 1'b1;
	assign gpio_tri_t[3] = 1'b0;
endmodule