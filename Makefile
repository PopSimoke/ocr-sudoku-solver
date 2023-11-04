# Makefile for all

SUBDIRS :=  src/GUI src/image_processing src/neural_network src/sudoku_solver src/XOR

.PHONY: all $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: clean

clean:
	for dir in $(SUBDIRS); do \
        	$(MAKE) -C $$dir clean; \
    	done
