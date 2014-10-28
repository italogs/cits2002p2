#include "uniqfs.h"

//  put.c, written by Italo Gomes Santana, October 2014


int put(int argc, char *argv[])
{

  printf("put();\n");

  if(strlen(*argv) >= UNIQFS_MAXNAME_LENGTH ) {fprintf(stderr,"%s: File name too long\n",*argv);exit(EXIT_FAILURE);}


  // iterate among the files
  while(*argv != NULL) {


    setHeaderBitmap();
    printf("-> filename: %s\n",*argv);

    unsigned long fileSize = getFileSize(*argv);
    printf("-> filesize %lu\n",fileSize);
    FILE *inputFile = fopen(*argv,"r");

    //couldn't open the file
    if(inputFile == NULL) {fprintf(stderr,"Usage: %s put filename1 [filename2 ....] \n",progname);exit(EXIT_FAILURE);}

    int filenBlocks = fileSize / header.blocksize + 1;

    printf("filenBlocks: %d\ncurrentblock: %d\n",filenBlocks,header.currentblock);

    //verify if there is enough space including for the info block
    if (header.currentblock + filenBlocks > header.nblocks) {fprintf(stderr,"%s: No space left on device\n",*argv);exit(EXIT_FAILURE);}

    unsigned char *md5FromARGV = MD5digest(*argv);






    FILE *navigator = open_volumename("r+");
    //go to block section
    fseek(navigator,sizeof header + (header.nblocks * sizeof(UNIQFS_BIT)) ,SEEK_SET);
    printf("before FOR %lu\n",ftell(navigator));
    bool fileAlreadyExists = false;
    for(unsigned long i = 0 ; i < header.currentblock; i++) {
        //data block
        printf(" bitmap[%lu]: ",i);
        if ( bitmap[i] == UNIQFS_UNUSED ) {
          printf("UNIQFS_UNUSED\n");
        } else if (bitmap[i] == UNIQFS_DATA) {
          printf("UNIQFS_DATA\n");
        } else if (bitmap[i] == UNIQFS_INFO) {
          printf("UNIQFS_INFO\n");
          UNIQFS_FILE_INFO fileInfo;
          fread(&fileInfo, sizeof(UNIQFS_FILE_INFO),1,navigator);

          // printf("entrada md5: ");
          // for(int i = 0 ; i < MD5_DIGEST_LENGTH ; i ++ ) {
          //     printf("%02x",md5FromARGV[i]);
          // }
          // printf("\n");
          // printf("existente md5: ");
          // for(int i = 0 ; i < MD5_DIGEST_LENGTH ; i ++ ) {
          //     printf("%02x",fileInfo.md5[i]);
          // }
          // printf("\n");

          if(isMD5equal(md5FromARGV,fileInfo.md5)) {

            printf("MD5 equal #TO DO\n");
            exit(EXIT_FAILURE);

          //   fileInfo.nfiles = 2;
          //   printf("ftell(navigator) %lu\n",ftell(navigator));
          //   fseek(navigator,-sizeof(UNIQFS_FILE_INFO),SEEK_CUR);
          //   fwrite(&fileInfo, sizeof(UNIQFS_FILE_INFO),1,navigator);
          //
          //   UNIQFS_FILE_INFO fileInfo2;
          // //  fread(&fileInfo2, sizeof(UNIQFS_FILE_INFO),1,navigator);
          //
          //
          //   printf("fileInfo.nfiles %d\n",fileInfo2.nfiles);
          //   printf("ftell(navigator) %lu\n",ftell(navigator));
          //   printf("Files already exists\n");
          //
          //   fileAlreadyExists = true;
          //
          //   UNIQFS_FILE_ENTRY fileEntryInput;
          //   time_t curtime;time(&curtime);
          //   fileEntryInput.creation_time = curtime;
          //   strcpy(fileEntryInput.filename,"vai se fuder");
          //   printf("%s\n",fileEntryInput.filename);
            // for ( int j = 0 ; j < fileInfo.nfiles - 1 ; j++ ) {
            //   fread(&file, sizeof(file),1,navigator);
            //   // fseek(navigator, sizeof(file),SEEK_CUR);
            // }
            // fwrite(&fileEntryInput, sizeof(UNIQFS_FILE_ENTRY), 1, navigator);

          } else {

            for ( int j = 0 ; j < fileInfo.nfiles ; j++ ) {
              fseek(navigator,sizeof(UNIQFS_FILE_ENTRY),SEEK_CUR);
            }
          }
        }
    }
    printf("after FOR %lu\n",ftell(navigator));
    printf("fim for\n");
    if(fileAlreadyExists) {
      printf("IF fileAlreadyExists \n");
      fclose(inputFile);
      argv++;
      continue;
    }




    UNIQFS_FILE_INFO fileInfoInput;
    fileInfoInput.nfiles = 1;
    fileInfoInput.length = fileSize;
    fileInfoInput.firstblock = header.currentblock;

    int i=0;
    for(i=0;i<MD5_DIGEST_LENGTH;i++) {
      fileInfoInput.md5[i] = md5FromARGV[i];
    }
    fileInfoInput.md5[i] = '\0';
    printf("position to write:%ld\n",ftell(navigator));
    fwrite(&fileInfoInput, sizeof(fileInfoInput), 1, navigator);


    UNIQFS_FILE_ENTRY fileEntryInput;
    time_t curtime;time(&curtime);
    fileEntryInput.creation_time = curtime;
    time_t a = fileEntryInput.creation_time;
    printf("-> creation time: %s\n",ctime(&a));
    strcpy(fileEntryInput.filename,*argv);

    fwrite(&fileEntryInput, sizeof(UNIQFS_FILE_ENTRY), 1, navigator);


    fclose(navigator);




    printf("bitmap[%d] = UNIQFS_INFO\n",header.currentblock);
    bitmap[header.currentblock++] = UNIQFS_INFO;

    for(int i = header.currentblock;i < header.currentblock + filenBlocks ; i++ ){
      bitmap[i] = UNIQFS_DATA;
      printf("bitmap[%d] = UNIQFS_DATA\n",i);
    }

    FILE *headerBitmap = open_volumename("r+");
    //int priorBlock = header.currentblock;
    header.currentblock += filenBlocks;
    fwrite(&header, sizeof(UNIQFS_VOLUME_HEADER), 1, headerBitmap);
    fwrite(bitmap, header.nblocks * sizeof(UNIQFS_BIT), 1, headerBitmap);
    fclose(headerBitmap);

    // rewind(volume);
    fclose(inputFile);

    argv++;

    // int b = 0;
    // char c = 0;
    // char emptyblock[header.blocksize];
    // while( (c = fgetc(inputFile)) != EOF ){
    //   emptyblock[b++] = c;
    //   if(b == header.blocksize) {
    //     fwrite(&emptyblock, header.blocksize, 1, volume);
    //     b = 0;
    //   }
    // }
    // fwrite(&emptyblock, header.blocksize, 1, volume);



    // for(int b=0 ; b<header.blocksize ; ++b)
    // {
    //     emptyblock[b]   = '\0';
    // }
    //
    // for(int b=0 ; b<header.nblocks ; ++b)
    // {
    //     fwrite(&emptyblock, header.blocksize, 1, volume);
    // }
    //
    //






  }

  return 0;
}
