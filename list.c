#include "uniqfs.h"

//  list.c, written by Italo Gomes Santana, October 2014


int list()
{

  printf("list();\n");

  FILE *volume = open_volumename("r");

  UNIQFS_VOLUME_HEADER header;
  fseek(volume, sizeof(header) + (header.nblocks * sizeof(UNIQFS_BIT)),SEEK_SET);
  
  fclose(volume);
  return 0;
}
