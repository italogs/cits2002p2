


uniqfs:        uniqfs.o put.o globals.o create.o md5digest.o get.o list.o
	cc -o uniqfs uniqfs.o globals.o put.o create.o get.o list.o md5digest.o -lssl -lcrypto

uniqfs.o:       uniqfs.h uniqfs.c
	cc -std=c99 -Wall -Werror -pedantic -c uniqfs.c

globals.o:      uniqfs.h globals.c
	cc -std=c99 -Wall -Werror -pedantic -c globals.c

create.o:       uniqfs.h create.c
	cc -std=c99 -Wall -Werror -pedantic -c create.c

put.o:					uniqfs.h put.c
	cc -std=c99 -Wall -Werror -pedantic -c put.c

get.o:					uniqfs.h get.c
	cc -std=c99 -Wall -Werror -pedantic -c get.c

list.o:					uniqfs.h list.c
	cc -std=c99 -Wall -Werror -pedantic -c list.c

md5digest.o:    uniqfs.h md5digest.c
	cc -Wno-deprecated-declarations -std=c99 -Wall -Werror -pedantic -c md5digest.c



clean:
	rm -f uniqfs.o globals.o create.o md5digest.o put.o get.o list.o uniqfs.o 
