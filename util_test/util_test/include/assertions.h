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


#endif /* INCLUDE_ASSERTIONS_H_ */
