make
rm -f uniqfs.o globals.o create.o md5digest.o put.o get.o list.o uniqfs.o /Volumes/MYUSB/uniqfs-one
./uniqfs create -n 500 -b 1024
./uniqfs put md5digest.c get.c
./uniqfs list
