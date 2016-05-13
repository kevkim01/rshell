COMPILE = g++
FLAGS = -W -Wall -Werror -pedantic -ansi

main: main.cpp command.h command_line.h
	$(COMPILE) $(FLAGS) main.cpp 