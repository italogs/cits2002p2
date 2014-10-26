#include "uniqfs.h"

//  list.c, written by Italo Gomes Santana, October 2014


int list()
{

  printf("list();\n");

  FILE *volume = open_volumename("r");

  UNIQFS_VOLUME_HEADER header;
  fread(&header, sizeof header,1,volume);
  UNIQFS_BIT *bitmap = (UNIQFS_BIT *) malloc(header.nblocks * sizeof(UNIQFS_BIT));
  fread(bitmap, header.nblocks * sizeof(UNIQFS_BIT), 1, volume);

  char emptyblock[header.blocksize];
  printf("%d\n",header.currentblock);
  for(unsigned long i = 0 ; i < header.currentblock; i++) {
      if ( bitmap[i] == UNIQFS_UNUSED || bitmap[i] == UNIQFS_DATA) {
        fread(&emptyblock, header.blocksize,1,volume);
      } else if (bitmap[i] == UNIQFS_INFO) {
        UNIQFS_FILE_INFO fileInfo;
        fread(&fileInfo, sizeof(UNIQFS_FILE_INFO),1,volume);

        printf("(%d files) md5: ",fileInfo.nfiles);
        for(int i = 0 ; i < MD5_DIGEST_LENGTH ; i ++ ) {
            printf("%02x",fileInfo.md5[i]);
        }
        printf("\n");
        UNIQFS_FILE_ENTRY file;
        for ( int j = 0 ; j < fileInfo.nfiles ; j++ ) {
          fread(&file, sizeof(UNIQFS_FILE_ENTRY),1,volume);
          time_t a = file.creation_time;
          printf("%s %s\n",file.filename,ctime(&a));

        }
      }
  }

  fclose(volume);
  return 0;
}
