/*
 * test.h
 *
 *  Created on: 20 abr. 2021
 *      Author: Fernando
 */

#ifndef INCLUDE_TEST_H_
#define INCLUDE_TEST_H_
#include <setjmp.h>

/* Declara que existe una struct TestGroup definida luego */
struct TestGroup;

/* Punteros a función par prueba, incialización y finalización*/

typedef void (*TG_Test)(struct TestGroup*);
typedef TG_Test TG_Before;
typedef TG_Test TG_After;

/**
 * Descriptor de función de prueba.
 */
typedef struct TestDescriptor{
	const char* description;
	TG_Test testFunction;
}TestDescriptor;

typedef struct TestGroup
{
	const char * groupName;
	TG_Before beforeGroup;
	TG_After afterGroup;
	TG_Before beforeTest;
	TG_After afterTest;
	TestDescriptor *tests;
	int numTests;
	struct{
		int run;
		int success;
		int failure;
		int error;
	}outcomeCounters;
	jmp_buf savedState;
	int canFail;
}TestGroup;

void TG_init(TestGroup *self, const char *groupName);

void TG_doBeforeGroup(TestGroup *self, TG_Before beginGroup);
void TG_doAfterGroup(TestGroup *self, TG_After afterGroup);
void TG_doBeforeTest(TestGroup *self,TG_Before beforeTest);
void TG_doAfterTest(TestGroup *self,TG_After afterTest);
void TG_setTests(TestGroup *self, TestDescriptor *tests, int numTests);
void TG_fail(TestGroup *self, const char *msg);
void TG_error(TestGroup *self, const char *msg);
void TG_runTests(TestGroup *self);
int TG_countExecuted(TestGroup *self);
int TG_countSuccessful(TestGroup *self);
int TG_countFailed(TestGroup *self);
int TG_countErrors(TestGroup *self);


#endif /* INCLUDE_TEST_H_ */
