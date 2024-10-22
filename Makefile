B = build
N = YAOS

AS_i386 = nasm -f elf32
CC_i386 = gcc -m32 -march=i386 -c -fno-pie -fno-stack-protector -fno-builtin -ffreestanding -I common
LD_i386 = ld -m elf_i386

common_hdr = $(wildcard common/*.h)
common_src = $(wildcard common/*.c)

i386_hdr = $(wildcard i386/*.h)
i386_src = $(wildcard i386/*.c)
i386_asm = $(wildcard i386/*.asm)
i386_obj = $(patsubst i386/%.asm,$B/i386/asm/%.o,$(i386_asm)) \
					 $(patsubst i386/%.c,$B/i386/system/%.o,$(i386_src)) \
					 $(patsubst common/%.c,$B/i386/common/%.o,$(common_src))

.PHONY: build-i386
build-i386: $N-i386.sys

.PHONY: run-i386
run-i386: $N-i386.sys
	qemu-system-i386 -kernel $^

fd1440.img: $N-i386.sys
	cp $^ boot/minboot/KERNEL.SYS
	$(MAKE) -C boot/minboot fd1440.img
	mv boot/minboot/fd1440.img $@
	$(MAKE) -C boot/minboot clean

$N-i386.sys: i386/kernel.ld $(i386_obj)
	$(LD_i386) -o $@ -T $^

$B/i386/asm/%.o: i386/%.asm
	mkdir -p $(dir $@)
	$(AS_i386) -o $@ $<

$B/i386/system/%.o: i386/%.c $(i386_hdr) $(common_hdr)
	mkdir -p $(dir $@)
	$(CC_i386) -I i386 -o $@ $<

$B/i386/common/%.o: common/%.c $(common_hdr)
	mkdir -p $(dir $@)
	$(CC_i386) -o $@ $<

.PHONY: clean
clean:
	rm -rf $B $N-* *.img

