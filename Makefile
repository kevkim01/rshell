COMPILE = g++
FLAGS = -W -Wall -Werror -pedantic -ansi

all: move
	$(COMPILE) $(FLAGS) ./src/main.cpp -o ./bin/rshell

rshell: move
	$(COMPILE) $(FLAGS) ./src/main.cpp -o ./bin/rshell

move:
	@mkdir bin
