#include "uniqfs.h"

//  put.c, written by Italo Gomes Santana, October 2014


int put(int argc, char *argv[])
{

  printf("put();\n");

  if(strlen(*argv) >= UNIQFS_MAXNAME_LENGTH ) {fprintf(stderr,"%s: File name too long\n",*argv);exit(EXIT_FAILURE);}


  // iterate among the files
  while(*argv != NULL) {
    FILE *volume = open_volumename("r+");
    FILE *upperPart = volume;
    setHeaderBitmap();
    printf("-> filename: %s\n",*argv);

    unsigned long fileSize = getFileSize(*argv);
    printf("-> filesize %lu\n",fileSize);
    FILE *inputFile = fopen(*argv,"r");

    //couldn't open the file
    if(inputFile == NULL) {fprintf(stderr,"Usage: %s put filename1 [filename2 ....] \n",progname);exit(EXIT_FAILURE);}

    int filenBlocks = fileSize / header.blocksize + 1;

    printf("filenblocks: %d\n",filenBlocks);

    //verify if there is enough space including for the info block
    if (header.currentblock + filenBlocks > header.nblocks) {fprintf(stderr,"%s: No space left on device\n",*argv);exit(EXIT_FAILURE);}


    UNIQFS_FILE_ENTRY fileEntryInput;
    time_t curtime;time(&curtime);
    fileEntryInput.creation_time = curtime;
    time_t a = fileEntryInput.creation_time;
    printf("-> creation time: %s\n",ctime(&a));
    strcpy(fileEntryInput.filename,*argv);


    UNIQFS_FILE_INFO fileInfoInput;
    fileInfoInput.nfiles = 1;
    fileInfoInput.length = fileSize;
    fileInfoInput.firstblock = header.currentblock;
    unsigned char *md5 = MD5digest(*argv);
    int i=0;
    for(i=0;i<MD5_DIGEST_LENGTH;i++) {
      fileInfoInput.md5[i] = md5[i];
    }
    fileInfoInput.md5[i] = EOF;






    char emptyblock[header.blocksize];
    UNIQFS_FILE_INFO fileInfo;
    UNIQFS_FILE_ENTRY file;
    // FILE *ref = NULL;
    for(unsigned long i = 0 ; i < header.currentblock; i++) {

        //data block
        if ( bitmap[i] == UNIQFS_UNUSED || bitmap[i] == UNIQFS_DATA) {
          //fread(&emptyblock, header.blocksize,1,volume);
          printf("if\n");
        } else if (bitmap[i] == UNIQFS_INFO) {
          printf("else\n");
          fread(&fileInfo, sizeof(fileInfo),1,volume);

          printf("entrada md5: ");
          for(int i = 0 ; i < MD5_DIGEST_LENGTH ; i ++ ) {
              printf("%02x",fileInfoInput.md5[i]);
          }
          printf("\n");
          printf("existente md5: ");
          for(int i = 0 ; i < MD5_DIGEST_LENGTH ; i ++ ) {
              printf("%02x",fileInfo.md5[i]);
          }
          printf("\n");

          if(isMD5equal(fileInfoInput.md5,fileInfo.md5)) {
            printf("Files already exists #TODO\n\n");
            exit(EXIT_FAILURE);
          }

          for ( int j = 0 ; j < fileInfo.nfiles ; j++ ) {
            fread(&file, sizeof(file),1,volume);
          }
        }
    }

    bitmap[header.currentblock++] = UNIQFS_INFO;
    for(int i = header.currentblock;i<header.currentblock + filenBlocks ; i++ ){
      bitmap[i] = UNIQFS_DATA;
    }
    header.currentblock += filenBlocks;
    fwrite(&header, sizeof(header), 1, upperPart);
    fwrite(bitmap, header.nblocks * sizeof(UNIQFS_BIT), 1, upperPart);

    

    fwrite(&fileInfoInput, sizeof(fileInfoInput), 1, volume);
    fwrite(&fileEntryInput, sizeof(fileEntryInput), 1, volume);
    int b = 0;
    char c = 0;
    while( (c = fgetc(inputFile)) != EOF ){
      emptyblock[b] = c;
      b++;
      if(b == header.blocksize) {
        fwrite(&emptyblock, header.blocksize, 1, volume);
        b = 0;
      }
    }

    while (b < header.blocksize) {
      emptyblock[b] = '\0';
      b++;
    }

    fwrite(&emptyblock, header.blocksize, 1, volume);
    fclose(inputFile);
    fclose(volume);
    argv++;


  }

  return 0;
}
