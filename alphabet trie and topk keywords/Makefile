build:
	gcc -o search_index tema2.c
run:
	./search_index
clean:
	rm -f search_index search_index.o
valgrind:
	valgrind --leak-check=full  --show-leak-kinds=all  --track-origins=yes ./search_index
pack:
	zip -FSr Savlovschi_Andrei-Bogdan_311CB_Tema2.zip README Makefile *.c *.h