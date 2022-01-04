#
# Edit makefile_real to change rules and config.
# Edit the below var to change the multiplier for the number of threads (by default it is 1, meaning it will be the number of cores you have).
#
coreMult = 2

CORES := $$(( $(shell nproc) * $(coreMult) ))
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)

build:
	@make build -j$(CORES) -f makefile_real --no-print-directory $@=

libs:
	@make libs -j$(CORES) -f makefile_real --no-print-directory $@=

run:
	@make run -j$(CORES) -f makefile_real --no-print-directory $@=

test:
	@make test -j$(CORES) -f makefile_real --no-print-directory $@=

gdb:
	@make gdb -j$(CORES) -f makefile_real --no-print-directory $@=

setGdbOpts:
	@make setGdbOpts -j$(CORES) -f makefile_real --no-print-directory $@=

clean:
	@make clean -j$(CORES) -f makefile_real --no-print-directory $@=

genmake:
	@make genmake -j$(CORES) -f makefile_real --no-print-directory $@=

endif

# mac
ifeq ($(UNAME_S),Darwin)
	@echo "Unsupported operating system due to my lack of mac devices, feel free to make a pr with a working build system"
endif

# windows
ifeq ($(OS),Windows_NT)
	@echo "Unsupported operating system due to:
"fuck that dll hell" - Winter 2021"
endif
