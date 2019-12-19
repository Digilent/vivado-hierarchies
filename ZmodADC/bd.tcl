
################################################################
# This is a generated script based on design: design_1
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

namespace eval _tcl {
proc get_script_folder {} {
   set script_path [file normalize [info script]]
   set script_folder [file dirname $script_path]
   return $script_folder
}
}
variable script_folder
set script_folder [_tcl::get_script_folder]

################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2019.1
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   common::send_msg_id "BD_TCL-1002" "WARNING" "This script was generated using Vivado <$scripts_vivado_version> without IP versions in the create_bd_cell commands, but is now being run in <$current_vivado_version> of Vivado. There may have been major IP version changes between Vivado <$scripts_vivado_version> and <$current_vivado_version>, which could impact the parameter settings of the IPs."

}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source design_1_script.tcl

set bCheckIPsPassed 1
##################################################################
# CHECK IPs
##################################################################
set bCheckIPs 1
if { $bCheckIPs == 1 } {
   set list_check_ips "\ 
natinst.com:user:AXI_ZmodADC1410:*\
xilinx.com:user:ZmodADC1410_Controller:*\
xilinx.com:ip:axi_dma:*\
xilinx.com:ip:ila:*\
xilinx.com:ip:xlslice:*\
"

   set list_ips_missing ""
   common::send_msg_id "BD_TCL-006" "INFO" "Checking if the following IPs exist in the project's IP catalog: $list_check_ips ."

   foreach ip_vlnv $list_check_ips {
      set ip_obj [get_ipdefs -all $ip_vlnv]
      if { $ip_obj eq "" } {
         lappend list_ips_missing $ip_vlnv
      }
   }

   if { $list_ips_missing ne "" } {
      catch {common::send_msg_id "BD_TCL-115" "ERROR" "The following IPs are not found in the IP Catalog:\n  $list_ips_missing\n\nResolution: Please add the repository containing the IP(s) to the project." }
      set bCheckIPsPassed 0
   }

}

if { $bCheckIPsPassed != 1 } {
  common::send_msg_id "BD_TCL-1003" "WARNING" "Will not continue with creation of design due to the error(s) above."
  return 3
}

##################################################################
# DESIGN PROCs
##################################################################


