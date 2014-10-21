#include "uniqfs.h"

/*
   CITS2002 Project 2 2014
   Name(s):             student-name1 (, student-name2)
   Student number(s):   student-number-1 (, student-number-2)
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




//  vim: ts=8 sw=4
