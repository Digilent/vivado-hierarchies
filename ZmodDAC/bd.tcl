
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
natinst.com:user:AXI_ZmodDAC1411_v1_0:*\
natinst.com:user:ZmodDAC1411_Controller:*\
xilinx.com:ip:axi_dma:*\
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


# Hierarchical cell: ZmodDAC
proc create_hier_cell_ZmodDAC_0 { parentCell nameHier } {

  variable script_folder

  if { $parentCell eq "" || $nameHier eq "" } {
     catch {common::send_msg_id "BD_TCL-102" "ERROR" "create_hier_cell_ZmodDAC_0() - Empty argument(s)!"}
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
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 AxiLite

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M_AXI_MM2S

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S_AXI_LITE


  # Create pins
  create_bd_pin -dir O -type clk DAC_CLKIN_0
  create_bd_pin -dir O -type clk DAC_CLKIO_0
  create_bd_pin -dir O DAC_CS_0
  create_bd_pin -dir O -from 13 -to 0 DAC_DATA_0
  create_bd_pin -dir O DAC_EN_0
  create_bd_pin -dir O -type rst DAC_RESET_0
  create_bd_pin -dir O DAC_SCLK_0
  create_bd_pin -dir IO DAC_SDIO_0
  create_bd_pin -dir O DAC_SET_FS1_0
  create_bd_pin -dir O DAC_SET_FS2_0
  create_bd_pin -dir I DacClk
  create_bd_pin -dir I SysClk
  create_bd_pin -dir I -type rst axi_resetn
  create_bd_pin -dir I -type clk m_axi_mm2s_aclk
  create_bd_pin -dir O -type intr mm2s_introut
  create_bd_pin -dir I -type clk s_axi_lite_aclk

  # Create instance: AXI_ZmodDAC1411_v1_0_0, and set properties
  set AXI_ZmodDAC1411_v1_0_0 [ create_bd_cell -type ip -vlnv natinst.com:user:AXI_ZmodDAC1411_v1_0 AXI_ZmodDAC1411_v1_0_0 ]

  # Create instance: ZmodDAC1411_Controll_0, and set properties
  set ZmodDAC1411_Controll_0 [ create_bd_cell -type ip -vlnv natinst.com:user:ZmodDAC1411_Controller ZmodDAC1411_Controll_0 ]
  set_property -dict [ list \
   CONFIG.kExtCalibEn {true} \
   CONFIG.kExtCmdInterfaceEn {true} \
   CONFIG.kExtScaleConfigEn {true} \
 ] $ZmodDAC1411_Controll_0

  # Create instance: axi_dma_1, and set properties
  set axi_dma_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_dma axi_dma_1 ]
  set_property -dict [ list \
   CONFIG.c_include_mm2s {1} \
   CONFIG.c_include_s2mm {0} \
   CONFIG.c_include_sg {0} \
   CONFIG.c_sg_include_stscntrl_strm {0} \
 ] $axi_dma_1

  # Create interface connections
  connect_bd_intf_net -intf_net AXI_ZmodDAC1411_v1_0_0_mCalibCh1 [get_bd_intf_pins AXI_ZmodDAC1411_v1_0_0/mCalibCh1] [get_bd_intf_pins ZmodDAC1411_Controll_0/sCalibCh1]
  connect_bd_intf_net -intf_net AXI_ZmodDAC1411_v1_0_0_mCalibCh2 [get_bd_intf_pins AXI_ZmodDAC1411_v1_0_0/mCalibCh2] [get_bd_intf_pins ZmodDAC1411_Controll_0/sCalibCh2]
  connect_bd_intf_net -intf_net AXI_ZmodDAC1411_v1_0_0_mSPI_IAP [get_bd_intf_pins AXI_ZmodDAC1411_v1_0_0/mSPI_IAP] [get_bd_intf_pins ZmodDAC1411_Controll_0/sSPI_IAP]
  connect_bd_intf_net -intf_net axi_dma_1_M_AXIS_MM2S [get_bd_intf_pins AXI_ZmodDAC1411_v1_0_0/s_axis_mm2s] [get_bd_intf_pins axi_dma_1/M_AXIS_MM2S]
  connect_bd_intf_net -intf_net axi_dma_1_M_AXI_MM2S [get_bd_intf_pins M_AXI_MM2S] [get_bd_intf_pins axi_dma_1/M_AXI_MM2S]
  connect_bd_intf_net -intf_net ps7_0_axi_periph_M02_AXI [get_bd_intf_pins AxiLite] [get_bd_intf_pins AXI_ZmodDAC1411_v1_0_0/AxiLite]
  connect_bd_intf_net -intf_net ps7_0_axi_periph_M03_AXI [get_bd_intf_pins S_AXI_LITE] [get_bd_intf_pins axi_dma_1/S_AXI_LITE]

  # Create port connections
  connect_bd_net -net AXI_ZmodDAC1411_v1_0_0_sCh1Out [get_bd_pins AXI_ZmodDAC1411_v1_0_0/sCh1Out] [get_bd_pins ZmodDAC1411_Controll_0/sCh1In]
  connect_bd_net -net AXI_ZmodDAC1411_v1_0_0_sCh1ScaleSelect [get_bd_pins AXI_ZmodDAC1411_v1_0_0/sCh1ScaleSelect] [get_bd_pins ZmodDAC1411_Controll_0/sExtCh1Scale]
  connect_bd_net -net AXI_ZmodDAC1411_v1_0_0_sCh2Out [get_bd_pins AXI_ZmodDAC1411_v1_0_0/sCh2Out] [get_bd_pins ZmodDAC1411_Controll_0/sCh2In]
  connect_bd_net -net AXI_ZmodDAC1411_v1_0_0_sCh2ScaleSelect [get_bd_pins AXI_ZmodDAC1411_v1_0_0/sCh2ScaleSelect] [get_bd_pins ZmodDAC1411_Controll_0/sExtCh2Scale]
  connect_bd_net -net AXI_ZmodDAC1411_v1_0_0_sDacEnOut [get_bd_pins AXI_ZmodDAC1411_v1_0_0/sDacEnOut] [get_bd_pins ZmodDAC1411_Controll_0/sDAC_EnIn]
  connect_bd_net -net AXI_ZmodDAC1411_v1_0_0_sZmodControllerRst_n [get_bd_pins AXI_ZmodDAC1411_v1_0_0/sZmodControllerRst_n] [get_bd_pins ZmodDAC1411_Controll_0/sRst_n]
  connect_bd_net -net Net1 [get_bd_pins DAC_SDIO_0] [get_bd_pins ZmodDAC1411_Controll_0/sDAC_SDIO]
  connect_bd_net -net S00_ARESETN_1 [get_bd_pins axi_resetn] [get_bd_pins AXI_ZmodDAC1411_v1_0_0/lRst_n] [get_bd_pins axi_dma_1/axi_resetn]
  connect_bd_net -net ZmodDAC1411_Controll_0_sDAC_CS [get_bd_pins DAC_CS_0] [get_bd_pins ZmodDAC1411_Controll_0/sDAC_CS]
  connect_bd_net -net ZmodDAC1411_Controll_0_sDAC_ClkIO [get_bd_pins DAC_CLKIO_0] [get_bd_pins ZmodDAC1411_Controll_0/sDAC_ClkIO]
  connect_bd_net -net ZmodDAC1411_Controll_0_sDAC_Clkin [get_bd_pins DAC_CLKIN_0] [get_bd_pins ZmodDAC1411_Controll_0/sDAC_Clkin]
  connect_bd_net -net ZmodDAC1411_Controll_0_sDAC_Data [get_bd_pins DAC_DATA_0] [get_bd_pins ZmodDAC1411_Controll_0/sDAC_Data]
  connect_bd_net -net ZmodDAC1411_Controll_0_sDAC_EnOut [get_bd_pins DAC_EN_0] [get_bd_pins ZmodDAC1411_Controll_0/sDAC_EnOut]
  connect_bd_net -net ZmodDAC1411_Controll_0_sDAC_Reset [get_bd_pins DAC_RESET_0] [get_bd_pins ZmodDAC1411_Controll_0/sDAC_Reset]
  connect_bd_net -net ZmodDAC1411_Controll_0_sDAC_SCLK [get_bd_pins DAC_SCLK_0] [get_bd_pins ZmodDAC1411_Controll_0/sDAC_SCLK]
  connect_bd_net -net ZmodDAC1411_Controll_0_sDAC_SetFS1 [get_bd_pins DAC_SET_FS1_0] [get_bd_pins ZmodDAC1411_Controll_0/sDAC_SetFS1]
  connect_bd_net -net ZmodDAC1411_Controll_0_sDAC_SetFS2 [get_bd_pins DAC_SET_FS2_0] [get_bd_pins ZmodDAC1411_Controll_0/sDAC_SetFS2]
  connect_bd_net -net ZmodDAC1411_Controll_0_sInitDone_n [get_bd_pins AXI_ZmodDAC1411_v1_0_0/sInitDone_n] [get_bd_pins ZmodDAC1411_Controll_0/sInitDone_n]
  connect_bd_net -net axi_dma_1_mm2s_introut [get_bd_pins mm2s_introut] [get_bd_pins axi_dma_1/mm2s_introut]
  connect_bd_net -net clk_wiz_0_clk_out1 [get_bd_pins m_axi_mm2s_aclk] [get_bd_pins AXI_ZmodDAC1411_v1_0_0/AxiStreamClk] [get_bd_pins axi_dma_1/m_axi_mm2s_aclk]
  connect_bd_net -net clk_wiz_0_clk_out2 [get_bd_pins s_axi_lite_aclk] [get_bd_pins AXI_ZmodDAC1411_v1_0_0/s00_axi_aclk] [get_bd_pins axi_dma_1/s_axi_lite_aclk]
  connect_bd_net -net clk_wiz_0_clk_out5 [get_bd_pins DacClk] [get_bd_pins ZmodDAC1411_Controll_0/DacClk]
  connect_bd_net -net processing_system7_0_FCLK_CLK0 [get_bd_pins SysClk] [get_bd_pins AXI_ZmodDAC1411_v1_0_0/SysClk] [get_bd_pins ZmodDAC1411_Controll_0/SysClk]

  # Restore current instance
  current_bd_instance $oldCurInst
}


proc available_tcl_procs { } {
   puts "##################################################################"
   puts "# Available Tcl procedures to recreate hierarchical blocks:"
   puts "#"
   puts "#    create_hier_cell_ZmodDAC_0 parentCell nameHier"
   puts "#"
   puts "##################################################################"
}

available_tcl_procs
