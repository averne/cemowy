.PHONY: all libs clean mrproper nx pc

all: nx pc

libs: nx pc

clean: nx pc

mrproper: nx pc
	@rm -rf build out

nx:
	@$(MAKE) $(filter-out $@,$(MAKECMDGOALS)) --no-print-directory -f Makefile.nx

pc:
	@$(MAKE) $(filter-out $@,$(MAKECMDGOALS)) --no-print-directory -f Makefile.pc

%:
	@:
