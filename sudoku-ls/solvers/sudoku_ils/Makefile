############################# Makefile ##########################
# Part of the Source Code for the Sudoku solver described in
# "A Hybrid Heuristic Approach for the Sudoku Problem"
#

MAKE = make
CLEAN = make clean

all: copy

copy: build_ilsSolver
	cp ilsSolver/ilsSolver build/

clean: clean_ilsSolver
	rm -rf build/*
 
build_ilsSolver:
	cd ilsSolver && $(MAKE) && cd ..
	
clean_ilsSolver:
	cd ilsSolver && $(CLEAN) && cd ..
