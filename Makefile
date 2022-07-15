ldshim: ldshim.c
	gcc -s -o ldshim -static ldshim.c

clean:
	rm ldshim
