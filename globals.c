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

    UNIQFS_BIT *bitmap = (UNIQFS_BIT *) malloc(sizeof(UNIQFS_BIT) * header.nblocks);

    fread(bitmap, header.nblocks * sizeof(UNIQFS_BIT), 1, fp);

    fclose(fp);
    return bitmap;
}






//  vim: ts=8 sw=4
