Yet Another Operating System
============================

YAOS is a simple microkernel operating system written to learn about computers.

Microkernel
-----------

Being a microkernel YAOS implements only **processor initialization**,
**memory management**, **program loading**, **multitasking** and **message passing**.

Everything else is provided via **system modules**, which run in usermode,
but have special permissions (like port and memory mapped io).

System Modules
--------------

- **idle.sys** is the first module to be loaded by YAOS. It acts as a parent for
all other modules and is important for task switching.

Supported Targets
-----------------

- 32-bit IBM compatible PC (x86)

The target can be selected by changing the ARCH variable in the config.mk file.

Build Environment
-----------------

- **Shell:** Any UNIX-like shell (with rm command)
- **Build system:** Make
- **Compiler:** Clang (LLVM)
- **Linker:** ld.lld (LLVM)
- **Assemblers:** NASM for x86
- **Additional tools:** GRUB for generating bootloaders

For building to be successful every tool listed above must be added to PATH.

The `make` command builds everything required for the current target.
The `make clean` command deletes everything created during the build process.

TODO List
---------

- Add more system calls
- Create a FDD driver
- Create a HDD driver
- Create a VFS (virtual file system) driver
- Create a vFAT driver
- Create a SO (shared object) driver
- Write some applications
- Check PCI driver reliability
- Create a Video driver

