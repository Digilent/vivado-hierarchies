Pmod Hierarchies
================

TODO: constraints.
TODO: SW READMEs.
TODO: HW READMEs.

Introduction
------------

This repo contains an experimental system for quickly adding support for a Pmod to a block-diagram based Vivado project. This is intended to replace or coexist with the Pmod IP cores in [vivado-library](https://github.com/Digilent/vivado-library). When compared to Pmod IP cores, this system is intended to (1) make the process of adding support for a new Pmod easier, (2) require significantly less maintenance with each Vivado release, and (3) allow for user modifications to the design.

--------------------------------------
Adding a Pmod Hierarchy to your Design
--------------------------------------

1. Clone this repository, using the following command:

    `git clone --recursive https://git.digilentinc.com/artvvb/Hierarchical-Pmods`

    Take note of the path to the cloned repository, it will be used later.

1. Launch Vivado.

1. Create or open the Vivado Project you wish to use the Pmod in.

1. Create or open the project's Block Design.

1. In Vivado's TCL Console, enter the following command:

    `source (path to cloned repo)/(Pmod of choice)/create_hier.tcl`

    When the script is finished running, the block design will contain a *Hierarchical Block*, named after the chosen Pmod, with several IP inside of it. The IP will be connected to one another and to the block's ports and pins.

1. Make sure your design has a processor, and a peripheral that can be used for stdout. In the case of Microblaze, a UART IP must be connected to the board's USBUART interface. In the case of Zynq, the MIO UART is used, and does not need to be configured.

1. Connect all of the Hierarchy's AXI interfaces to the processor in your design by clicking on *Run Connection Automation*. and checking all appropriate boxes.

1. Connect any interrupts the Hierarchy may have to the appropriate interrupt interface. For Microblaze, an AXI INTC, for Zynq the irq_f2p port.

1. Connect any additional clocks to clocks of the frequencies specified in the Pmod's README.txt file, found in its folder in this repo.

1. The next step, constraining the Hierarchy's Pmod_out port, has several different workflows.

    * If you selected a board while creating the project, you can use the *Board Flow* for this step. Go to Vivado's *Board* tab and find the Pmod connector you wish to connect to the Hierarchy. Right click on the entry, typically named something like "Connector JA", and select *Connect Board Component*. In the popup window, under *Connect to existing IP*, select the "Pmod_out" interface of the Hierarchy's Pmod Bridge IP. Click *OK*.

	    Make sure to validate your block design, save it, and create an HDL wrapper file.

	    Using this method, you will not have to manually create or edit constraints for the Pmod's pins.

	* If you selected a part instead of a board, or otherwise do not wish to use the *Board Flow*, you will need to create a port to connect to the Hierarchy's Pmod_out port. Select the Pmod_out port, then right click on it and select *Make External*. Select the newly created external interface port (named something like "Pmod_out_0") in the design, and give it a memorable name.

	    Validate your design, and save it. If your block design doesn't a wrapper file, right click on the design in the sources pane and select *Create HDL Wrapper*.

		Download the master XDC file for your board, and add it to the project. Master XDC files for Digilent boards can be found in the [digilent-xdc](https://github.com/Digilent/digilent-xdc) repository on Github.
		
		Open the master XDC and HDL wrapper files. Find the template constraints for the Pmod connector you wish to plug the Pmod into. Find the ports that correspond to the external interface ports in the HDL wrapper's port list (typically named things like "(port)_pin1_io"). Edit the XDC file so that the names in the "get_ports" calls for the Pmod connector correspond to the correct port names found in the HDL wrapper.

		If your Pmod uses I2C, add the following lines to the master XDC file, replacing (pmod) with the name of the interface as found in your HDL wrapper file:

		<code>
		set_property -dict {PULLUP TRUE} [get_ports (pmod)_pin3_io];<br/>
		set_property -dict {PULLUP TRUE} [get_ports (pmod)_pin4_io];
		</code>

		If your Pmod requires any other constraints, such as... `FIXME`
		
		`FIXME` validate flow

1. Generate a bitstream.

1. Export Hardware to SDK.

1. Launch SDK.

1. Create a new Application Project using the "Empty Application" template. Make sure to check if the Pmod requires you to change any settings or add any libraries to the project. Any requirements are detailed in the README.txt file in the Pmod's sdk_sources folder in this repo.

1. Copy all of the files from your Pmod's sdk_sources folder into the empty application's src folder.

1. Build All.

1. Plug in your board.

1. Xilinx -> Program FPGA.

1. Run the project.

`FIXME` further expansion on some bullets needed

-----------------------------
Creating a New Pmod Hierarchy
-----------------------------

These steps assume that you have already created a working demo project that uses the Pmod Bridge IP to connect various other IPs and RTL modules to a Pmod port.

`FIXME` incomplete

-------------------
Directory Structure
-------------------
* `repo/`
    * Contains a subset of vivado-library (perhaps should be a submodule).
    * *Rule*: Must contain all custom IP/IF dependencies of the Pmod hierarchies.
    * Several additional IP will likely need to be added as Pmods are added (The PWM 2.0 IP at least).
    * Currently only contains the Pmod interface and a modified version of the Pmod Bridge IP (I added a "USE_PULLUPS" parameter so that the IP can handle I2C properly).
* `Pmod*/`
    * `create_hier.tcl`
        * This script is what the user calls source on in the Vivado TCL Console to add a hierarchical block for that Pmod into their design.
        * Note that no relative paths are assumed; The user need not cd before sourcing it.
        * The script is written such that it can be identical in each Pmod* folder.
        * The script manages dependencies (IP, module reference sources) and names as a front-end to the bd.tcl script.
    * `bd.tcl`
        * Created using the command: `write_bd_tcl -no_ip_version -hier_blks (hierarchy) bd.tcl`.
        * When sourced, creates a TCL proc that can be called to recreate the hierarchy.
        * Further info on this in "Usage" section below.
    * `sdk_sources/`
        * Contains files to be copied into (application project)/src in SDK. This includes top level Pmod drivers (Pmod*.c, Pmod*.h) and example main.
        * Note that the SDK files in the initial set of Pmods are only minimally edited from their form in vivado-library.
        * I only changed (1) the names of the xparameters macros used in the main files and (2) #include paths to driver headers.
        * TODO: how to handle project settings and libraries as dependencies? Some Pmods require C++ application projects. Some Pmods require the math.h library.
        * Some ideas, in ascending level of difficulty:
            * *Option 1*: Resolving any dependencies must be detailed in the comment header of the example main file/s. (This is what we -hopefully- already do)
            * *Option 2*: Each Pmod's sdk_sources folder must contain a README with specific instructions on how to create a new app and bsp that supports the example.
            * *Option 3*: Each Pmod's sdk_sources folder must contain an XSCT script that creates a new app and bsp containing the example code.
            * *Option 4*: Create application templates for each Pmod that can be used in the File / Create New / Application Project flow.
        * *Rule*: Subfolders are intended to be included in any app using the Pmod, as such, the subfolders must have unique names.
    * `sources/`
        * All files in this folder are *added* (!) to the sources_1 fileset when the create_hier.tcl script is run. Adding sources may not be the best solution:
            * *Option 1*: Files are *added* to the project. Editing these files in the project will also affect the file in the repo, and in any reference of the hierarchy in any project using the repo. (This is what is implemented in this repo)
            * *Option 2*: Only import a file once. If any file with the same name exists, the file is skipped for import. This is the most straightforward alternative. Editing a file in a project affects each hierarchy in that project, but never the file in the repo.
            * *Option 3*: Import files and add a numeric suffix to the filenames and all modules within the files (avoiding this is why every instantiated IP has its own fileset).
            * *Option 4*: create new filesets for imported source files. This is probably a bad idea, and moves too much towards .
        * Typically contains Verilog/VHDL source files for RTL module references.
        * *Rule*: All files (and modules therein) contained in this folder MUST have unique names. This is meant to help to avoid naming conflicts with files and IPs that may already exist in a project. My preference is to require Pmod*_ as a prefix to module and file names.
    * `constrs/` (WIP)
        * Contains XDC files.
        * This system may only be able to constrain nets internal to the hierarchy described.
        * This system likely will require the user to edit any added XDCs after a design is completed in order to get the correct port names.
        * Board flow does work with hierarchical blocks, as long as the Pmod Bridge contained in the hierarchy has its "CONFIG.PMOD" parameter set to "Custom" by the bd.tcl script.

----