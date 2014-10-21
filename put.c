#include "uniqfs.h"

//  put.c, written by Italo Gomes Santana, October 2014


int put(int argc, char *argv[])
{

  printf("put();\n");

  if(strlen(*argv) >= UNIQFS_MAXNAME_LENGTH ) {
    fprintf(stderr,"%s: File name too long\n",*argv);exit(EXIT_FAILURE);
  }

  FILE *volume = open_volumename("a");

  UNIQFS_VOLUME_HEADER header = getHeader();

  UNIQFS_BIT *bitmap = getBitmap();


  if(volume == NULL) {fprintf(stderr,"Volume not defined. You should define one to use this function.\n");exit(EXIT_FAILURE);}

  while(*argv != NULL) {
    printf("filename: %s\n",*argv);

    FILE *fp = fopen(*argv,"r");
    //couldn't open the file
    if(fp == NULL) {fprintf(stderr,"Usage: %s put filename1 [filename2 ....] \n",progname);exit(EXIT_FAILURE);}

    fseek(fp,0,2);
    unsigned long fileSize = ftell(fp);

    int fileBlocks = fileSize / header.blocksize;

    //verify if there is enough space including for the info block
    if (header.currentblock + fileBlocks + 1 >= header.nblocks) {fprintf(stderr,"%s: No space left on device\n",*argv);exit(EXIT_FAILURE);}

    UNIQFS_FILE_ENTRY file;
    time_t curtime;time(&curtime);
    file.creation_time = curtime;
    time_t a = file.creation_time;
    printf("aqui: %s\n",ctime(&a));

    strcpy(file.filename,*argv);

    bitmap[header.currentblock] = UNIQFS_INFO;

    UNIQFS_FILE_INFO fileInfo;
    fileInfo.nfiles = 1;
    fileInfo.length = fileSize;
    fileInfo.firstblock = header.currentblock;
    char md5string[100];
    unsigned char *md5 = MD5digest(*argv);
    sprintMD5(md5string, md5);

    int i=0;int len = strlen(md5string);
    for(i=0;i<len;i++) {
      fileInfo.md5[i] = md5string[i];
    }
    fileInfo.md5[i] = '\0';
    printf("md5: %s\n",fileInfo.md5);
    header.currentblock++;
    printf("header.currentblock: %d",header.currentblock);

    for(int i = 0 ; i < fileBlocks;i++) {
        bitmap[header.currentblock + i] = UNIQFS_DATA;
    }




    int c = 0;

    for(int b=0 ; b<header.blocksize ; ++b)
    {
        emptyblock[b]   = '\0';
    }

    char emptyblock[header.blocksize];
    while( (c = fgetc(fp)) != EOF ){


    }


    for(int b=0 ; b<header.nblocks ; ++b)
    {
        fwrite(&emptyblock, header.blocksize, 1, fp);
    }




    fclose(fp);
    argv++;
  }

  return 0;
}
