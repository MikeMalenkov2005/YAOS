ROOTDIR = ../..
include $(ROOTDIR)/config.mk

LDFLAGS += -e Init

.PHONY: clean

$(ROOTDIR)/game.sys: $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o

