OBJ = main.o

network: $(OBJ)
	gcc -o parser $(OBJ)

main.o: main.c
	gcc -c main.c

clean:
	rm -rf $(OBJ) parser