Hierarchy Port Requirements
---------------------------
* `ext_spi_clk`: When combined, the frequency ratio of the AXI Quad SPI IP core (default 16x1) and frequency of the ext_spi_clk must produce a SPI clock of less than 10 MHz. The hierarchy was validated with an ext_spi_clk frequency of 50 MHz (50MHz / 16 = 3.125 MHz SPI clock frequency).
* `*_interrupt`: These ports do not need to be connected to an interrupt controller.

Constraints
-----------
* When using the Board Flow, no additional constraints are required.
* When not using the Board Flow, template constraints for the Pmod_out port can be found in the imported "PmodNAV_*.xdc" file.