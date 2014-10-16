#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>

#include <openssl/md5.h>

//  uniqfs.h, written by Chris.McDonald@uwa.edu.au, October 2014

#if     defined(__linux__)
#include <getopt.h>

extern  char                    *strdup(const char *s);
#endif

//  ----------------------------------------------------------------------

//  STRUCTURES USED THROUGHOUT THE uniqfs PROGRAM
typedef struct {
     uint32_t                   nblocks;
     uint32_t                   blocksize;
} UNIQFS_VOLUME_HEADER;

#define UNIQFS_UNUSED           0
#define UNIQFS_INFO             1
#define UNIQFS_DATA             2

typedef char                    UNIQFS_BIT;     // UNIQFS_UNUSED, ....

typedef struct {
     uint32_t                   nfiles;
     uint32_t                   length;
     uint32_t                   firstblock;
     unsigned char              md5[MD5_DIGEST_LENGTH];
} UNIQFS_FILE_INFO;

#define UNIQFS_MAXNAME_LENGTH   32      // including the NULL byte

typedef struct {
     char                       filename[UNIQFS_MAXNAME_LENGTH];
     int32_t                    creation_time;
} UNIQFS_FILE_ENTRY;


//  DEFINED IN create.c
extern  int                     create(int argc, char *argv[]);

//  DEFINED IN globals.c
extern  char                    *progname;
extern  char                    *volumename;
extern  FILE                    *open_volumename(char *mode);

//  DEFINED IN md5digest.c
extern	unsigned char	*MD5digest(char *filename);
extern	char		*sprintMD5(char output[], unsigned char MD5digest[]);


//  vim: ts=8 sw=4