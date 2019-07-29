Hierarchy Port Requirements
---------------------------
* `s_axi_aclk`: In order to generate a SPI clock of an appropriate frequency, the PmodAD1 hierarchy must be provided with a clock with a frequency of approximately 100 MHz. Clocks of different frequencies can be used, providing that care is taken to correctly set the PmodAD1_AXI module's parameters.

Constraints
-----------
* When using the Board Flow, no additional constraints are required.
* When not using the Board Flow, template constraints for the Pmod_out port can be found in the imported "PmodAD1_*.xdc" file.