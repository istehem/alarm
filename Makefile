all:
	gcc -o alarm-bin -lncurses *.c

clean:
	rm *.o

debug:
	gcc -D DEBUG -o alarm-bin -lncurses *.c
