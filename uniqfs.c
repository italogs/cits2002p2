#include "uniqfs.h"

/*
   CITS2002 Project 2 2014
   Name(s):             Italo Gomes Santana (, student-name2)
   Student number(s):   21382104 (, student-number-2)
   Date:                date-of-submission
*/

char *MD5_format(void *md5_result)
{
    static char fmt[64];

    unsigned char        *res = (unsigned char *)md5_result;
    char        *s      = fmt;

    for(int n=0 ; n<MD5_DIGEST_LENGTH; ++n, s+=2)
        sprintf(s, "%02x", res[n]);
    *s  = '\0';
    return fmt;
}

int main(int argc, char *argv[])
{
    int result  = 0;

//  INITIALISE IMPORTANT GLOBAL VARIABLES
    progname    = strdup(argv[0]);
    volumename  = getenv("UNIQFS_VOLUME");

//  CHECK THE REMAINING OPTIONS TO DETERMINE THE REQUIRED ACTION
    if(argc > 1 && strcmp(argv[1], "create") == 0)
    {
        result  = create(argc-1, &argv[1]);
    }
    else if(argc > 1 && strcmp(argv[1], "verify") == 0)
    {
         // IN THE SAMPLE SOLUTION, BUT NOT REQUIRED FOR YOUR PROJECT
    }
    else if(argc > 1 && strcmp(argv[1], "view") == 0)
    {
        // IN THE SAMPLE SOLUTION, BUT NOT REQUIRED FOR YOUR PROJECT
    }
    else if(argc > 1 && strcmp(argv[1],"put") == 0)
    {
        result = put(argc, &argv[2]);
    }
    else if(argc > 1 && strcmp(argv[1],"get") == 0)
    {
        //result = get(argc, &argv[2]);
    }
    else if(argc > 1 && strcmp(argv[1],"list") == 0)
    {
      dump_volume();
        // result = list();
    }
    else if(argc > 1 && strcmp(argv[1],"delete") == 0)
    {
      // result = delete(argc-1, &argv[1]);
    }
    else if(argc > 1 && strcmp(argv[1],"stats") == 0)
    {
      // result = stats();
    }
    else
    {
        fprintf(stderr, "Usage: %s action [options]\n", progname);
        exit(EXIT_FAILURE);
    }

    return result;
}

//  vim: ts=8 sw=4
