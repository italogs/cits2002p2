#include "uniqfs.h"
#include <time.h>
/*
   CITS2002 Project 2 2014
   Name(s):             Italo Gomes Santana (, student-name2)
   Student number(s):   21382104 (, student-number-2)
   Date:                date-of-submission
*/


//  GLOBAL VARIABLES AND FUNCTIONS USED THROUGHOUT THE PROGRAM

char    *progname       = NULL;
char    *volumename     = NULL;
UNIQFS_VOLUME_HEADER    header;
UNIQFS_BIT              *bitmap;



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

void setHeaderBitmap() {

    FILE *fp = open_volumename("r");

    fread(&header, sizeof header,1,fp);

    bitmap = (UNIQFS_BIT *) malloc(header.nblocks * sizeof(UNIQFS_BIT));

    fread(bitmap, header.nblocks * sizeof(UNIQFS_BIT), 1, fp);

    fclose(fp);
    return;
}

unsigned long getFileSize(char *filename) {
    printf("\ngetFileSize: %s\n",filename);

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


void SEEK_TO_BLOCK(int bitmapPos) {




}
void dump_volume(void)
{
    FILE *fpv = open_volumename("r");            // open volume for read-access
    // UNIQFS_VOLUME_HEADER header;
    fread(&header, sizeof header, 1, fpv);

    UNIQFS_BIT     thisbitmap[header.nblocks];
    fread(thisbitmap,  header.nblocks * sizeof(UNIQFS_BIT), 1, fpv);
    printf("\n\nDUMP_VOLUME\n");
    printf("header.nblocks=%i\n",       header.nblocks);
    printf("header.blocksize=%i\n",     header.blocksize);
    printf("header.currentblock=%i\n",     header.currentblock);



    // for(int b0=0 ; b0<header.nblocks ; ++b0)
      for(int b0=0 ; b0<15 ; ++b0)
    {
        printf("block-%04i  ", b0);

        switch (thisbitmap[b0]) {
          case UNIQFS_UNUSED : {
              printf("UNIQFS_UNUSED\n");
              break;
          }
          case UNIQFS_INFO : {
              printf("UNIQFS_INFO\n");

              UNIQFS_FILE_INFO    info;
              SEEK_TO_BLOCK(b0);
              fread(&info, sizeof info, 1, fpv);

              printf("\t\tinfo.nfiles=%i\n", info.nfiles);
              printf("\t\tinfo.length=%i\n", info.length);
              printf("\t\tinfo.firstblock=%i\n", info.firstblock);

              char        md5str[2*MD5_DIGEST_LENGTH+1];
              sprintMD5(md5str, info.md5);
              printf("\t\tinfo.md5=%s\n", md5str);

              if(info.nfiles > 1) {
                printf("info.nfiles %d\n",info.nfiles );
                // info.nfiles = 1;
              }
              for(int f=0 ; f<info.nfiles ; ++f)
              {
                  UNIQFS_FILE_ENTRY entry;

                  fread(&entry, sizeof entry, 1, fpv);

                  time_t when = entry.creation_time;
                  printf("\t\t\t%-24s %s", entry.filename, ctime(&when));
              }
              break;
          }
          case UNIQFS_DATA : {
              printf("UNIQFS_DATA\n");
              break;
          }
          default : {
              printf("UNRECOGNIZED!!\n");
              break;
          }
        }                                             // end of switch
    }                                   // end of foreach block
    fclose(fpv);
}




//  vim: ts=8 sw=4
