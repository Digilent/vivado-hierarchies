Hierarchy Port Requirements
---------------------------

The designs used in the Eclypse-Z7-HW repository (https://github.com/Digilent/Eclypse-Z7-HW)
  connect the ports of the Zmod DAC hierarchical block in the following ways:

* DacClk: Connected to a 100MHz output of a clocking wizard
* SysClk: Connected to a 100MHz output of a clocking wizard
* m_axi_mm2s_aclk: Connected to a 100MHz output of a clocking wizard
* s_axi_lite_aclk: Connected to a 50MHz output of a clocking wizard

* axi_resetn: Connected to a Processor System Reset's peripheral_aresetn,
              with slowest_sync_clk connected to s_axi_lite_aclk

* AxiLite and S_AXI_LITE: Connected to Zynq GP0 via an AXI interconnect
* M_AXI_MM2S: Connected to Zynq HP0 via an AXI SmartConnect

* mm2s_introut: Connected to Zynq IRQ_F2P via a Concat IP

Constraints
-----------
* The Board Flow cannot be used with this block.
* Template constraints for all external ports can be found in the ZmodDAC_*_ZmodDAC.xdc file.
  The constraints for the Eclypse Z7's Zmod connector B are uncommented by default.