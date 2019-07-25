Pmod Hierarchies
================

Introduction
------------

--------------------------------------
Adding a Pmod Hierarchy to your Design
--------------------------------------

-----------------------------
Creating a New Pmod Hierarchy
-----------------------------

-------------------
Directory Structure
-------------------
* <code>repo/</code>
  * Contains a subset of vivado-library (perhaps should be a submodule).
  * *Rule*: Must contain all custom IP/IF dependencies of the Pmod hierarchies.
  * Several additional IP will likely need to be added as Pmods are added (The PWM 2.0 IP at least).
  * Currently only contains the Pmod interface and a modified version of the Pmod Bridge IP (I added a "USE_PULLUPS" parameter so that the IP can handle I2C properly).
* <code>Pmod*/</code>
  * <code>create_hier.tcl</code>
    * This script is what the user calls source on in the Vivado TCL Console to add a hierarchical block for that Pmod into their design.
    * Note that no relative paths are assumed; The user need not cd before sourcing it.
    * The script is written such that it can be identical in each Pmod* folder.
    * The script manages dependencies (IP, module reference sources) and names as a front-end to the bd.tcl script.
  * <code>bd.tcl</code>
    * Created using the command: <code>write_bd_tcl -no_ip_version -hier_blks (hierarchy) bd.tcl</code>.
    * When sourced, creates a TCL proc that can be called to recreate the hierarchy.
    * Further info on this in "Usage" section below.
  * <code>sdk_sources/</code>
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
  * <code>sources/</code>
    * All files in this folder are *added* (!) to the sources_1 fileset when the create_hier.tcl script is run. Adding sources may not be the best solution:
      * *Option 1*: Files are *added* to the project. Editing these files in the project will also affect the file in the repo, and in any reference of the hierarchy in any project using the repo. (This is what is implemented in this repo)
      * *Option 2*: Only import a file once. If any file with the same name exists, the file is skipped for import. This is the most straightforward alternative. Editing a file in a project affects each hierarchy in that project, but never the file in the repo.
      * *Option 3*: Import files and add a numeric suffix to the filenames and all modules within the files (avoiding this is why every instantiated IP has its own fileset).
      * *Option 4*: create new filesets for imported source files. This is probably a bad idea, and moves too much towards .
    * Typically contains Verilog/VHDL source files for RTL module references.
    * *Rule*: All files (and modules therein) contained in this folder MUST have unique names. This is meant to help to avoid naming conflicts with files and IPs that may already exist in a project. My preference is to require Pmod*_ as a prefix to module and file names.
  * <code>constrs/</code> (WIP)
    * Contains XDC files.
    * This system may only be able to constrain nets internal to the hierarchy described.
    * This system likely will require the user to edit any added XDCs after a design is completed in order to get the correct port names.
    * Board flow does work with hierarchical blocks, as long as the Pmod Bridge contained in the hierarchy has its "CONFIG.PMOD" parameter set to "Custom" by the bd.tcl script.

----