ROOTDIR = .
include $(ROOTDIR)/config.mk

.PHONY: boot kernel modules clean

boot: kernel modules
	$(MAKE) -C boot/$(ARCH)

kernel:
	$(MAKE) -C kernel

modules:
	$(MAKE) -C modules

clean:
	$(MAKE) -C modules clean
	$(MAKE) -C kernel clean
	$(MAKE) -C boot/$(ARCH) clean
	rm -f $(KERNEL) *.mod *.iso *.img
