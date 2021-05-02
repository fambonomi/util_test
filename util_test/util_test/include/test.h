/*
 * test.h
 *
 *  Created on: 20 abr. 2021
 *      Author: Fernando
 */

#ifndef INCLUDE_TEST_H_
#define INCLUDE_TEST_H_
#include <setjmp.h>
#include <iassertion.h>
#include <stddef.h> /*size_t*/
/* Declara que existe una struct TestGroup definida luego */
typedef struct TestGroup TestGroup;

/**
 * struct plugin de salida
 */
typedef struct TGReporter TGReporter;

/* Punteros a función par prueba, incialización y finalización*/

typedef void (*TG_Test)(TestGroup* tg);
typedef TG_Test TG_Before;
typedef TG_Test TG_After;

/**
 * Descriptor de función de prueba.
 */
typedef struct TestDescriptor{
	const char* description;
	TG_Test testFunction;
}TestDescriptor;

typedef struct TestGroupOutcome{
	int run;
	int passed;
	int failed;
	int error;
}TestGroupOutcome;


typedef struct TestGroup
{
	const char * groupName;
	TGReporter *reportPlugin;
	struct{
		TG_Before beforeGroup;
		TG_After afterGroup;
		TG_Before beforeTest;
		TG_After afterTest;
		TestDescriptor *tests;
		int numTests;
	}testActions;
    TestGroupOutcome outcomeCounters;
	struct{
		jmp_buf savedState;
		int canFail;
		int currentTest;
		int hasFailed;
		int hadError;
	}testingState;
}TestGroup;

void TG_init(TestGroup *self, const char *groupName);

void TG_setReportPlugin(TestGroup *self, TGReporter *plugin);
void TG_setBeforeGroupAction(TestGroup *self, TG_Before beginGroup);
void TG_setAfterGroupAction(TestGroup *self, TG_After afterGroup);
void TG_setBeforeTestAction(TestGroup *self,TG_Before beforeTest);
void TG_setAfterTestAction(TestGroup *self,TG_After afterTest);
void TG_setTests(TestGroup *self, TestDescriptor *tests, int numTests);
void TG_runTests(TestGroup *self);
int TG_countExecuted(TestGroup *self);
int TG_countPassed(TestGroup *self);
int TG_countFailed(TestGroup *self);
int TG_countErrors(TestGroup *self);
const TestGroupOutcome *TG_getTestOutcome(TestGroup *self);
int TG_allTestsPassed(TestGroup *self);

#endif /* INCLUDE_TEST_H_ */
