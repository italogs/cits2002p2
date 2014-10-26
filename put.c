#include "uniqfs.h"

//  put.c, written by Italo Gomes Santana, October 2014


int put(int argc, char *argv[])
{

  printf("put();\n");

  if(strlen(*argv) >= UNIQFS_MAXNAME_LENGTH ) {
    fprintf(stderr,"%s: File name too long\n",*argv);exit(EXIT_FAILURE);
  }

  FILE *volume = open_volumename("r+");
  if(volume == NULL) {fprintf(stderr,"Volume not defined. You should define one to use this function.\n");exit(EXIT_FAILURE);}

  UNIQFS_VOLUME_HEADER header = getHeader();
  UNIQFS_BIT *bitmap = getBitmap();

  // iterate among the files
  while(*argv != NULL) {
    printf("-> filename: %s\n",*argv);

    unsigned long fileSize = getFileSize(*argv);
    printf("-> filesize %lu\n",fileSize);
    FILE *inputFile = fopen(*argv,"r");

    // //couldn't open the file
    if(inputFile == NULL) {fprintf(stderr,"Usage: %s put filename1 [filename2 ....] \n",progname);exit(EXIT_FAILURE);}

    int filenBlocks = fileSize / header.blocksize;

    //verify if there is enough space including for the info block
    if (header.currentblock + filenBlocks + 1 >= header.nblocks) {fprintf(stderr,"%s: No space left on device\n",*argv);exit(EXIT_FAILURE);}


    UNIQFS_FILE_ENTRY fileEntryInput;
    time_t curtime;time(&curtime);
    fileEntryInput.creation_time = curtime;
    time_t a = fileEntryInput.creation_time;
    printf("-> creation time: %s\n",ctime(&a));
    strcpy(fileEntryInput.filename,"abc123");

    UNIQFS_FILE_INFO fileInfoInput;
    fileInfoInput.nfiles = 1;
    fileInfoInput.length = fileSize;
    fileInfoInput.firstblock = header.currentblock;
    // char md5string[MD5_DIGEST_LENGTH] = "";
    unsigned char *md5 = MD5digest(*argv);
    // sprintMD5(md5string, md5);
    // int len = strlen(md5string);
    int i=0;
    for(i=0;i<MD5_DIGEST_LENGTH;i++) {
      fileInfoInput.md5[i] = md5[i];
    }
    fileInfoInput.md5[i] = '\0';
    // printf("-> generated md5: %02x\n",fileInfoInput.md5);


    bitmap[header.currentblock] = UNIQFS_INFO;
    header.currentblock++;
    for(int i = 0 ; i < filenBlocks;i++) {
        bitmap[header.currentblock + i] = UNIQFS_DATA;
    }
    header.currentblock += filenBlocks;
    fwrite(&header, sizeof(header), 1, volume);
    fwrite(bitmap, header.nblocks * sizeof(UNIQFS_BIT), 1, volume);



    char emptyblock[header.blocksize];
    UNIQFS_FILE_INFO fileInfo;
    UNIQFS_FILE_ENTRY file;
    for(unsigned long i = 0 ; i < header.currentblock; i++) {
        //data block
        if ( bitmap[i] == UNIQFS_UNUSED || bitmap[i] == UNIQFS_DATA) {
          fread(&emptyblock, header.blocksize,1,volume);

        } else if (bitmap[i] == UNIQFS_INFO) {

          fread(&fileInfo, sizeof(UNIQFS_FILE_INFO),1,volume);
          if(isMD5equal(fileInfoInput.md5,fileInfo.md5)) {
            printf("Files already exists #TODO\n\n");
            exit(EXIT_FAILURE);
          }

          for ( int j = 0 ; j < fileInfo.nfiles ; j++ ) {
            fread(&file, sizeof(UNIQFS_FILE_ENTRY),1,volume);
          }
        }
    }

    fwrite(&fileInfoInput, sizeof(UNIQFS_FILE_INFO), 1, volume);

    printf("(%d files) md5: ",fileInfoInput.nfiles);
    for(int i = 0 ; i < MD5_DIGEST_LENGTH ; i ++ ) {
        printf("%02x",fileInfoInput.md5[i]);
    }
    printf("\nfilename: %s\n",fileEntryInput.filename);
    fwrite(&fileEntryInput, sizeof(UNIQFS_FILE_ENTRY), 1, volume);
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
    printf("aqui %d %c\n",header.blocksize,emptyblock[0]);
    while (b < header.blocksize) {
      emptyblock[b] = '\0';
      b++;
    }

    fwrite(&emptyblock, header.blocksize, 1, volume);
    fclose(inputFile);
    argv++;

  }
  fclose(volume);
  return 0;
}
