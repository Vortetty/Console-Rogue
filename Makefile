#
# Edit makefile_real to change rules and config.
# Edit the below var to change the multiplier for the number of threads (by default it is 1, meaning it will be the number of cores you have).
#
coreMult = 2

CORES := $$(( $(shell nproc) * $(coreMult) ))
UNAME_S := $(shell uname -s)

# Linuhh how install <X>
ifeq ($(UNAME_S),Linux)

build:
	@make build -j$(CORES) -f makefile_real --no-print-directory $@

libs:
	@make libs -j$(CORES) -f makefile_real --no-print-directory $@

run:
	@make run -j$(CORES) -f makefile_real --no-print-directory $@

test:
	@make test -j$(CORES) -f makefile_real --no-print-directory $@

gdb:
	@make gdb -j$(CORES) -f makefile_real --no-print-directory $@

setGdbOpts:
	@make setGdbOpts -j$(CORES) -f makefile_real --no-print-directory $@

clean:
	@make clean -j$(CORES) -f makefile_real --no-print-directory $@

genmake:
	@make genmake -j$(CORES) -f makefile_real --no-print-directory $@

modules:
	@make modules -j$(CORES) -f makefile_real --no-print-directory $@

endif

# MacOShit
ifeq ($(UNAME_S),Darwin)

genmake:
	@make mac_genmake -j$(CORES) -f makefile_real --no-print-directory $@

endif

# Windex
ifeq ($(OS),Windows_NT)

build:
	@make win_build -j$(CORES) -f makefile_real --no-print-directory $@

test:
	@make win_test -j$(CORES) -f makefile_real --no-print-directory $@

genmake:
	@make win_genmake -j$(CORES) -f makefile_real --no-print-directory $@

endif
