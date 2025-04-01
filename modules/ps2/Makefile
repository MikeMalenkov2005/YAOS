ROOTDIR = ../..
include $(ROOTDIR)/config.mk

LDFLAGS += -e Init

.PHONY: clean

$(ROOTDIR)/ps2.sys: $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o

