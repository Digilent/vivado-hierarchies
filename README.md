Directory Structure:
	Pmod*** folders:
		create_hier.tcl:
			This is what the user sources to add a hierarchical block for that Pmod into their design.
			Manages dependencies (IP, module reference sources) and names for the bd.tcl script.
		bd.tcl
			Created using the command <code>write_bd_tcl -no_ip_version -hier_blks (hierarchy) bd.tcl</code>.
			(hierarchy) is the 
			When sourced, creates a process that can be called to recreate the hierarchy.
			Further info on this in "Usage" section below.
		sdk_sources/:
			Contains files to be copied into <app>/src in SDK. This includes top level Pmod drivers and example main.
			TODO: how to handle dependencies? Some Pmods require C++ application projects. Some Pmods require the math.h library.
			Some ideas, in ascending level of difficulty:
				Option 1: These dependencies must be described in the comment header of the example main file/s.
				Option 2: Each Pmod's sdk_sources folder must contain a README with specific instructions on how to create a new application project that supports the example.
				Option 3: Each Pmod's sdk_sources folder must contain an XSCT script that creates a new application project containing the example code.
				Option 4: Create application templates for each Pmod that can be used in the File / Create New / Application Project flow.
			The SDK files in the example Pmods are only minimally edited from their form in vivado-library.
		sources/:
			All files in this folder are added to the sources_1 fileset when the create_hier.tcl script is run.
			Typically contains Verilog/VHDL source files for RTL module references.
			All files (and modules therein) contained in this folder MUST have unique names, to avoid naming conflicts with files and IPs that may already exist in a project.
				My preference is to use Pmod***_ as a required prefix.
		constrs/: (WIP)
			Contains XDC files.
			May only be able to constrain nets internal to the hierarchy described.
			Likely will require the user to edit after a design is completed.
			Board flow does work with hierarchical blocks, as long as the Pmod Bridge contained in the hierarchy has its "CONFIG.PMOD" parameter set to "Custom" by the bd.tcl script.
	repo
		Contains a subset of vivado-library (perhaps should be a submodule).
		Must contain all custom IP/IF dependencies of the Pmod hierarchies.
		Currently only contains the Pmod interface and a modified version of the Pmod Bridge IP (I added a "USE_PULLUPS" parameter so that the bridge can handle I2C properly).
		