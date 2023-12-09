# Makefile for all

SUBDIRS :=  ./src/GUI ./src/image_processing ./src/neural_network ./src/sudoku_solver

.PHONY: all $(SUBDIRS)

all: $(SUBDIRS) create display
	
create:	
	echo -e "cd src/GUI\n./ocr_gui\ncd ../../" > start.sh
	chmod +x start.sh
	echo -e "cd src/neural_network\n./number_recognition\ncd ../../" > neural_network.sh
	chmod +x neural_network.sh

display:
	clear
	cat welcome.txt

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: clean

clean:
	for dir in $(SUBDIRS); do \
        	$(MAKE) -C $$dir clean; \
    	done
	${RM} start.sh
	${RM} neural_network.sh
