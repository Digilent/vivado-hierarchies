################# Constraints for the Eclypse Z7's Zmod Port A #################

#set_property PACKAGE_PIN K19 [get_ports {nameHier_DAC_DATA_0[0]}]
#set_property PACKAGE_PIN L18 [get_ports {nameHier_DAC_DATA_0[1]}]
#set_property PACKAGE_PIN K18 [get_ports {nameHier_DAC_DATA_0[2]}]
#set_property PACKAGE_PIN L22 [get_ports {nameHier_DAC_DATA_0[3]}]
#set_property PACKAGE_PIN L19 [get_ports {nameHier_DAC_DATA_0[4]}]
#set_property PACKAGE_PIN K20 [get_ports {nameHier_DAC_DATA_0[5]}]
#set_property PACKAGE_PIN K21 [get_ports {nameHier_DAC_DATA_0[6]}]
#set_property PACKAGE_PIN J20 [get_ports {nameHier_DAC_DATA_0[7]}]
#set_property PACKAGE_PIN P15 [get_ports {nameHier_DAC_DATA_0[8]}]
#set_property PACKAGE_PIN N15 [get_ports {nameHier_DAC_DATA_0[9]}]
#set_property PACKAGE_PIN P18 [get_ports {nameHier_DAC_DATA_0[10]}]
#set_property PACKAGE_PIN P17 [get_ports {nameHier_DAC_DATA_0[11]}]
#set_property PACKAGE_PIN T19 [get_ports {nameHier_DAC_DATA_0[12]}]
#set_property PACKAGE_PIN R19 [get_ports {nameHier_DAC_DATA_0[13]}]
#
#set_property IOSTANDARD LVCMOS18 [get_ports -filter { name =~ nameHier_DAC_DATA*}]
#
#set_property PACKAGE_PIN N19 [get_ports nameHier_DAC_CLKIN_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_DAC_CLKIN_0]
#set_property PACKAGE_PIN M19 [get_ports nameHier_DAC_CLKIO_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_DAC_CLKIO_0]
#
##DAC SPI
#set_property PACKAGE_PIN P16 [get_ports nameHier_DAC_SDIO_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_DAC_SDIO_0]
#set_property DRIVE 4 [get_ports nameHier_DAC_SDIO_0]
#set_property PACKAGE_PIN R16 [get_ports nameHier_DAC_CS_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_DAC_CS_0]
#set_property DRIVE 4 [get_ports nameHier_DAC_CS_0]
#set_property PACKAGE_PIN T18 [get_ports nameHier_DAC_SCLK_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_DAC_SCLK_0]
#set_property DRIVE 4 [get_ports nameHier_DAC_SCLK_0]
#
#set_property PACKAGE_PIN T16 [get_ports nameHier_DAC_SET_FS1_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_DAC_SET_FS1_0]
#set_property PACKAGE_PIN T17 [get_ports nameHier_DAC_SET_FS2_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_DAC_SET_FS2_0]
#set_property PACKAGE_PIN R18 [get_ports nameHier_DAC_RESET_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_DAC_RESET_0]
#set_property PACKAGE_PIN J18 [get_ports nameHier_DAC_EN_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_DAC_EN_0]
#
#
#create_generated_clock -name nameHier_DAC_CLKIN_0 -source [get_pins bdDesign_i/nameHier/ZmodDAC1411_Controll_0/U0/InstDAC_ClkinODDR/C] -divide_by 1 [get_ports nameHier_DAC_CLKIN_0]
#create_generated_clock -name nameHier_DAC_CLKIO_0 -source [get_pins bdDesign_i/nameHier/ZmodDAC1411_Controll_0/U0/InstDAC_ClkIO_ODDR/C] -divide_by 1 [get_ports nameHier_DAC_CLKIO_0]
#set_output_delay -clock [get_clocks nameHier_DAC_CLKIN_0] -clock_fall -min -add_delay -1.200 [get_ports {nameHier_DAC_DATA_0[*]}]
#set_output_delay -clock [get_clocks nameHier_DAC_CLKIN_0] -clock_fall -max -add_delay 0.250 [get_ports {nameHier_DAC_DATA_0[*]}]
#set_output_delay -clock [get_clocks nameHier_DAC_CLKIN_0] -min -add_delay -1.100 [get_ports {nameHier_DAC_DATA_0[*]}]
#set_output_delay -clock [get_clocks nameHier_DAC_CLKIN_0] -max -add_delay 0.130 [get_ports {nameHier_DAC_DATA_0[*]}]



