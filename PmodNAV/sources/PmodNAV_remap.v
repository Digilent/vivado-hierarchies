`timescale 1ns / 1ps

module PmodNAV_remap ( 
    (* X_INTERFACE_INFO = "xilinx.com:interface:gpio:1.0 GPIO_TOP TRI_I" *)
    (* X_INTERFACE_PARAMETER = "MODE Master" *)
    input wire [3:0] gpio_top_i,
    (* X_INTERFACE_INFO = "xilinx.com:interface:gpio:1.0 GPIO_TOP TRI_O" *)
    output wire [3:0] gpio_top_o,
    (* X_INTERFACE_INFO = "xilinx.com:interface:gpio:1.0 GPIO_TOP TRI_T" *)
    output wire [3:0] gpio_top_t,
    
    (* X_INTERFACE_INFO = "xilinx.com:interface:gpio:1.0 GPIO_BOTTOM TRI_I" *)
    (* X_INTERFACE_PARAMETER = "MODE Master" *)
    input wire [3:0] gpio_bottom_i,
    (* X_INTERFACE_INFO = "xilinx.com:interface:gpio:1.0 GPIO_BOTTOM TRI_O" *)
    output wire [3:0] gpio_bottom_o,
    (* X_INTERFACE_INFO = "xilinx.com:interface:gpio:1.0 GPIO_BOTTOM TRI_T" *)
    output wire [3:0] gpio_bottom_t,
    
    (* X_INTERFACE_INFO = "xilinx.com:interface:spi:1.0 SPI_IN IO0_I" *)
    (* X_INTERFACE_PARAMETER = "MODE Slave" *)
    output wire spi_in_io0_i, // IO0 Input Port (required)
    (* X_INTERFACE_INFO = "xilinx.com:interface:spi:1.0 SPI_IN IO0_O" *)
    input wire spi_in_io0_o, // IO0 Output Port (required)
    (* X_INTERFACE_INFO = "xilinx.com:interface:spi:1.0 SPI_IN IO0_T" *)
    input wire spi_in_io0_t, // IO0 Tri-State Control (required)
    (* X_INTERFACE_INFO = "xilinx.com:interface:spi:1.0 SPI_IN IO1_I" *)
    output wire spi_in_io1_i, // IO1 Input Port (required)
    (* X_INTERFACE_INFO = "xilinx.com:interface:spi:1.0 SPI_IN IO1_O" *)
    input wire spi_in_io1_o, // IO1 Output Port (required)
    (* X_INTERFACE_INFO = "xilinx.com:interface:spi:1.0 SPI_IN IO1_T" *)
    input wire spi_in_io1_t, // IO1 Tri-State Control (required)
    (* X_INTERFACE_INFO = "xilinx.com:interface:spi:1.0 SPI_IN SCK_I" *)
    output wire spi_in_sck_i, // SPI Clock Input (required)
    (* X_INTERFACE_INFO = "xilinx.com:interface:spi:1.0 SPI_IN SCK_O" *)
    input wire spi_in_sck_o, // SPI Clock Output (required)
    (* X_INTERFACE_INFO = "xilinx.com:interface:spi:1.0 SPI_IN SCK_T" *)
    input wire spi_in_sck_t, // SPI Clock Tri-State Control (required)
    (* X_INTERFACE_INFO = "xilinx.com:interface:spi:1.0 SPI_IN SS_I" *)
    output wire [2:0] spi_in_ss_i, // Slave Select Input (optional)
    (* X_INTERFACE_INFO = "xilinx.com:interface:spi:1.0 SPI_IN SS_O" *)
    input wire [2:0] spi_in_ss_o,  // Slave Select Output (required)
    (* X_INTERFACE_INFO = "xilinx.com:interface:spi:1.0 SPI_IN SS_T" *)
    input wire spi_in_ss_t, // Slave Select Tri-State Control (required)
    
    (* X_INTERFACE_INFO = "xilinx.com:interface:gpio:1.0 GPIO_IN TRI_I" *)
    (* X_INTERFACE_PARAMETER = "MODE Slave" *)
    output wire [1:0] gpio_in_i,
    (* X_INTERFACE_INFO = "xilinx.com:interface:gpio:1.0 GPIO_IN TRI_O" *)
    input wire [1:0] gpio_in_o,
    (* X_INTERFACE_INFO = "xilinx.com:interface:gpio:1.0 GPIO_IN TRI_T" *)
    input wire [1:0] gpio_in_t
    
);
    localparam ALT_BOTTOM_PIN = 3, ALT_SS_PIN = 2;
    localparam MAG_BOTTOM_PIN = 2, MAG_SS_PIN = 1;
    localparam AG_TOP_PIN = 0,  AG_SS_PIN = 0;
    localparam SCK_TOP_PIN = 3;
    localparam IO1_TOP_PIN = 2;
    localparam IO0_TOP_PIN = 1;
    
    assign gpio_bottom_o[1:0] = gpio_in_o;
    assign gpio_bottom_o[MAG_BOTTOM_PIN] = spi_in_ss_o[MAG_SS_PIN];
    assign gpio_bottom_o[ALT_BOTTOM_PIN] = spi_in_ss_o[ALT_SS_PIN];
    
    assign gpio_bottom_t[1:0] = gpio_in_t;
    assign gpio_bottom_t[MAG_BOTTOM_PIN] = spi_in_ss_t;
    assign gpio_bottom_t[ALT_BOTTOM_PIN] = spi_in_ss_t;
    
    assign gpio_top_o[AG_TOP_PIN] = spi_in_ss_o[AG_SS_PIN];
    assign gpio_top_o[SCK_TOP_PIN] = spi_in_sck_o;
    assign gpio_top_o[IO1_TOP_PIN] = spi_in_io1_o;
    assign gpio_top_o[IO0_TOP_PIN] = spi_in_io0_o;
    
    assign gpio_top_t[AG_TOP_PIN] = spi_in_ss_t;
    assign gpio_top_t[SCK_TOP_PIN] = spi_in_sck_t;
    assign gpio_top_t[IO1_TOP_PIN] = spi_in_io1_t;
    assign gpio_top_t[IO0_TOP_PIN] = spi_in_io0_t;
    
    assign gpio_in_i = gpio_bottom_i[1:0];
    assign spi_in_ss_i[MAG_SS_PIN] = gpio_bottom_i[MAG_BOTTOM_PIN];
    assign spi_in_ss_i[ALT_SS_PIN] = gpio_bottom_i[ALT_BOTTOM_PIN];
    assign spi_in_ss_o[AG_SS_PIN] = gpio_top_o[AG_TOP_PIN];
    assign spi_in_sck_i = gpio_top_i[SCK_TOP_PIN];
    assign spi_in_io1_i = gpio_top_i[IO1_TOP_PIN];
    assign spi_in_io0_i = gpio_top_i[IO0_TOP_PIN];
endmodule
