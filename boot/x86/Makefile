ROOTDIR = ../..
include $(ROOTDIR)/config.mk

GRUBIMG = grub.img
GRUBCFG = grub.cfg
GRUBMOD = biosdisk part_msdos fat multiboot configfile ls cat help

FLOPPY = floppy.img

.PHONY: clean

# all: $(ROOTDIR)/$(KERNEL).img $(ROOTDIR)/$(KERNEL).iso

# $(ROOTDIR)/$(KERNEL).iso: $(KERNEL_FILE)

$(ROOTDIR)/$(KERNEL).img: $(KERNEL_FILE) $(FLOPPY) $(MODULE_FILES)
	cp $(FLOPPY) $@
	mcopy -i $@ $(KERNEL_FILE) ::/$(KERNEL_NAME).sys
	mcopy -i $@ $(MODULE_FILES) ::/

$(FLOPPY): $(GRUBIMG) $(GRUBCFG)
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=/usr/lib/grub/i386-pc/boot.img of=$@ conv=notrunc
	dd if=$(GRUBIMG) of=$@ conv=notrunc seek=1
	mformat -i $@ -kR $(shell echo $$(($(shell ls --block-size=512 -s $(GRUBIMG) | sed 's/\s.*$$//') + 2)))
	mcopy -i $@ $(GRUBCFG) ::/

$(GRUBIMG):
	grub-mkimage -p / -C auto -O i386-pc -o $@ $(GRUBMOD)

clean:
	rm -f $(GRUBIMG)