# Hierarchical cell: ZmodADC
proc create_hier_cell_ZmodADC_0 { parentCell nameHier } {

  variable script_folder

  if { $parentCell eq "" || $nameHier eq "" } {
     catch {common::send_msg_id "BD_TCL-102" "ERROR" "create_hier_cell_ZmodADC_0() - Empty argument(s)!"}
     return
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_msg_id "BD_TCL-100" "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_msg_id "BD_TCL-101" "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj

  # Create cell and set as current instance
  set hier_obj [create_bd_cell -type hier $nameHier]
  current_bd_instance $hier_obj

  # Create interface pins
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M_AXI_S2MM

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S00_AXI

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S_AXI_LITE


  # Create pins
  create_bd_pin -dir I -from 13 -to 0 ADC_DATA_0
  create_bd_pin -dir I ADC_DCO_0
  create_bd_pin -dir I -type clk ADC_InClk
  create_bd_pin -dir O ADC_SYNC_0
  create_bd_pin -dir I -type clk AxiStreamClk
  create_bd_pin -dir O CLKIN_ADC_N_0
  create_bd_pin -dir O CLKIN_ADC_P_0
  create_bd_pin -dir O SC1_AC_H_0
  create_bd_pin -dir O SC1_AC_L_0
  create_bd_pin -dir O SC1_GAIN_H_0
  create_bd_pin -dir O SC1_GAIN_L_0
  create_bd_pin -dir O SC2_AC_H_0
  create_bd_pin -dir O SC2_AC_L_0
  create_bd_pin -dir O SC2_GAIN_H_0
  create_bd_pin -dir O SC2_GAIN_L_0
  create_bd_pin -dir O SC_COM_H_0
  create_bd_pin -dir O SC_COM_L_0
  create_bd_pin -dir I -type clk SysClk
  create_bd_pin -dir O cs_sc1_0
  create_bd_pin -dir O -type intr lIrqOut
  create_bd_pin -dir I -type rst lRst_n
  create_bd_pin -dir I -type clk s00_axi_aclk
  create_bd_pin -dir O -type intr s2mm_introut
  create_bd_pin -dir O sclk_sc_0
  create_bd_pin -dir IO sdio_sc_0

  # Create instance: AXI_ZmodADC1410_1, and set properties
  set AXI_ZmodADC1410_1 [ create_bd_cell -type ip -vlnv natinst.com:user:AXI_ZmodADC1410 AXI_ZmodADC1410_1 ]
  set_property -dict [ list \
   CONFIG.kCrossRegCnt {13} \
 ] $AXI_ZmodADC1410_1

  # Create instance: ZmodADC1410_Controll_1, and set properties
  set ZmodADC1410_Controll_1 [ create_bd_cell -type ip -vlnv xilinx.com:user:ZmodADC1410_Controller ZmodADC1410_Controll_1 ]
  set_property -dict [ list \
   CONFIG.kCh1HgMultCoefStatic {"000000000000000000"} \
   CONFIG.kCh1LgMultCoefStatic {"000000000000000000"} \
   CONFIG.kCh2HgMultCoefStatic {"000000000000000000"} \
   CONFIG.kCh2LgMultCoefStatic {"000000000000000000"} \
   CONFIG.kExtCmdInterfaceEn {true} \
   CONFIG.kExtRelayConfigEn {true} \
 ] $ZmodADC1410_Controll_1

  # Create instance: axi_dma_0, and set properties
  set axi_dma_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_dma axi_dma_0 ]
  set_property -dict [ list \
   CONFIG.c_include_mm2s {0} \
   CONFIG.c_include_sg {0} \
   CONFIG.c_sg_include_stscntrl_strm {0} \
   CONFIG.c_sg_length_width {16} \
 ] $axi_dma_0

  # Create instance: ila_0, and set properties
  set ila_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:ila ila_0 ]
  set_property -dict [ list \
   CONFIG.C_ENABLE_ILA_AXI_MON {false} \
   CONFIG.C_MONITOR_TYPE {Native} \
   CONFIG.C_NUM_OF_PROBES {8} \
 ] $ila_0

  # Create instance: xlslice_0, and set properties
  set xlslice_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice xlslice_0 ]
  set_property -dict [ list \
   CONFIG.DIN_FROM {15} \
   CONFIG.DIN_TO {2} \
   CONFIG.DIN_WIDTH {16} \
   CONFIG.DOUT_WIDTH {14} \
 ] $xlslice_0

  # Create instance: xlslice_1, and set properties
  set xlslice_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice xlslice_1 ]
  set_property -dict [ list \
   CONFIG.DIN_FROM {15} \
   CONFIG.DIN_TO {2} \
   CONFIG.DIN_WIDTH {16} \
   CONFIG.DOUT_WIDTH {14} \
 ] $xlslice_1

  # Create interface connections
  connect_bd_intf_net -intf_net AXI_ZmodADC1410_1_AXI_S2MM [get_bd_intf_pins AXI_ZmodADC1410_1/AXI_S2MM] [get_bd_intf_pins axi_dma_0/S_AXIS_S2MM]
  connect_bd_intf_net -intf_net AXI_ZmodADC1410_1_mCalibCh1 [get_bd_intf_pins AXI_ZmodADC1410_1/mCalibCh1] [get_bd_intf_pins ZmodADC1410_Controll_1/sCalibCh1]
  connect_bd_intf_net -intf_net AXI_ZmodADC1410_1_mCalibCh2 [get_bd_intf_pins AXI_ZmodADC1410_1/mCalibCh2] [get_bd_intf_pins ZmodADC1410_Controll_1/sCalibCh2]
  connect_bd_intf_net -intf_net AXI_ZmodADC1410_1_mSPI [get_bd_intf_pins AXI_ZmodADC1410_1/mSPI_IAP] [get_bd_intf_pins ZmodADC1410_Controll_1/sSPI_IAP]
  connect_bd_intf_net -intf_net axi_dma_0_M_AXI_S2MM [get_bd_intf_pins M_AXI_S2MM] [get_bd_intf_pins axi_dma_0/M_AXI_S2MM]
  connect_bd_intf_net -intf_net ps7_0_axi_periph_M00_AXI [get_bd_intf_pins S00_AXI] [get_bd_intf_pins AXI_ZmodADC1410_1/S00_AXI]
  connect_bd_intf_net -intf_net ps7_0_axi_periph_M01_AXI [get_bd_intf_pins S_AXI_LITE] [get_bd_intf_pins axi_dma_0/S_AXI_LITE]

  # Create port connections
  connect_bd_net -net ADC_DATA_0_1 [get_bd_pins ADC_DATA_0] [get_bd_pins ZmodADC1410_Controll_1/dADC_Data]
  connect_bd_net -net ADC_DCO_0_1 [get_bd_pins ADC_DCO_0] [get_bd_pins ZmodADC1410_Controll_1/DcoClk]
  connect_bd_net -net AXI_ZmodADC1410_1_lIrqOut [get_bd_pins lIrqOut] [get_bd_pins AXI_ZmodADC1410_1/lIrqOut]
  connect_bd_net -net AXI_ZmodADC1410_1_sCh1CouplingSelect [get_bd_pins AXI_ZmodADC1410_1/sCh1CouplingSelect] [get_bd_pins ZmodADC1410_Controll_1/sCh1CouplingConfig]
  connect_bd_net -net AXI_ZmodADC1410_1_sCh1GainSelect [get_bd_pins AXI_ZmodADC1410_1/sCh1GainSelect] [get_bd_pins ZmodADC1410_Controll_1/sCh1GainConfig] [get_bd_pins ila_0/probe5]
  connect_bd_net -net AXI_ZmodADC1410_1_sCh2CouplingSelect [get_bd_pins AXI_ZmodADC1410_1/sCh2CouplingSelect] [get_bd_pins ZmodADC1410_Controll_1/sCh2CouplingConfig]
  connect_bd_net -net AXI_ZmodADC1410_1_sCh2GainSelect [get_bd_pins AXI_ZmodADC1410_1/sCh2GainSelect] [get_bd_pins ZmodADC1410_Controll_1/sCh2GainConfig] [get_bd_pins ila_0/probe6]
  connect_bd_net -net AXI_ZmodADC1410_1_sTestMode [get_bd_pins AXI_ZmodADC1410_1/sTestMode] [get_bd_pins ZmodADC1410_Controll_1/sTestMode] [get_bd_pins ila_0/probe7]
  connect_bd_net -net AXI_ZmodADC1410_1_sZmodControllerRst_n [get_bd_pins AXI_ZmodADC1410_1/sZmodControllerRst_n] [get_bd_pins ZmodADC1410_Controll_1/sRst_n]
  connect_bd_net -net Net [get_bd_pins sdio_sc_0] [get_bd_pins ZmodADC1410_Controll_1/sADC_SDIO]
  connect_bd_net -net S00_ARESETN_1 [get_bd_pins lRst_n] [get_bd_pins AXI_ZmodADC1410_1/lRst_n] [get_bd_pins axi_dma_0/axi_resetn]
  connect_bd_net -net ZmodADC1410_Controll_0_CALIB_DONE_N [get_bd_pins AXI_ZmodADC1410_1/sInitDone_n] [get_bd_pins ZmodADC1410_Controll_1/sInitDone_n] [get_bd_pins ila_0/probe4]
  connect_bd_net -net ZmodADC1410_Controll_1_FIFO_EMPTY_CHA [get_bd_pins ZmodADC1410_Controll_1/FIFO_EMPTY_CHA] [get_bd_pins ila_0/probe0]
  connect_bd_net -net ZmodADC1410_Controll_1_FIFO_EMPTY_CHB [get_bd_pins ZmodADC1410_Controll_1/FIFO_EMPTY_CHB] [get_bd_pins ila_0/probe1]
  connect_bd_net -net ZmodADC1410_Controll_1_adcClkIn_n [get_bd_pins CLKIN_ADC_N_0] [get_bd_pins ZmodADC1410_Controll_1/adcClkIn_n]
  connect_bd_net -net ZmodADC1410_Controll_1_adcClkIn_p [get_bd_pins CLKIN_ADC_P_0] [get_bd_pins ZmodADC1410_Controll_1/adcClkIn_p]
  connect_bd_net -net ZmodADC1410_Controll_1_adcSync [get_bd_pins ADC_SYNC_0] [get_bd_pins ZmodADC1410_Controll_1/adcSync]
  connect_bd_net -net ZmodADC1410_Controll_1_sADC_CS [get_bd_pins cs_sc1_0] [get_bd_pins ZmodADC1410_Controll_1/sADC_CS]
  connect_bd_net -net ZmodADC1410_Controll_1_sADC_Sclk [get_bd_pins sclk_sc_0] [get_bd_pins ZmodADC1410_Controll_1/sADC_Sclk]
  connect_bd_net -net ZmodADC1410_Controll_1_sCh1CouplingH [get_bd_pins SC1_AC_H_0] [get_bd_pins ZmodADC1410_Controll_1/sCh1CouplingH]
  connect_bd_net -net ZmodADC1410_Controll_1_sCh1CouplingL [get_bd_pins SC1_AC_L_0] [get_bd_pins ZmodADC1410_Controll_1/sCh1CouplingL]
  connect_bd_net -net ZmodADC1410_Controll_1_sCh1GainH [get_bd_pins SC1_GAIN_H_0] [get_bd_pins ZmodADC1410_Controll_1/sCh1GainH]
  connect_bd_net -net ZmodADC1410_Controll_1_sCh1GainL [get_bd_pins SC1_GAIN_L_0] [get_bd_pins ZmodADC1410_Controll_1/sCh1GainL]
  connect_bd_net -net ZmodADC1410_Controll_1_sCh1Out [get_bd_pins ZmodADC1410_Controll_1/sCh1Out] [get_bd_pins xlslice_0/Din]
  connect_bd_net -net ZmodADC1410_Controll_1_sCh2CouplingH [get_bd_pins SC2_AC_H_0] [get_bd_pins ZmodADC1410_Controll_1/sCh2CouplingH]
  connect_bd_net -net ZmodADC1410_Controll_1_sCh2CouplingL [get_bd_pins SC2_AC_L_0] [get_bd_pins ZmodADC1410_Controll_1/sCh2CouplingL]
  connect_bd_net -net ZmodADC1410_Controll_1_sCh2GainH [get_bd_pins SC2_GAIN_H_0] [get_bd_pins ZmodADC1410_Controll_1/sCh2GainH]
  connect_bd_net -net ZmodADC1410_Controll_1_sCh2GainL [get_bd_pins SC2_GAIN_L_0] [get_bd_pins ZmodADC1410_Controll_1/sCh2GainL]
  connect_bd_net -net ZmodADC1410_Controll_1_sCh2Out [get_bd_pins ZmodADC1410_Controll_1/sCh2Out] [get_bd_pins xlslice_1/Din]
  connect_bd_net -net ZmodADC1410_Controll_1_sRelayComH [get_bd_pins SC_COM_H_0] [get_bd_pins ZmodADC1410_Controll_1/sRelayComH]
  connect_bd_net -net ZmodADC1410_Controll_1_sRelayComL [get_bd_pins SC_COM_L_0] [get_bd_pins ZmodADC1410_Controll_1/sRelayComL]
  connect_bd_net -net axi_dma_0_s2mm_introut [get_bd_pins s2mm_introut] [get_bd_pins axi_dma_0/s2mm_introut]
  connect_bd_net -net clk_wiz_0_clk_out1 [get_bd_pins AxiStreamClk] [get_bd_pins AXI_ZmodADC1410_1/AxiStreamClk] [get_bd_pins axi_dma_0/m_axi_s2mm_aclk]
  connect_bd_net -net clk_wiz_0_clk_out2 [get_bd_pins s00_axi_aclk] [get_bd_pins AXI_ZmodADC1410_1/s00_axi_aclk] [get_bd_pins axi_dma_0/s_axi_lite_aclk]
  connect_bd_net -net clk_wiz_0_clk_out4 [get_bd_pins ADC_InClk] [get_bd_pins ZmodADC1410_Controll_1/ADC_InClk]
  connect_bd_net -net processing_system7_0_FCLK_CLK0 [get_bd_pins SysClk] [get_bd_pins AXI_ZmodADC1410_1/SysClk] [get_bd_pins ZmodADC1410_Controll_1/SysClk] [get_bd_pins ila_0/clk]
  connect_bd_net -net xlslice_0_Dout [get_bd_pins AXI_ZmodADC1410_1/sCh1In] [get_bd_pins ila_0/probe2] [get_bd_pins xlslice_0/Dout]
  connect_bd_net -net xlslice_1_Dout [get_bd_pins AXI_ZmodADC1410_1/sCh2In] [get_bd_pins ila_0/probe3] [get_bd_pins xlslice_1/Dout]

  # Restore current instance
  current_bd_instance $oldCurInst
}


proc available_tcl_procs { } {
   puts "##################################################################"
   puts "# Available Tcl procedures to recreate hierarchical blocks:"
   puts "#"
   puts "#    create_hier_cell_ZmodADC_0 parentCell nameHier"
   puts "#"
   puts "##################################################################"
}

available_tcl_procs
