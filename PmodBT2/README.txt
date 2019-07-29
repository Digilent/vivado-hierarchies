Hierarchy Port Requirements
---------------------------
* `*_interrupt`: These ports do not need to be connected to an interrupt controller.

Constraints
-----------
* When using the Board Flow, no additional constraints are required.
* When not using the Board Flow, template constraints for the Pmod_out port can be found in the imported "PmodBT2_*.xdc" file.