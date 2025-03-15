Yet Another Operating System
============================

YAOS is a simple microkernel operating system written to learn about computers.

Microkernel
-----------

Supported Targets
-----------------

- 32-bit IBM compatible PC (x86)

The target can be selected by changing the ARCH variable in the config.mk file.

Build Environment
-----------------

- **Shell:** Any UNIX-like shell (with rm command)
- **Build system:** Make
- **Compiler:** Clang
- **Assemblers:** NASM for x86
- **Additional tools:** GRUB for generating bootloaders

The `make` command builds everything required for the current target.
The `make clean` command deletes everything created during the build process.

