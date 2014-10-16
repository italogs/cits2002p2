
uniqfs:        uniqfs.o globals.o create.o md5digest.o
	cc -o uniqfs uniqfs.o globals.o create.o md5digest.o -lssl -lcrypto

uniqfs.o:       uniqfs.h uniqfs.c
	cc -std=c99 -Wall -Werror -pedantic -c uniqfs.c

globals.o:      uniqfs.h globals.c
	cc -std=c99 -Wall -Werror -pedantic -c globals.c

create.o:       uniqfs.h create.c
	cc -std=c99 -Wall -Werror -pedantic -c create.c

md5digest.o:    uniqfs.h md5digest.c
	cc -Wno-deprecated-declarations -std=c99 -Wall -Werror -pedantic -c md5digest.c



clean:
	rm -f uniqfs uniqfs.o globals.o create.o md5digest.o
