# This script is used to add a PmodNAV hierarchical block to the base block design of a project it is run in.

# Determine if any IP dependencies are missing, if they are, add the repo directory to the repo search paths.
# Care should be taken to avoid duplicates...
proc ip_not_found {} {
	# Add any IP dependencies included in the repo directory here
	if { [llength [get_ipdefs digilentinc.com:ip:pmod_bridge:*]] == 0 } {
		return 1
	} else {
		return 0
	}
}
if { [ip_not_found] } {
	set ip_repo_list [get_property IP_REPO_PATHS [current_project]]
	lappend ip_repo_list [file join [file dirname [file dirname [info script]]] repo]
	set_property IP_REPO_PATHS $ip_repo_list [current_project]
	update_ip_catalog
}

# Add source files to the project
set list_source_files [glob -nocomplain [file join [file dirname [info script]] sources *]]
if { [llength $list_source_files] > 0 } {
	# adding the same file more than once is okay
	catch { add_files -fileset sources_1 $list_source_files }
}

# Set 
set oldCurInst [current_bd_instance .]; # Save current instance; Restore later
current_bd_instance /; # Set root design as current

set nameHier [file tail [file dirname [info script]]]
for {set idx 0} {[llength [get_bd_cells ${nameHier}_${idx}]] > 0} {incr idx} {}
set nameHier ${nameHier}_${idx}; # Find first unused trailing number for a name like PmodNAV_0

set hier_obj [create_bd_cell -type hier ${nameHier}]; # Create cell and set as current instance
current_bd_instance $hier_obj

# Create instance: PmodNAV_remap_0, and set properties
set module_refs [list]
foreach {block_name block_cell_name} $module_refs {
	if { [catch {set $block_cell_name [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
		catch {common::send_msg_id "BD_TCL-105" "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
	} elseif { $PmodNav_remap_0 eq "" } {
		catch {common::send_msg_id "BD_TCL-106" "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
	}
}

# Create instance: axi_gpio_0, and set properties
set axi_gpio_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:* axi_gpio_0 ]
set_property -dict [ list \
	CONFIG.C_GPIO_WIDTH {2} \
	CONFIG.C_INTERRUPT_PRESENT {1} \
	CONFIG.C_IS_DUAL {0} \
] $axi_gpio_0

# Create instance: axi_uart16550_0, and set properties
set axi_uart16550_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_uart16550:* axi_uart16550_0 ]
# default properties

# Create instance: pmod_bridge_0, and set properties
set pmod_bridge_0 [ create_bd_cell -type ip -vlnv digilentinc.com:ip:pmod_bridge:* pmod_bridge_0 ]
set_property -dict [ list \
	CONFIG.Bottom_Row_Interface {Disabled} \
	CONFIG.PMOD {Custom} \
	CONFIG.Top_Row_Interface {UART} \
	CONFIG.USE_BOARD_FLOW {true} \
] $pmod_bridge_0

# Create instance: xl_constant_freeze_0, and set properties
set xl_constant_freeze_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:* xl_constant_freeze_0 ]
set_property -dict [ list \
	CONFIG.CONST_VAL {0} \
	CONFIG.CONST_WIDTH {1} \
] $xl_constant_freeze_0

# Create interface pins
create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 AXI_LITE_UART
create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 AXI_LITE_GPIO
create_bd_intf_pin -mode Master -vlnv digilentinc.com:interface:pmod_rtl:1.0 Pmod_out

# Create interface connections
connect_bd_intf_net -intf_net axi_uart16550_0_UART [get_bd_intf_pins axi_uart16550_0/UART] [get_bd_intf_pins pmod_bridge_0/UART_Top_Row]
connect_bd_intf_net -intf_net axi_gpio_0_GPIO [get_bd_intf_pins axi_gpio_0/GPIO] [get_bd_intf_pins pmod_bridge_0/UART_GPIO_Top_Row]
connect_bd_intf_net -intf_net AXI_LITE_UART [get_bd_intf_pins AXI_LITE_UART] [get_bd_intf_pins axi_uart16550_0/S_AXI]
connect_bd_intf_net -intf_net AXI_LITE_GPIO [get_bd_intf_pins AXI_LITE_GPIO] [get_bd_intf_pins axi_gpio_0/S_AXI]
connect_bd_intf_net -intf_net Pmod_out [get_bd_intf_pins Pmod_out] [get_bd_intf_pins pmod_bridge_0/Pmod_out]

# Create pins
create_bd_pin -dir O gpio_interrupt
create_bd_pin -dir O uart_interrupt
create_bd_pin -dir I -type clk s_axi_aclk
create_bd_pin -dir I -type rst s_axi_aresetn

# Create port connections
connect_bd_net -net PmodBT2_axi_aclk [get_bd_pins s_axi_aclk] [get_bd_pins axi_gpio_0/s_axi_aclk] [get_bd_pins axi_uart16550_0/s_axi_aclk]
connect_bd_net -net PmodBT2_axi_aresetn [get_bd_pins s_axi_aresetn] [get_bd_pins axi_gpio_0/s_axi_aresetn] [get_bd_pins axi_uart16550_0/s_axi_aresetn]
connect_bd_net -net PmodBT2_gpio_interrupt [get_bd_pins gpio_interrupt] [get_bd_pins axi_gpio_0/ip2intc_irpt]
connect_bd_net -net PmodBT2_qspi_interrupt [get_bd_pins uart_interrupt] [get_bd_pins axi_uart16550_0/ip2intc_irpt]
connect_bd_net -net PmodBT2_freeze [get_bd_pins xl_constant_freeze_0/dout] [get_bd_pins axi_uart16550_0/freeze]


# Restore current instance
current_bd_instance $oldCurInst

#set list_constr_files [glob -nocomplain [file join [file dirname [info script]] constrs *]]
#catch { import_files -fileset constrs_1 $list_constr_files }
## TODO: add find and replace for port names...