import re

inFile = open("makefile_real")
out = open("Makefile", "w")

commands = [i.split(":")[0][1:] for i in re.findall(r"\n[^#^\n].+:[^=^\n.]*\n", inFile.read())]
lin_commands = list(filter(lambda x:not x.startswith("win_") and not x.startswith("mac_"), commands))
win_commands = list(filter(lambda x:x.startswith("win_"), commands))
mac_commands = list(filter(lambda x:x.startswith("mac_"), commands))

out.write(f'''#
# Edit makefile_real to change rules and config.
# Edit the below var to change the multiplier for the number of threads (by default it is 1, meaning it will be the number of cores you have).
#
coreMult = 2

CORES := $$(( $(shell nproc) * $(coreMult) ))
UNAME_S := $(shell uname -s)

# Linuhh how install <X>
ifeq ($(UNAME_S),Linux)

KNOWN = {" ".join(lin_commands)}

''')

for i in lin_commands:
    out.write(f'''{i}:
	@make {i} -j$(CORES) -f makefile_real --no-print-directory $(filter-out $(KNOWN), $@)

''')

out.write(f'''endif

# MacOShit
ifeq ($(UNAME_S),Darwin)

KNOWN = {" ".join(i[4:] for i in mac_commands)}

''')

for i in mac_commands:
    out.write(f'''{i[4:]}:
	@make {i} -j$(CORES) -f makefile_real --no-print-directory $(filter-out $(KNOWN), $@)

''')

out.write(f'''endif

# Windex
ifeq ($(OS),Windows_NT)

KNOWN = {" ".join(i[4:] for i in win_commands)}

''')

for i in win_commands:
    out.write(f'''{i[4:]}:
	@make {i} -j$(CORES) -f makefile_real --no-print-directory $(filter-out $(KNOWN), $@)

''')

out.write('''endif
''')

print("\nLinux commands added:")
for i in lin_commands:
    print(f" - {i}")

print("\nMac commands added:")
for i in mac_commands:
    print(f" - {i}")
    
print("\nWindows commands added:")
for i in win_commands:
    print(f" - {i}")

inFile.close()
out.close()
