#include "uniqfs.h"

//  md5digest.c, written by Chris.McDonald@uwa.edu.au, October 2014

//  CALCULATE THE MD5 DIGEST OF A NAMED FILE - NO NEED TO UNDERSTAND THIS
//  see:        http://en.wikipedia.org/wiki/MD5
//  and         https://www.openssl.org/docs/crypto/md5.html

unsigned char *MD5digest(char *filename)
{
    static unsigned char        digest[MD5_DIGEST_LENGTH];

    memset(digest, 0, sizeof(digest));

    FILE *fp = fopen(filename, "r");

    if(fp != NULL)
    {
        MD5_CTX         mdContext;
        unsigned char   data[1024];
        size_t          nbytes;

        MD5_Init(&mdContext);
        while((nbytes = fread(data, 1, sizeof(data), fp)) != 0)
            MD5_Update(&mdContext, data, nbytes);
        MD5_Final(digest, &mdContext);
        fclose(fp);
    }
    return digest;
}

//  PRINT AN MD5 DIGEST INTO A PROVIDED STRING (which must be >= 33 bytes long)
char *sprintMD5(char output[], unsigned char MD5digest[MD5_DIGEST_LENGTH])
{
    unsigned char       *d = MD5digest;
    char                *s = output;

    for(int n=0 ; n<MD5_DIGEST_LENGTH ; ++n, s+=2)
        sprintf(s, "%02x", d[n]);
    *s  = '\0';
    return output;
}


//  THE FOLLOWING CODE IS REQUIRED IFF USING SeeC - NO NEED TO UNDERSTAND IT
#ifdef __SEEC__

void __SeeC_MD5_Init__() {}
void __SeeC_MD5_Update__() {}
void __SeeC_MD5_Final__() {}

unsigned char *__SeeC_MD5digest__(char *filename)
{
    extern void SeeCRecordSetCurrentPointer(void const *);
    extern void __SeeC_LockMemoryForWriting__();
    extern void __SeeC_CheckCStringRead__(size_t, char const *);
    extern void __SeeC_SetPointerTargetNewValid__(void const *);
    extern void __SeeC_RecordUntypedState__(char const *, size_t);
    extern void __SeeC_ReleaseLocks__();

    __SeeC_LockMemoryForWriting__();
    __SeeC_CheckCStringRead__(0, filename);

    unsigned char * const Result = MD5digest(filename);

    SeeCRecordSetCurrentPointer(Result);
    __SeeC_SetPointerTargetNewValid__(Result);
    __SeeC_RecordUntypedState__((char const *)Result, MD5_DIGEST_LENGTH);
    __SeeC_ReleaseLocks__();

    return Result;
}

#endif

//  vim: ts=8 sw=4
