# Pmod Hierarchies

## Introduction

This repo contains an experimental system for quickly adding support for a Pmod to a block-diagram based Vivado project. This is intended to replace or coexist with the Pmod IP cores in [vivado-library](https://github.com/Digilent/vivado-library). When compared to Pmod IP cores, this system is intended to (1) make the process of adding support for a new Pmod easier, (2) require significantly less maintenance with each Vivado release, and (3) allow for user modifications to the design.

The next two sections of this document (*Adding a Pmod Hierarchy to your Design* and *Creating a New Pmod Hierarchy*) detail how this repo is intended to be worked with.

The final section of this document (*Directory Structure - Details*), discusses how this repo is put together. It also presents some design decisions that may cause issues and potential alternatives.

-----------------------------------------
## Adding a Pmod Hierarchy to your Design


1. Clone this repository, using the following command:
   - `git clone --recursive https://git.digilentinc.com/artvvb/Hierarchical-Pmods`
   - Take note of the path to the cloned repository, it will be used later.
1. Launch Vivado.
1. Create or open the Vivado Project you wish to use the Pmod in.
1. Create or open the project's Block Design.
1. In Vivado's TCL Console, enter the following command:
   - `source (path to cloned repo)/(Pmod of choice)/create_hier.tcl`
   - When the script is finished running, the block design will contain a *Hierarchical Block*, named after the chosen Pmod, with several IP inside of it. The IP will be connected to one another and to the block's ports and pins.
1. Make sure your design has a processor, and a peripheral that can be used for stdout. In the case of Microblaze, a UART IP must be connected to the board's USBUART interface. In the case of Zynq, the MIO UART is used, and does not need to be configured.
1. Check the README.txt file, found in your Pmod's folder in this repo, to find additional information about how the ports of the Hierarchy must be connected to the rest of your design. With this information in mind:
   - Connect all of the Hierarchy's AXI interfaces to the processor in your design by clicking on *Run Connection Automation*. and checking all appropriate boxes.
   - Connect any interrupts the Hierarchy may have to the appropriate interrupt controller, an AXI INTC IP (for Microblaze designs), the Zynq Processing System's irq\_f2p port (for Zynq designs).
   - Connect any additional clocks to clocks generated by a Memory Interface Generator or a Clocking Wizard (for Microblaze designs), or a Zynq Processing System (for Zynq designs).
1. The next step, constraining the Hierarchy's Pmod\_out port, has two different workflows.
   - If you selected a board while creating your project, you can use the *Board Flow* for this step:
      1. Go to Vivado's *Board* tab and find the Pmod connector you wish to connect to the Hierarchy. Right click on the entry, typically named something like "Connector JA", and select *Connect Board Component*. In the popup window, under *Connect to existing IP*, select the "Pmod\_out" interface of the Hierarchy's Pmod Bridge IP. Click *OK*.
      1. Make sure to validate your block design, save it, and create an HDL wrapper file.
      1. Check the README.txt file in your Pmod's folder for additional instructions to determine whether any additional constraints are required.
   - If you selected a part instead of a board, or otherwise do not wish to use the *Board Flow*, you will need to create a port to connect to the Hierarchy's Pmod\_out port and constrain it.
      1. Select the Pmod\_out port, then right click on it and select *Make External*. Select the newly created external interface port (named something like "Pmod\_out\_0") in the design, and give it a memorable name.
      1. Validate your design, and save it. If your block design doesn't a wrapper file, right click on the design in the sources pane and select *Create HDL Wrapper*.
      1. When the Hierarchy was created, a constraint file, named "(Default Hierarchy Name)\_Pmod\_out.xdc", was imported into the project. This file contains the constraints required when not using the board flow. Uncomment each line starting with "set\_property" by removing the leading "#" symbol.
      1. The text "FIXME" appears in several places in the constraint file. These correspond to places where you will need to manually enter values specific to your board and design.
      1. Find the correct port names for your Pmod interface by reviewing the port map of the top module near the top of the HDL wrapper file. Enter these port names into the corresponding place in the constraint file (after get\_ports, near the end of each line).
      1. Download the master XDC file for your board. Master XDC files for Digilent boards can be found in the [digilent-xdc](https://github.com/Digilent/digilent-xdc) repository on Github. Find the LOC property values that correspond to the Pmod connector of your board that you wish to connect your Pmod to. Enter these values into the corresponding LOC fields in the hierarchy's constraint file.
1. Generate a bitstream.
1. Export Hardware to SDK.
1. Launch SDK.
1. Create a new Application Project using the "Empty Application" template. Make sure to check if the Pmod requires you to change any settings or add any libraries to the project. Any requirements are detailed in the README.txt file in the Pmod's sdk\_sources folder in this repo.
1. Copy all of the files from your Pmod's sdk\_sources folder into the empty application's src folder.
1. Build All.
1. Plug in your board.
1. Xilinx -> Program FPGA.
1. Run the project.

`FIXME` further expansion on some bullets needed

--------------------------------
## Creating a New Pmod Hierarchy


These steps assume that you have already created a working demo project that uses the Pmod Bridge IP to connect various other IPs and RTL modules to a Pmod port.

1. Create a sub-folder of this repo for the new hierarchy. The sub-folder should be named "Pmod*", where * is the name of the specific Pmod (ex: HYGRO).
1. Create folders named `constrs` and `sources` under the hierarchy's sub-folder.
1. Create a new hierarchical block, in Vivado IPI - right click on the block diagram's background and select "Create Hierarchy". Move all IPs and module references required to reproduce your design into the new hierarchical block.
1. Make sure that the Pmod Bridge IP in your design is not connected to a board interface, and that when re-customizing it, the IP Interface PmodOut is set to connect to the "Custom" Board Interface.
1. Run the command `write_bd_tcl -no_ip_version -hier_blks /(hierarchy) (repo path)/(Pmod*)/bd.tcl` in the Vivado TCL Console, replacing (hierarchy) with the name of the hierarchy you created in step 3, (repo path) with the path to this repository on your filesystem, and (Pmod*) with the name of the subfolder created in step 1.
1. Create template XDC files and add them to the constrs folder. Any lines that are required regardless of flow should be uncommented. Any lines not required in the Board Flow should be commented out by default. Any comments should use "##" to doubly comment out their line. "FIXME"s should be used anywhere that the user may need to enter a specific value (typically the name of the interface port and pin LOC properties).
1. Create a README.txt file under the hierarchy's folder in the repo that contains descriptions of any special cases required when a user is constraining or connecting the hierarchy. This typically includes whether interrupts are required, any frequency requirements for clock pins. `FIXME constraints`
1. Copy any files depended upon by an RTL module into the sources folder. Each file in this folder, and any modules contained therein, must have unique names starting with (Pmod*_), where * is the name of the specific Pmod (ex: HYGRO).
1. Create a folder named `sdk_sources` under the hierarchy's folder in the repo. Beneath this folder, create a "Pmod*" folder, which will contain any sources that can be used in *any* design. Any sources outside of this folder are treated as specific to a demo using only that Pmod. For example, The PmodBT2 hierarchy's sdk_sources folder contains a main.c file, and a PmodBT2 folder, containing a driver source and header file.
1. Create a README.txt file under sdk_sources, containing instructions on how to resolve any dependencies of the drivers and demo sources. Of the four examples initially present in this repo, only the PmodNAV has additional softwre requirements. As an example of requirements that must be stated here, some Pmods may require that their applications are built in C++.
1. Make sure that any custom IPs required by your hierarchy are present in this repository's repo folder. If they are not, add them. Note that since the -no_ip_version flag is used when creating BD TCL scripts, only the version of the IP with the highest version number is used.
1. Lastly, copy the create_hier.tcl script from another hierarchy into your hierarchy's folder.
1. Create a new project and run through the steps found Adding a Pmod Hierarchy to your Design to verify that your hierarchy works. Once it has been tested in hardware, commit and push with Git.

### Tips and Tricks

- *Module references* - custom Verilog or VHDL placed into a block diagram.
   - To create a new module reference, first create a new verilog source file. 
   - Determine what its input and output interfaces will be (for example, if it needs to take in one GPIO bus and output another).
   - Click on Language Templates under Project Manager in the Flow Navigator. You can find templates for grouping your module's ports as IPI interfaces under (Language) > IP Integrator HDL > Advanced Interfaces.
   - These modules can be added to a block diagram by right clicking on the background of the diagram and selecting *Add Module*.
   - I used these to create the *_remap modules in several of the example hierarchies, which allow me to connect interfaces to the Pmod Bridge that are not supported by default. For example, with the PmodNAV, connecting the three SPI CS pins of the AXI Quad SPI to the top and bottom GPIO interfaces of the Pmod Bridge. The original NAV IP used a large number of Concat and Slice IPs to achieve the same result.
   - *Note*: All errors must be corrected in these source files before they can be added to a block diagram as references. They may not even appear in the list of modules that can be added until all syntax errors are corrected - syntax errors that may not even be reported by Vivado.
   - *Note*: These modules may instantiate other Verilog/VHDL modules, but cannot instantiate any packaged IP, custom or from the catalog.
- *How the hierarchies' hardware was created*: I took a soft approach to creating the Pmod Hierarchies included as examples. They are nearly identical to the internals of the corresponding Pmod IP cores.
   - I first added a Pmod IP to a diagram, and opened it to edit in the IP Packager.
   - Using the list of IPs, found in the source hierarchy of the IP Packager project, under the Pmod*.v wrapper file, as reference, I added an instance of each IP to my block diagram, then configured it to match the one found in the IP.
   - I then opened the Pmod*.v wrapper, and used it to inform how I needed to connect each IP together.
   - *Note*: Some of the designs were tedious to recreate in full due to a large number of IPs used only to manage connections to the bridge. These I replaced with Pmod*_remap module references, described above.
   - *Note*: The PmodAD1 IP contained a custom AXI slave component. This I placed in a module reference as well, renaming and editing the files to fit with the "unique names" requirement specified in step 7 of the *Creating a New Pmod Hierarchy* section of this document.

--------------------------------
## Directory Structure - Details

- `repo/`
   - Contains a subset of vivado-library (perhaps should be a submodule).
   - *Rule*: Must contain all custom IP/IF dependencies of the Pmod hierarchies.
   - Several additional IP will likely need to be added as Pmods are added (The PWM 2.0 IP at least).
   - Currently only contains the Pmod interface and a modified version of the Pmod Bridge IP (I added a "USE_PULLUPS" parameter so that the IP can handle I2C properly).
- `Pmod*/`
   - `create_hier.tcl`
      - This script is what the user calls source on in the Vivado TCL Console to add a hierarchical block for that Pmod into their design.
      - Note that no relative paths are assumed; The user need not cd before sourcing it.
      - The script is written such that it can be identical in each Pmod* folder.
      - The script manages dependencies (IP, module reference sources) and names as a front-end to the bd.tcl script.
   - `bd.tcl`
      - Created using the command: `write_bd_tcl -no_ip_version -hier_blks (hierarchy) bd.tcl`.
      - When sourced by create_hier.tcl, creates a TCL proc that can be called to recreate the hierarchy.
   - `sdk_sources/`
      - Contains files to be copied into (application project)/src in SDK. This includes top level Pmod drivers (Pmod*.c, Pmod*.h) and example main.
      - Note that the SDK files in the initial set of Pmods are only minimally edited from their form in vivado-library.
      - I only changed (1) the names of the xparameters macros used in the main files and (2) #include paths to driver headers.
      - We must present project and workspace requirements to the user. Some Pmods require C++ application projects. Some Pmods require the math.h library. There are, I am sure, other dependencies that I have not thought of.
      - Some ideas, in ascending level of difficulty:
         - *Option 1*: Resolving any dependencies must be detailed in the comment header of the example main file/s. (This is what we -hopefully- already do)
         - *Option 2*: Each Pmod's sdk_sources folder must contain a README with specific instructions on how to create a new app and bsp that supports the example. (This is what I chose for the time being)
         - *Option 3*: Each Pmod's sdk_sources folder must contain an XSCT script that creates a new app and bsp containing the example code.
         - *Option 4*: Create *application templates* for each Pmod that can be used in the File / Create New / Application Project flow. [Xilinx document](https://www.xilinx.com/html_docs/xilinx2018_1/SDK_Doc/SDK_tasks/task_create_user_template_app.html)
      - *Rule*: Subfolders are intended to be included in any app using the Pmod, as such, the subfolders must have unique names.
   - `sources/`
      - All files in this folder are *added* (!) to the sources_1 fileset (and thereby the xil_defaultlib library) when the create_hier.tcl script is run. Adding sources may not be the best solution:
         - *Option 1*: Files are *added* to the project. Editing these files in the project will also affect the file in the repo, and in any reference of the hierarchy in any project using the repo. (This is what is implemented in this repo)
         - *Option 2*: Only import a file once. If any file with the same name exists, the file is skipped for import. This is the most straightforward alternative. Editing a file in a project affects each hierarchy in that project, but never the file in the repo.
         - *Option 3*: Import files and add a numeric suffix to the filenames and all modules within the files (avoiding this is why every instantiated IP has its own fileset).
         - *Option 4*: create new filesets for imported source files. This is probably a bad idea, and moves too much towards .
      - Typically contains Verilog/VHDL source files for RTL module references.
      - *Rule*: All files (and modules therein) contained in this folder MUST have unique names. This is meant to help to avoid naming conflicts with files and IPs that may already exist in a project. My preference is to require Pmod*_ as a prefix to module and file names.
   - `constrs/`
      - All files in this folder are *imported* to the constrs_1 fileset when the create_hier.tcl script is run. The imported file's name is modified such that it will not conflict with existing constraints.
      - *Rule*: Constraints that are to be used only when the board flow is not being used should be commented out by default. FIXMEs should be used to show where the user must manually enter values.
      - This system requires that the user edits imported XDCs after a design is completed in order to get the correct port names.
      - Board flow does work with hierarchical blocks, as long as the Pmod Bridge contained in the hierarchy has its "CONFIG.PMOD" parameter set to "Custom" by the bd.tcl script.