################# Constraints for the Eclypse Z7's Zmod Port B #################

set_property PACKAGE_PIN Y19 [get_ports {nameHier_DAC_DATA_0[0]}]
set_property PACKAGE_PIN Y18 [get_ports {nameHier_DAC_DATA_0[1]}]
set_property PACKAGE_PIN AB22 [get_ports {nameHier_DAC_DATA_0[2]}]
set_property PACKAGE_PIN AB20 [get_ports {nameHier_DAC_DATA_0[3]}]
set_property PACKAGE_PIN AA18 [get_ports {nameHier_DAC_DATA_0[4]}]
set_property PACKAGE_PIN AA19 [get_ports {nameHier_DAC_DATA_0[5]}]
set_property PACKAGE_PIN Y21 [get_ports {nameHier_DAC_DATA_0[6]}]
set_property PACKAGE_PIN Y20 [get_ports {nameHier_DAC_DATA_0[7]}]
set_property PACKAGE_PIN V15 [get_ports {nameHier_DAC_DATA_0[8]}]
set_property PACKAGE_PIN V14 [get_ports {nameHier_DAC_DATA_0[9]}]
set_property PACKAGE_PIN AB15 [get_ports {nameHier_DAC_DATA_0[10]}]
set_property PACKAGE_PIN AB14 [get_ports {nameHier_DAC_DATA_0[11]}]
set_property PACKAGE_PIN W13 [get_ports {nameHier_DAC_DATA_0[12]}]
set_property PACKAGE_PIN V13 [get_ports {nameHier_DAC_DATA_0[13]}]

set_property IOSTANDARD LVCMOS18 [get_ports -filter { name =~ nameHier_DAC_DATA*}]

set_property PACKAGE_PIN W16 [get_ports nameHier_DAC_CLKIN_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_DAC_CLKIN_0]
set_property PACKAGE_PIN W17 [get_ports nameHier_DAC_CLKIO_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_DAC_CLKIO_0]

#DAC SPI
set_property PACKAGE_PIN Y14 [get_ports nameHier_DAC_SDIO_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_DAC_SDIO_0]
set_property DRIVE 4 [get_ports nameHier_DAC_SDIO_0]
set_property PACKAGE_PIN AA14 [get_ports nameHier_DAC_CS_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_DAC_CS_0]
set_property DRIVE 4 [get_ports nameHier_DAC_CS_0]
set_property PACKAGE_PIN AA13 [get_ports nameHier_DAC_SCLK_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_DAC_SCLK_0]
set_property DRIVE 4 [get_ports nameHier_DAC_SCLK_0]

set_property PACKAGE_PIN W15 [get_ports nameHier_DAC_SET_FS1_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_DAC_SET_FS1_0]
set_property PACKAGE_PIN Y15 [get_ports nameHier_DAC_SET_FS2_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_DAC_SET_FS2_0]
set_property PACKAGE_PIN Y13 [get_ports nameHier_DAC_RESET_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_DAC_RESET_0]
set_property PACKAGE_PIN AA22 [get_ports nameHier_DAC_EN_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_DAC_EN_0]


create_generated_clock -name nameHier_DAC_CLKIN_0 -source [get_pins bdDesign_i/nameHier/ZmodDAC1411_Controll_0/U0/InstDAC_ClkinODDR/C] -divide_by 1 [get_ports nameHier_DAC_CLKIN_0]
create_generated_clock -name nameHier_DAC_CLKIO_0 -source [get_pins bdDesign_i/nameHier/ZmodDAC1411_Controll_0/U0/InstDAC_ClkIO_ODDR/C] -divide_by 1 [get_ports nameHier_DAC_CLKIO_0]
set_output_delay -clock [get_clocks nameHier_DAC_CLKIN_0] -clock_fall -min -add_delay -1.200 [get_ports {nameHier_DAC_DATA_0[*]}]
set_output_delay -clock [get_clocks nameHier_DAC_CLKIN_0] -clock_fall -max -add_delay 0.250 [get_ports {nameHier_DAC_DATA_0[*]}]
set_output_delay -clock [get_clocks nameHier_DAC_CLKIN_0] -min -add_delay -1.100 [get_ports {nameHier_DAC_DATA_0[*]}]
set_output_delay -clock [get_clocks nameHier_DAC_CLKIN_0] -max -add_delay 0.130 [get_ports {nameHier_DAC_DATA_0[*]}]
