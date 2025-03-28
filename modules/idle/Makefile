ROOTDIR = ../..
include $(ROOTDIR)/config.mk

LDFLAGS += -e Init

.PHONY: clean

$(ROOTDIR)/idle.sys: $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o

