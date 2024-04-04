# This Makefile provides a set of commands to compile, create object files, 
# and run an embedded systems project. It also includes additional commands 
# for cleaning up object files and executables, as well as initializing GPIO.

# The 'all' target is the default target that will be executed when no specific 
# target is provided. It depends on the 'compile', 'object', and 'run' targets.
all: compile object run

help:
	@echo "Usage: make [command]"
	@echo "  init      - open gpio"
	@echo "  run       - compile and run the project"
	@echo "  compile   - compile the project"
	@echo "  object    - create object files"
	@echo "  clean     - remove object files and executables"
	@echo "  help      - display this help message"
	@echo "  all       - compile, create object files and run the project"

object:
	@echo creating object files...
	@gcc -c src/main.c -o bin/main.o

compile:
	@echo compiling...
	@gcc -o bin/main src/main.cpp

run: compile
	@echo running...
	@./bin/main

clean:
	@echo cleaning...
	@rm -f bin/main bin/main.o

init:
	@echo opening gpio...
	@sudo chmod +x export-gpio.sh
	@./export-gpio.sh