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

static inline char *composeMessage(const char* message,const char* reason,const char *func,const char *file,int line)
{
	snprintf(buffer,BUFFER_LEN,"%s\n Reason: %s\n  In function %s\n  In file %s\n  In line %d\n",
			message,reason,func,file,line);
	return buffer;
}

void Assert_assertTrue(TestGroup *tg,const char* message,int value,const char *func,const char *file,int line)
{
	static const char failReason[]="Value that should be true is false";
	if(!value)
		TG_fail(tg,composeMessage(message,failReason,func,file,line));
}

void Assert_assertFalse(TestGroup *tg,const char* message,int value,const char *func,const char *file,int line)
{
	static const char failReason[]="Value that should be false is true";
	if(value)
		TG_fail(tg,composeMessage(message,failReason,func,file,line));
}
