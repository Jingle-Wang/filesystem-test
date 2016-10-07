filesystem-test: filesystem-test.o
		gcc filesystem-test.o -O3 -o filesystem-test -g -lpthread


filesystem-test.o: filesystem-test.c evaluate.h
		gcc -c filesystem-test.c -o filesystem-test.o -g -lpthread

gfapi-filesystem-test: gfapi-filesystem-test.c evaluate.h
		gcc -O2 -o gfapi-filesystem-test gfapi-filesystem-test.c -L /usr/local/lib  -lgfapi -I ~/glusterfs/api/src -g -lpthread

clean:
	rm -rf *.o filesystem-test

api-clean:
	rm -rf gfapi-filesystem-test
