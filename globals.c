#include "uniqfs.h"

/*
   CITS2002 Project 2 2014
   Name(s):             Italo Gomes Santana (, student-name2)
   Student number(s):   21382104 (, student-number-2)
   Date:                date-of-submission
*/


//  GLOBAL VARIABLES AND FUNCTIONS USED THROUGHOUT THE PROGRAM

char    *progname       = NULL;
char    *volumename     = NULL;

FILE *open_volumename(char *mode)
{
    if(volumename == NULL)
    {
        fprintf(stderr, "%s: required volume name is not defined\n", progname);
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(volumename, mode);
    if(fp == NULL)
    {
        perror(progname);
        exit(EXIT_FAILURE);
    }
    return fp;
}

bool isMD5equal(unsigned char *md51, unsigned char *md52) {
    for ( int i = 0 ; i < MD5_DIGEST_LENGTH; i ++ ) {
      if(md51[i] != md52[i]){
        return false;
      }
    }
    return true;
}

void printHeaderBitmap() {
    UNIQFS_VOLUME_HEADER header = getHeader();

    printf("\n\n----> PRINTING HEADER\n");


    printf("header\nnblocks:%d blocksize:%d currentblock:%d\n",header.nblocks,header.blocksize,header.currentblock);
    printf("\n----> PRINTING BITMAP\n");
    UNIQFS_BIT *temp = getBitmap();
    for(int i = 0 ; i < 10;i++) {
      printf("bitmap[%d]: %d\n",i,temp[i]);
    }
    printf("\n\n");


}



UNIQFS_VOLUME_HEADER getHeader() {
    UNIQFS_VOLUME_HEADER header;
    FILE *fp = open_volumename("r");
    fread(&header, sizeof header,1,fp);
    fclose(fp);
    return header;
}

UNIQFS_BIT *getBitmap() {

    FILE *fp = open_volumename("r");

    UNIQFS_VOLUME_HEADER header;
    fread(&header, sizeof header,1,fp);

    UNIQFS_BIT *bitmap = (UNIQFS_BIT *) malloc(header.nblocks * sizeof(UNIQFS_BIT));

    fread(bitmap, header.nblocks * sizeof(UNIQFS_BIT), 1, fp);

    fclose(fp);
    return bitmap;
}

unsigned long getFileSize(char *filename) {
    printf("getFileSize: %s\n",filename);
    FILE *fp = fopen(filename,"r");
    if(fp == NULL)
    {
        perror(progname);
        exit(EXIT_FAILURE);
    }
    fseek(fp,0,2);
    unsigned long fileSize = ftell(fp);
    fclose(fp);
    return fileSize;
}






//  vim: ts=8 sw=4
