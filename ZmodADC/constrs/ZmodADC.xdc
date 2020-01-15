################# Constraints for the Eclypse Z7's Zmod Port A #################

set_property PACKAGE_PIN T16 [get_ports nameHier_SC1_AC_H_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC1_AC_H_0]
set_property PACKAGE_PIN T17 [get_ports nameHier_SC1_AC_L_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC1_AC_L_0]
set_property PACKAGE_PIN R19 [get_ports nameHier_SC2_AC_H_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC2_AC_H_0]
set_property PACKAGE_PIN T19 [get_ports nameHier_SC2_AC_L_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC2_AC_L_0]
set_property PACKAGE_PIN N15 [get_ports nameHier_SC1_GAIN_H_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC1_GAIN_H_0]
set_property PACKAGE_PIN P15 [get_ports nameHier_SC1_GAIN_L_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC1_GAIN_L_0]
set_property PACKAGE_PIN P17 [get_ports nameHier_SC2_GAIN_H_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC2_GAIN_H_0]
set_property PACKAGE_PIN P18 [get_ports nameHier_SC2_GAIN_L_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC2_GAIN_L_0]
set_property PACKAGE_PIN J20 [get_ports nameHier_SC_COM_H_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC_COM_H_0]
set_property PACKAGE_PIN K21 [get_ports nameHier_SC_COM_L_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC_COM_L_0]
################# ADC (SC1 & SC2) ########################################
#ADC1(Scope 1&2) & ADC2(Scope 3&4) SPI
set_property PACKAGE_PIN R18 [get_ports nameHier_sdio_sc_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_sdio_sc_0]
set_property DRIVE 4 [get_ports nameHier_sdio_sc_0]
set_property PACKAGE_PIN M21 [get_ports nameHier_cs_sc1_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_cs_sc1_0]
set_property DRIVE 4 [get_ports nameHier_cs_sc1_0]
set_property PACKAGE_PIN T18 [get_ports nameHier_sclk_sc_0]
set_property IOSTANDARD LVCMOS18 [get_ports nameHier_sclk_sc_0]
set_property DRIVE 4 [get_ports nameHier_sclk_sc_0]
#ADC1 & ADC2 SYNC
set_property PACKAGE_PIN M22 [get_ports nameHier_ADC_SYNC_0]
#set_property PACKAGE_PIN Y19 [get_ports {sync_adc[1]}]
set_property IOSTANDARD LVCMOS18 [get_ports -filter { name =~ nameHier_ADC_SYNC* }]
set_property DRIVE 4 [get_ports -filter { name =~ nameHier_ADC_SYNC* }]
set_property SLEW SLOW [get_ports -filter { name =~ nameHier_ADC_SYNC* }]
#  S4       S8       F4       F8
#  1.99ns   2.56ns   1.82ns   2.06ns

#ADC 1&2 DCO
set_property PACKAGE_PIN M19 [get_ports nameHier_ADC_DCO_0]
#set_property PACKAGE_PIN W17 [get_ports {clkout_adc[1]}]
set_property IOSTANDARD LVCMOS18 [get_ports -filter { name =~ nameHier_ADC_DCO* }]
#ADC1&2 CLK
#set_property PACKAGE_PIN AB14 [get_ports {clkin_adc[2]}]
#set_property PACKAGE_PIN AB15 [get_ports {clkin_adc[3]}]

set_property IOSTANDARD DIFF_SSTL18_I [get_ports -filter { name =~ nameHier_CLKIN_ADC* }]
set_property PACKAGE_PIN N19 [get_ports nameHier_CLKIN_ADC_P_0]
set_property PACKAGE_PIN N20 [get_ports nameHier_CLKIN_ADC_N_0]
set_property SLEW SLOW [get_ports -filter { name =~ nameHier_CLKIN_ADC* }]
# slow I    2.06ns
# slow II   1.76ns
# fast I    1.60ns
# fast II   1.59ns

