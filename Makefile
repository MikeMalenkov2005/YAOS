B = build

kernel_src = $(shell find kernel -name "*.c")

i386_obj := $(patsubst i386/%.asm, $B/i386/asm/%.o, $(shell find i386 -name "*.asm"))
i386_obj += $(patsubst i386/%.c, $B/i386/c/%.o, $(shell find i386 -name "*.c"))
i386_obj += $(patsubst kernel/%.c, $B/i386/kernel/%.o, $(kernel_src))

.PHONY:
i386: $B/boot/floppy.bin kernel.ld $(i386_obj)
	ld -m elf_i386 --oformat binary -o kernel.bin -T kernel.ld $(i386_obj)
	cp $B/boot/floppy.bin fd1440.img
	mcopy -i fd1440.img kernel.bin ::/KERNEL.SYS
	mattrib -i fd1440.img +rsh ::/KERNEL.SYS

.PHONY: clean
clean:
	rm -rf $B

$B/i386/asm/%.o: i386/%.asm
	mkdir -p $(dir $@)
	nasm -f elf32 -o $@ $^

$B/i386/c/%.o: i386/%.c
	mkdir -p $(dir $@)
	gcc -m32 -march=i386 -mgeneral-regs-only -c -Os -fno-pie -fno-stack-protector -fno-builtin -ffreestanding -o $@ $^

$B/i386/kernel/%.o: kernel/%.c
	mkdir -p $(dir $@)
	gcc -m32 -march=i386 -mgeneral-regs-only -c -Os -fno-pie -fno-stack-protector -fno-builtin -ffreestanding -o $@ $^

$B/%.bin: %.asm
	mkdir -p $(dir $@)
	nasm -f bin -o $@ $^

