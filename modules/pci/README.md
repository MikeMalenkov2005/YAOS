PCI Module
==========

This module is resposible for interacting with PCI configuration space.

Message Format
--------------

The PCI message format is structure `PCI_COMMAND`,
which is defined in `sys/pci.h`.

Supported Commands
------------------

1. Read config register (register offset) - reads upto 4 bytes.

All other command numbers return zeros.

