/*
 * test.c
 *
 *  Created on: 20 abr. 2021
 *      Author: Fernando
 */


#include <test.h>
#include <setjmp.h> /*setjmp, longjmp*/
#include <string.h> /*memset*/
#include <stdio.h> /*printf*/
#include <stddef.h> /*NULL*/

/**
 * Static function section
 */

static inline void TG__recordFailure(TestGroup *self, const char *msg)
{
	printf(" [FAIL]\n");
	printf("    Failure: %s\n",msg);
	++self->outcomeCounters.failure;
}

static inline void TG__recordError(TestGroup *self, const char *msg)
{
	printf(" [ERROR]\n");
	printf("    Error: %s\n",msg);
	++self->outcomeCounters.error;
}

static inline void TG__recoverFromFailureOrError(TestGroup *self)
{
	longjmp(self->testingState.savedState,1);
}

static inline void TG__recordSuccess(TestGroup *self)
{
	printf(" [OK]\n");
	++self->outcomeCounters.success;
}

static inline void TG__doBeforeTest(TestGroup *self)
{
	const TG_Before beforeFn = self->testActions.beforeTest;
	const int beforeFnIsValid = (TG_Before) 0 != beforeFn;

	if(beforeFnIsValid)
		beforeFn(self);
}

static inline TestDescriptor *TG__getCurrentTestDescriptor(TestGroup *self)
{
	const int currentTest = self->testingState.currentTest;
	const int isRunning = currentTest < self->testActions.numTests;

	if (isRunning)
		return &self->testActions.tests[currentTest];
	else
		return NULL;
}
static inline TG_Test TG__getCurrentTestAction(TestGroup *self)
{
	const TestDescriptor *descriptor = TG__getCurrentTestDescriptor(self);
	const int descriptorIsValid = NULL == descriptor ;

	if (descriptorIsValid)
		return descriptor->testFunction;
	else
		return (TG_Test)0;
}
static inline void TG__doTest(TestGroup *self)
{
	TG_Test test = TG__getCurrentTestAction(self);
	const int validTest = (TG_Test) 0 != test;

	if (validTest)
		test(self);
	else
		TG_error(self, "Missing testing function");
}

static inline void TG__callAndRecordTest(TestGroup *self, TG_Test test)
{
	++self->outcomeCounters.run;
	TG__doBeforeTest(self);
	test(self);
	TG__recordSuccess(self); /* in case of error this will not get executed */
}


static inline void TG__runTestAndManageErrors(TestGroup *self, TG_Test test)
{
	int backFromAFailure = setjmp(self->testingState.savedState) != 0;

	self->testingState.canFail = !backFromAFailure;

	if(!backFromAFailure)
		TG__callAndRecordTest(self,test);

	self->testingState.canFail = 0;

}


static inline void TG__announceAndRunTest(TestGroup *self)
{
	const TestDescriptor *descriptor= TG__getCurrentTestDescriptor(self);
	const int isValidDescriptor = NULL != descriptor;
	if (isValidDescriptor)
	{
		printf(descriptor->description);
		TG__runTestAndManageErrors(self, descriptor->testFunction);
	}
}

static inline void TG__showOutcome(TestGroup *self)
{
	printf("\nOutcome:\nTests run: %d, success: %d, failures: %d, errors: %d\n---End---\n\n",
			TG_countExecuted(self),
			TG_countSuccessful(self),
			TG_countFailed(self),
			TG_countErrors(self));
}

static inline void TG__resetOutcomeCounts(TestGroup *self)
{
	memset(&self->outcomeCounters,0,sizeof(self->outcomeCounters));
}

static inline void TG__showTitle(TestGroup *self)
{
	printf("---Begining---\nTest group: %s\nNumber of tests: %d\n\n",
			self->groupName,self->testActions.numTests);
}

static inline void TG__selectFirstTest(TestGroup *self)
{
	self->testingState.currentTest=0;
}
static inline void TG__selectNextTest(TestGroup *self)
{
	++self->testingState.currentTest;
}
static inline int TG__reachedEndOfTests(TestGroup *self)
{
	return self->testingState.currentTest >= self->testActions.numTests;
}

static inline void TG__initTestRun(TestGroup *self)
{
	TG__showTitle(self);
	TG__resetOutcomeCounts(self);
	TG__selectFirstTest(self);
}

static inline void TG__finalizeTestRun(TestGroup *self)
{
	TG__showOutcome(self);
}

/**
 * Public API
 */

void TG_init(TestGroup *self,const char *groupName)
{
	memset(self,0,sizeof(*self));
	self->groupName = groupName;
}

void TG_setTests(TestGroup *self, TestDescriptor *tests,int numTests)
{
	self->testActions.tests = tests;
	self->testActions.numTests=numTests;
}

void TG_setBeforeGroupAction(TestGroup *self, TG_Before beforeGroup)
{
	self->testActions.beforeGroup = beforeGroup;
}

void TG_setAfterGroupAction(TestGroup *self, TG_After afterGroup)
{
	self->testActions.afterGroup = afterGroup;
}
void TG_setBeforeTestAction(TestGroup *self,TG_Before beforeTest)
{
	self->testActions.beforeTest = beforeTest;
}

void TG_setAfterTestAction(TestGroup *self,TG_After afterTest)
{
	self->testActions.afterTest=afterTest;
}

int TG_countExecuted(TestGroup *self)
{
	return self->outcomeCounters.run;
}

void TG_runTests(TestGroup *self)
{
	TG__initTestRun(self);
	while (!TG__reachedEndOfTests(self))
	{
		TG__announceAndRunTest(self);
		TG__selectNextTest(self);
	}
	TG__finalizeTestRun(self);
}


int TG_countSuccessful(TestGroup *self)
{
	return self->outcomeCounters.success;
}

int TG_countFailed(TestGroup *self)
{
	return self->outcomeCounters.failure;
}

int TG_countErrors(TestGroup *self)
{
	return self->outcomeCounters.error;
}


void TG_fail(TestGroup *self, const char *msg)
{
	if(self->testingState.canFail)
	{
		TG__recordFailure(self,msg);
		TG__recoverFromFailureOrError(self);
	}
}

void TG_error(TestGroup *self, const char *msg)
{
	if(self->testingState.canFail)
	{
		TG__recordError(self,msg);
		TG__recoverFromFailureOrError(self);
	}
}
