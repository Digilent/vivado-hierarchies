`timescale 1ns / 1ps

module PmodHYGRO_remap ( 
    (* X_INTERFACE_INFO = "xilinx.com:interface:gpio:1.0 GPIO_TOP TRI_I" *)
    (* X_INTERFACE_PARAMETER = "MODE Master" *)
    input wire [1:0] gpio_top_i,
    (* X_INTERFACE_INFO = "xilinx.com:interface:gpio:1.0 GPIO_TOP TRI_O" *)
    output wire [1:0] gpio_top_o,
    (* X_INTERFACE_INFO = "xilinx.com:interface:gpio:1.0 GPIO_TOP TRI_T" *)
    output wire [1:0] gpio_top_t,
    
	input wire [1:0] iic_gpo
);
	assign gpio_top_t = 2'b0;
	assign gpio_top_o = iic_gpo;
endmodule
