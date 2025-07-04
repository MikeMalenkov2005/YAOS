ROOTDIR = ../..
include $(ROOTDIR)/config.mk

OBJECTS += arch/$(ARCH).o
LDFLAGS += -e Init

.PHONY: clean

$(ROOTDIR)/pci.sys: $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o

