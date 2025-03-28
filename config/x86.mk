CFLAGS += -target i686-pc-none-elf
LDFLAGS += -m elf_i386

ELFASM = nasm -f elf32
RAWASM = nasm -f bin
