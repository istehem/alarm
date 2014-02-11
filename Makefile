all:
	gcc -o ararm-bin -lncurses *.c

clean:
	rm *.o
