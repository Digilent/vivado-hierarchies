# This script manages dependencies and name resolution for the bd.tcl script

# Determine if any IP dependencies are missing, if they are, add the repo directory to the repo search paths.
# Care should be taken to avoid duplicates...
proc ip_not_found {} {
	# Add any IP dependencies included in the repo directory here
	set ip_vlnvs [list \
		digilentinc.com:ip:pmod_bridge:* \
	]
	foreach vlnv $ip_vlnvs {
		if { [llength [get_ipdefs $vlnv]] == 0 } {
			return 1
		}
	}
	return 0
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

# Resolve hierarchy name
set pmod [file tail [file dirname [info script]]]
for {set idx 0} {[llength [get_bd_cells /${pmod}_${idx}]] > 0} {incr idx} {}
set nameHier ${pmod}_${idx}; # Find first unused trailing number for a name like PmodNAV_0

# Create temporary copies of each constraint file, affix the hierarchy name to them, then import them into the project
set list_constr_files [glob -nocomplain [file join [file dirname [info script]] constrs *]]
set tempdir [file join [file dirname [info script]] temp]
file mkdir $tempdir
foreach constr $list_constr_files {
	file copy ${constr} [file join ${tempdir} ${nameHier}_[file tail ${constr}]]
}
set list_temp_constr_files [glob -nocomplain [file join ${tempdir} *]]
catch { import_files -fileset constrs_1 $list_temp_constr_files }
file delete -force $tempdir

# call the process created in the bd.tcl script. recreates the described hierarchy
source [file join [file dirname [info script]] "bd.tcl"]
create_hier_cell_${pmod}_* / $nameHier
