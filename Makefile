ldshim: ldshim.c
	gcc -o ldshim -static ldshim.c

clean:
	rm ldshim