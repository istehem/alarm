all:
	gcc -o alarm-bin *.c -lncurses

clean:
	rm *.o

debug:
	gcc -D DEBUG -o alarm-bin *.c -lncurses 
