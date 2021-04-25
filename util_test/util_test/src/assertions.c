/*
 * assertions.c
 *
 *  Created on: 23 abr. 2021
 *      Author: Fernando
 */
#include <assertions.h>
#include <stdio.h> /*snprintf*/


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
