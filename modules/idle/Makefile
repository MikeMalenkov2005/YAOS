ROOTDIR = ../..
include $(ROOTDIR)/config.mk

.PHONY: clean

$(IDLEMOD): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o
