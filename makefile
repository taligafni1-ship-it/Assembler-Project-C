main: main.o macro.o check_action.o check_action_second_pass.o first_pass.o second_pass.o
	gcc -ansi -Wall -pedantic main.o macro.o check_action.o check_action_second_pass.o first_pass.o second_pass.o -o main

main.o: main.c types.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o

macro.o: macro.c macro.h types.h
	gcc -c -ansi -Wall -pedantic macro.c -o macro.o

check_action.o: check_action.c check_action.h types.h
	gcc -c -ansi -Wall -pedantic check_action.c -o check_action.o

check_action_second_pass.o: check_action_second_pass.c check_action_second_pass.h types.h
	gcc -c -ansi -Wall -pedantic check_action_second_pass.c -o check_action_second_pass.o

first_pass.o: first_pass.c first_pass.h types.h
	gcc -c -ansi -Wall -pedantic first_pass.c -o first_pass.o

second_pass.o: second_pass.c second_pass.h check_action_second_pass.h types.h
	gcc -c -ansi -Wall -pedantic second_pass.c -o second_pass.o