set_property PACKAGE_PIN N22 [get_ports {nameHier_ADC_DATA_0[0]}]
set_property PACKAGE_PIN L21 [get_ports {nameHier_ADC_DATA_0[1]}]
set_property PACKAGE_PIN R16 [get_ports {nameHier_ADC_DATA_0[2]}]
set_property PACKAGE_PIN J18 [get_ports {nameHier_ADC_DATA_0[3]}]
set_property PACKAGE_PIN K18 [get_ports {nameHier_ADC_DATA_0[4]}]
set_property PACKAGE_PIN L19 [get_ports {nameHier_ADC_DATA_0[5]}]
set_property PACKAGE_PIN L18 [get_ports {nameHier_ADC_DATA_0[6]}]
set_property PACKAGE_PIN L22 [get_ports {nameHier_ADC_DATA_0[7]}]
set_property PACKAGE_PIN K20 [get_ports {nameHier_ADC_DATA_0[8]}]
set_property PACKAGE_PIN P16 [get_ports {nameHier_ADC_DATA_0[9]}]
set_property PACKAGE_PIN K19 [get_ports {nameHier_ADC_DATA_0[10]}]
set_property PACKAGE_PIN J22 [get_ports {nameHier_ADC_DATA_0[11]}]
set_property PACKAGE_PIN J21 [get_ports {nameHier_ADC_DATA_0[12]}]
set_property PACKAGE_PIN P22 [get_ports {nameHier_ADC_DATA_0[13]}]

set_property IOSTANDARD LVCMOS18 [get_ports -filter { name =~ nameHier_ADC_DATA*}]

# DCO to data skew [-1.2;1] ns -2.24 + 5 ns ADC Output delay option (DCO delay)

create_clock -period 10.000 -name nameHier_ADC_DCO_0 -waveform {0.000 5.000} [get_ports nameHier_ADC_DCO_0]
create_generated_clock -name nameHier_CLKIN_ADC_P_0 -source [get_pins bdDesign_i/nameHier/ZmodADC1410_Controll_1/U0/InstADC_ClkODDR/C] -divide_by 1 [get_ports nameHier_CLKIN_ADC_P_0]

set_input_delay -clock [get_clocks nameHier_ADC_DCO_0] -clock_fall -min -add_delay 3.240 [get_ports {nameHier_ADC_DATA_0[*]}]
set_input_delay -clock [get_clocks nameHier_ADC_DCO_0] -clock_fall -max -add_delay 5.440 [get_ports {nameHier_ADC_DATA_0[*]}]
set_input_delay -clock [get_clocks nameHier_ADC_DCO_0] -min -add_delay 3.240 [get_ports {nameHier_ADC_DATA_0[*]}]
set_input_delay -clock [get_clocks nameHier_ADC_DCO_0] -max -add_delay 5.440 [get_ports {nameHier_ADC_DATA_0[*]}]



################# Constraints for the Eclypse Z7's Zmod Port B #################

