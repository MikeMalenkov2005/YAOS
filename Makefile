B = build

c_src = $(shell find kernel -name "*.c")
i386_c_obj = $(patsubst kernel/%.c, $B/i386/c/%.o, $(c_src))

i386_asm_obj = $(patsubst i386/%.asm, $B/i386/asm/%.o, $(shell find i386 -name "*.asm"))

.PHONY:
i386: $B/boot/floppy.bin kernel.ld $(i386_asm_obj) $(i386_c_obj)
	ld -m elf_i386 --oformat binary -o kernel.bin -T kernel.ld $(i386_asm_obj) $(i386_c_obj)
	cp $B/boot/floppy.bin fd1440.img
	mcopy -i fd1440.img kernel.bin ::/KERNEL.SYS
	mattrib -i fd1440.img +rsh ::/KERNEL.SYS

.PHONY: clean
clean:
	rm -rf $B

$B/i386/asm/%.o: i386/%.asm
	mkdir -p $(dir $@)
	nasm -f elf32 -o $@ $^

$B/i386/c/%.o: kernel/%.c
	mkdir -p $(dir $@)
	gcc -m32 -march=i386 -c -Os -fno-pie -fno-stack-protector -fno-builtin -ffreestanding -o $@ $^

$B/%.bin: %.asm
	mkdir -p $(dir $@)
	nasm -f bin -o $@ $^

