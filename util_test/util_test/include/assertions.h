/*
 * assertions.h
 *
 *  Created on: 23 abr. 2021
 *      Author: Fernando
 */

#ifndef INCLUDE_ASSERTIONS_H_
#define INCLUDE_ASSERTIONS_H_
#include <test.h>

void TG_assertTrue(TestGroup *tg,const char* message,int value,const char *func,const char *file,int line);
#define ASSERT_TRUE(tg,message,value) TG_assertTrue((tg),(message),(value),__func__,__FILE__,__LINE__)

void TG_assertFalse(TestGroup *tg,const char* message,int value,const char *func,const char *file,int line);
#define ASSERT_FALSE(tg,message,value) TG_assertFalse((tg),(message),(value),__func__,__FILE__,__LINE__)

void TG_assertIntEqual(TestGroup *tg,const char* message,long long int A,long long int B,const char *func,const char *file,int line);
#define ASSERT_INT_EQUAL(tg,message,A,B) TG_assertIntEqual((tg),(message),(A),(B),__func__,__FILE__,__LINE__)

void TG_assertIntNotEqual(TestGroup *tg,const char* message,long long int A,long long int B,const char *func,const char *file,int line);
#define ASSERT_INT_NOT_EQUAL(tg,message,A,B) TG_assertIntNotEqual((tg),(message),(A),(B),__func__,__FILE__,__LINE__)

void TG_assertMemoryEqual(TestGroup *tg,const char* message,const void *A,const void *B,size_t length,const char *func,const char *file, int line);
#define ASSERT_MEMORY_EQUAL(tg,message,A,B,length) TG_assertMemoryEqual((tg),(message),(A),(B),(length),__func__,__FILE__,__LINE__)

void TG_assertMemoryNotEqual(TestGroup *tg,const char* message,const void *A,const void *B,size_t length,const char *func,const char *file, int line);
#define ASSERT_MEMORY_NOT_EQUAL(tg,message,A,B,length) TG_assertMemoryNotEqual((tg),(message),(A),(B),(length),__func__,__FILE__,__LINE__)


void TG_fail(TestGroup *tg, const char *message, const char *reason,const char *func,const char *file, int line);
#define REPORT_FAIL(tg,message,reason) TG_fail(tg,message,reason,__func__,__FILE__,__LINE__)

void TG_error(TestGroup *tg,const char *message, const char *reason,const char *func,const char *file, int line);
#define REPORT_ERROR(tg,message,reason) TG_error(tg,message,reason,__func__,__FILE__,__LINE__)

#endif /* INCLUDE_ASSERTIONS_H_ */
