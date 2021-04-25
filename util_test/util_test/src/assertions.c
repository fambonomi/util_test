/*
 * assertions.c
 *
 *  Created on: 23 abr. 2021
 *      Author: Fernando
 */
#include <assertions.h>
#include <stdio.h> /*snprintf*/
#include <string.h> /*memcmp*/

#define BUFFER_LEN 512
static char buffer[BUFFER_LEN];

static inline char *assertBooleanMessage(const char* message,const char* reason,const char *func,const char *file,int line)
{
	snprintf(buffer,BUFFER_LEN,"%s\n Reason: %s\n  In function %s\n  In file %s\n  In line %d\n",
			message,reason,func,file,line);
	return buffer;
}

void TG_assertTrue(TestGroup *tg,const char* message,int value,const char *func,const char *file,int line)
{
	static const char failReason[]="Value that should be true is false";
	if(!value)
		TG_fail(tg,assertBooleanMessage(message,failReason,func,file,line));
}

void TG_assertFalse(TestGroup *tg,const char* message,int value,const char *func,const char *file,int line)
{
	static const char failReason[]="Value that should be false is true";
	if(value)
		TG_fail(tg,assertBooleanMessage(message,failReason,func,file,line));
}


static inline const char* assertIntMessage(const char* message,const char* reason,
                                           long long int A,long long int B,
                                           const char *func,const char *file,int line)
{
    snprintf(buffer,BUFFER_LEN,"%s\n Reason: %s\n  With A = %lld\n  With B = %lld\n  In function %s\n  In file %s\n  In line %d\n",
            message,reason,A,B,func,file,line);
    return buffer;
}


void TG_assertIntEqual(TestGroup *tg,const char* message,long long int A,long long int B,const char *func,const char *file,int line)
{
    const char* reason = "Integers A and B should have been equal";

    if (A != B)
        TG_fail(tg,assertIntMessage(message, reason, A, B, func, file, line));
}

void TG_assertIntNotEqual(TestGroup *tg,const char* message,long long int A,long long int B,const char *func,const char *file,int line)
{
    const char* reason = "Integers A and B should have been different";

    if (A == B)
        TG_fail(tg,assertIntMessage(message, reason, A, B, func, file, line));
}

static inline const char* assertMemoryMessage(const char* message,const char* reason,
                                           const void* A, const void *B,size_t length,
                                           const char *func,const char *file,int line)
{
    snprintf(buffer,BUFFER_LEN,"%s\n Reason: %s\n  With A = %p\n  With B = %p\n  Length: %llu\n  In function %s\n  In file %s\n  In line %d\n",
            message,reason,A,B,(unsigned long long int) length, func,file,line);
    return buffer;
}

void TG_assertMemoryEqual(TestGroup *tg,const char* message,
                          const void *A,const void *B, size_t length,
                          const char *func,const char *file, int line)
{
    const char* reason = "Content of memory blocks A and B should be the same but is different";
    const char* reasonError = "Asked to dereference a null pointer, given pointers A and B.";

    const int validPointers = (const void*) 0 != A &&
                              (const void*) 0 != B;

    const int logicallyEqual = A == B ||
                               0 == length;

    if (!validPointers)
        TG_error(tg,assertMemoryMessage(message, reasonError,
                                        A, B, length, func, file, line));

    const int equal = logicallyEqual ||
                      !memcmp(A,B,length);

    if (!equal)
            TG_fail(tg,assertMemoryMessage(message, reason,
                            A, B, length, func, file, line));
}


void TG_assertMemoryNotEqual(TestGroup *tg,const char* message,
                          const void *A,const void *B, size_t length,
                          const char *func,const char *file, int line)
{
    const char* reason = "Content of memory blocks A and B should be different but is the same";
    const char* reasonError = "Asked to dereference a null pointer, given pointers A and B.";

    const int validPointers = (const void*) 0 != A &&
                              (const void*) 0 != B;

    const int logicallyEqual = A == B ||
                               0 == length;

    if (!validPointers)
        TG_error(tg,assertMemoryMessage(message, reasonError,
                                        A, B, length, func, file, line));

    const int equal = logicallyEqual ||
                      !memcmp(A,B,length);

    if (equal)
            TG_fail(tg,assertMemoryMessage(message, reason,
                            A, B, length, func, file, line));
}
