Hierarchy Port Requirements
---------------------------

The designs used in the Eclypse-Z7-HW repository (https://github.com/Digilent/Eclypse-Z7-HW)
  connect the ports of the Zmod ADC hierarchical block in the following ways:

* ADC_InClk:    Connected to a 400MHz output of a clocking wizard
* AxiStreamClk: Connected to a 100MHz output of a clocking wizard
* SysClk:       Connected to a 100MHz output of a clocking wizard
* s00_axi_aclk: Connected to a 50MHz output of a clocking wizard

* IRst_n: Connected to a Processor System Reset's peripheral_aresetn,
          with slowest_sync_clk connected to s00_axi_aclk

* S00_AXI and S_AXI_LITE: Connected to Zynq GP0 via an AXI interconnect
* M_AXI_S2MM: Connected to Zynq HP0 via an AXI SmartConnect

* lIrqOut and s2mm_introut: Connected to Zynq IRQ_F2P via a Concat IP

Constraints
-----------
* The Board Flow cannot be used with this block.
* Template constraints for all external ports can be found in the ZmodADC_*_ZmodADC.xdc file.
  The constraints for the Eclypse Z7's Zmod connector A are uncommented by default.