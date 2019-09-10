.PHONY: nx pc

export LANG=en

nx:
	@$(MAKE) $(filter-out $@,$(MAKECMDGOALS)) --no-print-directory -f Makefile.nx

pc:
	@$(MAKE) $(filter-out $@,$(MAKECMDGOALS)) --no-print-directory -f Makefile.pc

%:
	@:
