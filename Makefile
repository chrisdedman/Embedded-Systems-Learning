all: compile object run

help:
	@echo "Usage: make [command]"
	@echo "  init 		 - open gpio"
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
	@gcc -o bin/main src/main.c

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