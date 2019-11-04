===============================================================
 
Makefile for building MSP Code Examples in command line
environement using the GCC ARM Embedded Toolchain

GCC ARM Embedded Homepage:

    https://launchpad.net/gcc-arm-embedded

===============================================================

Makefile usage:

    make DEVICE=<deviceName> EXAMPLE=<exampleName>
    e.g. make DEVICE=MSP432P401R


Debug with GDB:

    make debug DEVICE=<deviceName>

===============================================================

This Makefile assumes the GCC ARM Embedded Toolchain is install directory
<install_dir>/arm-compiler. To explicitly specify the GCC install path, use:

    make DEVICE=<deviceName> \
         GCC_BIN_DIR=<path_To_GCC_ARM_Root_containing_bin> \
		 GCC_INC_DIR=<path_To_GCC_ARM_Root_containing_include_files> 

===============================================================