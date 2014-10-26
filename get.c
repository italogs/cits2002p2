#include "uniqfs.h"

//  put.c, written by Italo Gomes Santana, October 2014


int get(int argc, char *argv[])
{

  printf("get();\n");

  if(strlen(*argv) >= UNIQFS_MAXNAME_LENGTH ) {
    fprintf(stderr,"%s: File name too long\n",*argv);exit(EXIT_FAILURE);
  }

  
  printHeaderBitmap();
  return 0;
}
