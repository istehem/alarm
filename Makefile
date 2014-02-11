all:
	gcc -o alarm-bin -lncurses *.c

clean:
	rm *.o