#set_property PACKAGE_PIN W15 [get_ports nameHier_SC1_AC_H_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC1_AC_H_0]
#set_property PACKAGE_PIN Y15 [get_ports nameHier_SC1_AC_L_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC1_AC_L_0]
#set_property PACKAGE_PIN V13 [get_ports nameHier_SC2_AC_H_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC2_AC_H_0]
#set_property PACKAGE_PIN W13 [get_ports nameHier_SC2_AC_L_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC2_AC_L_0]
#set_property PACKAGE_PIN V14 [get_ports nameHier_SC1_GAIN_H_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC1_GAIN_H_0]
#set_property PACKAGE_PIN V15 [get_ports nameHier_SC1_GAIN_L_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC1_GAIN_L_0]
#set_property PACKAGE_PIN AB14 [get_ports nameHier_SC2_GAIN_H_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC2_GAIN_H_0]
#set_property PACKAGE_PIN AB15 [get_ports nameHier_SC2_GAIN_L_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC2_GAIN_L_0]
#set_property PACKAGE_PIN Y20 [get_ports nameHier_SC_COM_H_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC_COM_H_0]
#set_property PACKAGE_PIN Y21 [get_ports nameHier_SC_COM_L_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_SC_COM_L_0]
################## ADC (SC1 & SC2) ########################################
##ADC1(Scope 1&2) & ADC2(Scope 3&4) SPI
#set_property PACKAGE_PIN Y13 [get_ports nameHier_sdio_sc_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_sdio_sc_0]
#set_property DRIVE 4 [get_ports nameHier_sdio_sc_0]
#set_property PACKAGE_PIN V17 [get_ports nameHier_cs_sc1_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_cs_sc1_0]
#set_property DRIVE 4 [get_ports nameHier_cs_sc1_0]
#set_property PACKAGE_PIN AA13 [get_ports nameHier_sclk_sc_0]
#set_property IOSTANDARD LVCMOS18 [get_ports nameHier_sclk_sc_0]
#set_property DRIVE 4 [get_ports nameHier_sclk_sc_0]
##ADC1 & ADC2 SYNC
#set_property PACKAGE_PIN U17 [get_ports nameHier_ADC_SYNC_0]
##set_property PACKAGE_PIN Y19 [get_ports {sync_adc[1]}]
#set_property IOSTANDARD LVCMOS18 [get_ports -filter { name =~ nameHier_ADC_SYNC* }]
#set_property DRIVE 4 [get_ports -filter { name =~ nameHier_ADC_SYNC* }]
#set_property SLEW SLOW [get_ports -filter { name =~ nameHier_ADC_SYNC* }]
##  S4       S8       F4       F8
##  1.99ns   2.56ns   1.82ns   2.06ns
#
##ADC 1&2 DCO
#set_property PACKAGE_PIN W17 [get_ports nameHier_ADC_DCO_0]
##set_property PACKAGE_PIN W17 [get_ports {clkout_adc[1]}]
#set_property IOSTANDARD LVCMOS18 [get_ports -filter { name =~ nameHier_ADC_DCO* }]
##ADC1&2 CLK
##set_property PACKAGE_PIN AB14 [get_ports {clkin_adc[2]}]
##set_property PACKAGE_PIN AB15 [get_ports {clkin_adc[3]}]
#
#set_property IOSTANDARD DIFF_SSTL18_I [get_ports -filter { name =~ nameHier_CLKIN_ADC* }]
#set_property PACKAGE_PIN W16 [get_ports nameHier_CLKIN_ADC_P_0]
#set_property PACKAGE_PIN Y16 [get_ports nameHier_CLKIN_ADC_N_0]
#set_property SLEW SLOW [get_ports -filter { name =~ nameHier_CLKIN_ADC* }]
## slow I    2.06ns
## slow II   1.76ns
## fast I    1.60ns
## fast II   1.59ns
#
#set_property PACKAGE_PIN U16 [get_ports {nameHier_ADC_DATA_0[0]}]
#set_property PACKAGE_PIN AB19 [get_ports {nameHier_ADC_DATA_0[1]}]
#set_property PACKAGE_PIN AA14 [get_ports {nameHier_ADC_DATA_0[2]}]
#set_property PACKAGE_PIN AA22 [get_ports {nameHier_ADC_DATA_0[3]}]
#set_property PACKAGE_PIN AB22 [get_ports {nameHier_ADC_DATA_0[4]}]
#set_property PACKAGE_PIN AA18 [get_ports {nameHier_ADC_DATA_0[5]}]
#set_property PACKAGE_PIN Y18 [get_ports {nameHier_ADC_DATA_0[6]}]
#set_property PACKAGE_PIN AB20 [get_ports {nameHier_ADC_DATA_0[7]}]
#set_property PACKAGE_PIN AA19 [get_ports {nameHier_ADC_DATA_0[8]}]
#set_property PACKAGE_PIN Y14 [get_ports {nameHier_ADC_DATA_0[9]}]
#set_property PACKAGE_PIN Y19 [get_ports {nameHier_ADC_DATA_0[10]}]
#set_property PACKAGE_PIN AB21 [get_ports {nameHier_ADC_DATA_0[11]}]
#set_property PACKAGE_PIN AA21 [get_ports {nameHier_ADC_DATA_0[12]}]
#set_property PACKAGE_PIN U15 [get_ports {nameHier_ADC_DATA_0[13]}]
#
#set_property IOSTANDARD LVCMOS18 [get_ports -filter { name =~ nameHier_ADC_DATA*}]
#
## DCO to data skew [-1.2;1] ns -2.24 + 5 ns ADC Output delay option (DCO delay)
#
#create_clock -period 10.000 -name nameHier_ADC_DCO_0 -waveform {0.000 5.000} [get_ports nameHier_ADC_DCO_0]
#create_generated_clock -name nameHier_CLKIN_ADC_P_0 -source [get_pins bdDesign_i/nameHier/ZmodADC1410_Controll_1/U0/InstADC_ClkODDR/C] -divide_by 1 [get_ports nameHier_CLKIN_ADC_P_0]
#
#set_input_delay -clock [get_clocks nameHier_ADC_DCO_0] -clock_fall -min -add_delay 3.240 [get_ports {nameHier_ADC_DATA_0[*]}]
#set_input_delay -clock [get_clocks nameHier_ADC_DCO_0] -clock_fall -max -add_delay 5.440 [get_ports {nameHier_ADC_DATA_0[*]}]
#set_input_delay -clock [get_clocks nameHier_ADC_DCO_0] -min -add_delay 3.240 [get_ports {nameHier_ADC_DATA_0[*]}]
#set_input_delay -clock [get_clocks nameHier_ADC_DCO_0] -max -add_delay 5.440 [get_ports {nameHier_ADC_DATA_0[*]}]
