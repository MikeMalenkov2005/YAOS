#### TARGET CONFIG ####

ARCH = x86

KERNEL_NAME = YAOS
KERNEL_VERSION = 0.1
KERNEL = $(KERNEL_NAME)-$(KERNEL_VERSION)
KERNEL_FILE = $(ROOTDIR)/$(KERNEL).sys

MODULES = idle

#### PROJECT FILES ####

SOURCES = $(wildcard *.c)

OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

#### TOOLS CONFIG ####

CC = clang
CFLAGS = -I$(ROOTDIR)/include -ffreestanding -fno-builtin -nostdlib -nostdinc -fno-stack-protector -Wall -Wextra -Werror -Os -c

LD = ld.lld

#### TARGET SPECIFIC ####

include $(ROOTDIR)/config/$(ARCH).mk

MODULE_FILES = $(patsubst %,$(ROOTDIR)/%.sys,$(MODULES))

