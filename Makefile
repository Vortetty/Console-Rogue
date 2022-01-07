#
# Edit makefile_real to change rules and config.
# Edit the below var to change the multiplier for the number of threads (by default it is 1, meaning it will be the number of cores you have).
#
coreMult = 2

CORES := $$(( $(shell nproc) * $(coreMult) ))
UNAME_S := $(shell uname -s)

# Linuhh how install <X>
ifeq ($(UNAME_S),Linux)

KNOWN = .PHONY build libs run test gdb setGdbOpts clean genmake modules

.PHONY:
	@make .PHONY -j$(CORES) -f makefile_real --no-print-directory $(filter-out $(KNOWN), $@)

build:
	@make build -j$(CORES) -f makefile_real --no-print-directory $(filter-out $(KNOWN), $@)

libs:
	@make libs -j$(CORES) -f makefile_real --no-print-directory $(filter-out $(KNOWN), $@)

run:
	@make run -j$(CORES) -f makefile_real --no-print-directory $(filter-out $(KNOWN), $@)

test:
	@make test -j$(CORES) -f makefile_real --no-print-directory $(filter-out $(KNOWN), $@)

gdb:
	@make gdb -j$(CORES) -f makefile_real --no-print-directory $(filter-out $(KNOWN), $@)

setGdbOpts:
	@make setGdbOpts -j$(CORES) -f makefile_real --no-print-directory $(filter-out $(KNOWN), $@)

clean:
	@make clean -j$(CORES) -f makefile_real --no-print-directory $(filter-out $(KNOWN), $@)

genmake:
	@make genmake -j$(CORES) -f makefile_real --no-print-directory $(filter-out $(KNOWN), $@)

modules:
	@make modules -j$(CORES) -f makefile_real --no-print-directory $(filter-out $(KNOWN), $@)

endif

# MacOShit
ifeq ($(UNAME_S),Darwin)

KNOWN = genmake

genmake:
	@make mac_genmake -j$(CORES) -f makefile_real --no-print-directory $(filter-out $(KNOWN), $@)

endif

# Windex
ifeq ($(OS),Windows_NT)

KNOWN = build test clean genmake

build:
	@make win_build -j$(CORES) -f makefile_real --no-print-directory $(filter-out $(KNOWN), $@)

test:
	@make win_test -j$(CORES) -f makefile_real --no-print-directory $(filter-out $(KNOWN), $@)

clean:
	@make win_clean -j$(CORES) -f makefile_real --no-print-directory $(filter-out $(KNOWN), $@)

genmake:
	@make win_genmake -j$(CORES) -f makefile_real --no-print-directory $(filter-out $(KNOWN), $@)

endif
