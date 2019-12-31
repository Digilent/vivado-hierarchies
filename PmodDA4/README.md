 ## Adding PmodDA4 Hierarchy IP to your Design

Please follow the guide :

https://reference.digilentinc.com/learn/programmable-logic/tutorials/vivado-hierarchical-blocks/start

Hierarchy Requirements
* Using the Board Flow it requires the board files (Installation steps https://reference.digilentinc.com/reference/software/vivado/board-files?redirect=1 )
* When using MicroBlaze, if "section `.text' will not fit in region" error appears in SDK project, the user should increase the microblaze_0_local_memory/dlmb_bram_if_cntlr 
* and the microblaze_0_local_memory/ilmb_bram_if_cntlr range in the Address Editor tab from Vivado Block Design, then generate bistream, export bistream and update the linker script in Xilinx SDK.

Constraints
-----------
* When using the Board Flow, no additional constraints are required.
* When not using the Board Flow, template constraints for the Pmod_out port can be found in the imported "PmodDA4_*.xdc" file.


PmodDA4 Hierarchy demo

sdk_sources folder contains the demo and the drivers for the Pmod DA4.
A 2.5 V peak-to-peak triangle wave of an unspecified frequency is output at channel A.							                                                                         
To connect to UART, use a serial terminal such as TeraTerm attached to the programmed FPGA's port at the proper Baud rate.                            */
                                                                    
UART TYPE BAUD RATE                                                    
uartns550 9600                                                          
uartlite  Configurable only in HW design(default 9600)                                
ps7_uart  115200 (configured by bootrom/bsp) 
