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
set module_refs [list \
	PmodHYGRO_remap PmodHYGRO_remap_0 \
]
foreach {block_name block_cell_name} $module_refs {
	if { [catch {set $block_cell_name [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
		catch {common::send_msg_id "BD_TCL-105" "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
	} elseif { $PmodNav_remap_0 eq "" } {
		catch {common::send_msg_id "BD_TCL-106" "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
	}
}

# Create instance: axi_iic_0, and set properties
set axi_iic_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_iic:* axi_iic_0 ]
set_property -dict [ list \
	CONFIG.C_GPO_WIDTH {2} \
] $axi_iic_0

# Create instance: axi_timer_0, and set properties
set axi_timer_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_timer:* axi_timer_0 ]
set_property -dict [ list \
	CONFIG.enable_timer2 {0} \
] $axi_timer_0

# Create instance: pmod_bridge_0, and set properties
set pmod_bridge_0 [ create_bd_cell -type ip -vlnv digilentinc.com:ip:pmod_bridge:* pmod_bridge_0 ]
set_property -dict [ list \
	CONFIG.Bottom_Row_Interface {Disabled} \
	CONFIG.PMOD {Custom} \
	CONFIG.Top_Row_Interface {I2C} \
	CONFIG.USE_BOARD_FLOW {true} \
] $pmod_bridge_0

# Create interface pins
create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 AXI_LITE_I2C
create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 AXI_LITE_TIMER
create_bd_intf_pin -mode Master -vlnv digilentinc.com:interface:pmod_rtl:1.0 Pmod_out

# Create interface connections
connect_bd_intf_net -intf_net axi_iic_0_IIC [get_bd_intf_pins axi_iic_0/IIC] [get_bd_intf_pins pmod_bridge_0/I2C_Top_Row]
connect_bd_intf_net -intf_net AXI_LITE_I2C [get_bd_intf_pins AXI_LITE_I2C] [get_bd_intf_pins axi_iic_0/S_AXI]
connect_bd_intf_net -intf_net AXI_LITE_TIMER [get_bd_intf_pins AXI_LITE_TIMER] [get_bd_intf_pins axi_timer_0/S_AXI]
connect_bd_intf_net -intf_net PmodHYGRO_remap_0_GPIO_TOP [get_bd_intf_pins PmodHYGRO_remap_0/GPIO_TOP] [get_bd_intf_pins pmod_bridge_0/I2C_GPIO_Top_Row]
connect_bd_intf_net -intf_net Pmod_out [get_bd_intf_pins Pmod_out] [get_bd_intf_pins pmod_bridge_0/Pmod_out]

# Create pins
create_bd_pin -dir I -type clk s_axi_aclk
create_bd_pin -dir I -type rst s_axi_aresetn
create_bd_pin -dir O i2c_interrupt
create_bd_pin -dir O timer_interrupt

# Create port connections
connect_bd_net -net PmodHYGRO_axi_aclk [get_bd_pins s_axi_aclk] [get_bd_pins axi_iic_0/s_axi_aclk] [get_bd_pins axi_timer_0/s_axi_aclk]
connect_bd_net -net PmodHYGRO_axi_aresetn [get_bd_pins s_axi_aresetn] [get_bd_pins axi_iic_0/s_axi_aresetn] [get_bd_pins axi_timer_0/s_axi_aresetn]
connect_bd_net -net PmodHYGRO_i2c_interrupt [get_bd_pins i2c_interrupt] [get_bd_pins axi_iic_0/iic2intc_irpt]
connect_bd_net -net PmodHYGRO_timer_interrupt [get_bd_pins timer_interrupt] [get_bd_pins axi_timer_0/interrupt]
connect_bd_net -net axi_iic_0_gpo [get_bd_pins axi_iic_0/gpo] [get_bd_pins PmodHYGRO_remap_0/iic_gpo]

# Restore current instance
current_bd_instance $oldCurInst

#set list_constr_files [glob -nocomplain [file join [file dirname [info script]] constrs *]]
#catch { import_files -fileset constrs_1 $list_constr_files }
## TODO: add find and replace for port names...