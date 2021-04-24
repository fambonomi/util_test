/*
 * assertions.h
 *
 *  Created on: 23 abr. 2021
 *      Author: Fernando
 */

#ifndef INCLUDE_ASSERTIONS_H_
#define INCLUDE_ASSERTIONS_H_
#include <test.h>

void Assert_assertTrue(TestGroup *tg,const char* message,int value,const char *func,const char *file,int line);


#define ASSERT_TRUE(tg,message,value) Assert_assertTrue((tg),(message),(value),__func__,__FILE__,__LINE__)

void Assert_assertFalse(TestGroup *tg,const char* message,int value,const char *func,const char *file,int line);

#define ASSERT_FALSE(tg,message,value) Assert_assertFalse((tg),(message),(value),__func__,__FILE__,__LINE__)


#endif /* INCLUDE_ASSERTIONS_H_ */
