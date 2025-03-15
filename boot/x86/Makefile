ROOTDIR = ../..
include $(ROOTDIR)/config.mk

GRUBIMG = grub.img
GRUBCFG = grub.cfg
MODULES = biosdisk part_msdos fat multiboot configfile ls cat help

FLOPPY = floppy.img

.PHONY: clean

# all: $(KERNEL).img $(KERNEL).iso

# $(KERNEL).iso: $(KERNEL)

$(KERNEL).img: $(KERNEL) $(FLOPPY) $(IDLEMOD)
	cp $(FLOPPY) $@
	mcopy -i $@ $(KERNEL) ::/YAOS.SYS
	mcopy -i $@ $(IDLEMOD) ::/IDLE.MOD

$(FLOPPY): $(GRUBIMG) $(GRUBCFG)
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=/usr/lib/grub/i386-pc/boot.img of=$@ conv=notrunc
	dd if=$(GRUBIMG) of=$@ conv=notrunc seek=1
	mformat -i $@ -kR $(shell echo $$(($(shell ls --block-size=512 -s $(GRUBIMG) | sed 's/\s.*$$//') + 2)))
	mcopy -i $@ $(GRUBCFG) ::/GRUB.CFG

$(GRUBIMG):
	grub-mkimage -p / -C auto -O i386-pc -o $@ $(MODULES)

clean:
	rm -f $(GRUBIMG)
