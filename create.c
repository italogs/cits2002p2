#include "uniqfs.h"
#include <getopt.h>

//  create.c, written by Chris.McDonald@uwa.edu.au, October 2014

#define CREATE_OPTLIST          "b:n:v:"

int create(int argc, char *argv[])
{
    UNIQFS_VOLUME_HEADER        header = {
        .nblocks        = 0,
        .blocksize      = 0
    };

//  CHECK THE COMMAND-LINE ARGUMENTS PROVIDED FOR 'create'
    int  opt;
    opterr      = 0;
    while((opt = getopt(argc, argv, CREATE_OPTLIST)) != -1)   
    {  
        switch (opt) {
//  BLOCKSIZE - AN INTEGER ARGUMENT
        case 'b' :
            header.blocksize    =  atoi(optarg);
            break;

//  NBLOCKS - AN INTEGER ARGUMENT
        case 'n' :
            header.nblocks      =  atoi(optarg);
            break;

//  NAME OF REQUIRED VOLUME
        case 'v' :
            volumename          =  strdup(optarg);
            break;

//  OOPS - AN UNKNOWN ARGUMENT
        default :
            header.blocksize    = 0;
            header.nblocks      = 0;
            break;
        }
    }

//  ENSURE THAT THE CORRECT COMBINATION/VALUES OF OPTIONS WERE PROVIDED
    if(header.nblocks   <= 0 ||
       header.blocksize <  (sizeof(UNIQFS_FILE_INFO)+sizeof(UNIQFS_FILE_ENTRY)))
    {
        fprintf(stderr,
                "Usage: %s create [-v volumename] -n nblocks -b blocksize\n",
                progname);
        exit(EXIT_FAILURE);
    }

//  INITIALISE THE 'bitmap' BLOCK FOR THE VOLUME
    UNIQFS_BIT  bitmap[header.nblocks];
    for(int b=0 ; b<header.nblocks ; ++b)
    {
        bitmap[b]       = UNIQFS_UNUSED;
    }

//  INITIALISE ONE EMPTY BLOCK (EVENTUALLY TO HOLD INFO OR DATA)
    char        emptyblock[header.blocksize];
    for(int b=0 ; b<header.blocksize ; ++b)
    {
        emptyblock[b]   = '\0';
    }

//  ATTEMPT TO CREATE AND OPEN THE REQUIRED UNIQFS-VOLUME ON DISK
    FILE *fp = open_volumename("w");            // similar to fopen()

//  WRITE ALL OF THE INITIALISED FIELDS TO THE VOLUME
    fwrite(&header, sizeof header, 1, fp);
    fwrite(bitmap, header.nblocks * sizeof(UNIQFS_BIT), 1, fp);
    for(int b=0 ; b<header.nblocks ; ++b)
    {
        fwrite(&emptyblock, header.blocksize, 1, fp);
    }

//  AND FINISH CREATING THE VOLUME
    fclose(fp);

    return 0;
}

//  vim: ts=8 sw=4
