MODULES = idle
CLEANS = $(patsubst %,%.clean,$(MODULES))

.PHONY: all $(MODULES) clean $(CLEANS)

all: $(MODULES)

$(MODULES):
	$(MAKE) -C $@

clean: $(CLEANS)

$(CLEANS):
	$(MAKE) -C $(patsubst %.clean,%,$@) clean
