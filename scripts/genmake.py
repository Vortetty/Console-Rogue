import re

inFile = open("makefile_real")
out = open("Makefile", "w")

commands = [i.split(":")[0][1:] for i in re.findall(r"\n.+:[^=.]*\n\t", inFile.read())]

out.write('''#
# Edit makefile_real to change rules and config.
# Edit the below var to change the multiplier for the number of threads (by default it is 1, meaning it will be the number of cores you have).
#
coreMult = 2

CORES := $$(( $(shell nproc) * $(coreMult) ))
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)

''')

for i in commands:
    out.write(f'''{i}:
	@make {i} -j$(CORES) -f makefile_real --no-print-directory $@=

''')

out.write('''endif

# mac
ifeq ($(UNAME_S),Darwin)
	@echo "Unsupported operating system due to my lack of mac devices, feel free to make a pr with a working build system"
endif

# windows
ifeq ($(OS),Windows_NT)
	@echo "Unsupported operating system due to:\n\"fuck that dll hell\" - Winter 2021"
endif
''')
