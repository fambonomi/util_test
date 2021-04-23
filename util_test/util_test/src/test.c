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
#include <tgreporter_stdout.h>
/**
 * Static function section
 */

static inline void TG__setDefaultReporter(TestGroup *self)
{
	self->reportPlugin = TGReporter_stdout_getInstance();
}
static inline TGReporter * TG__getReporter(TestGroup *self)
{
	const int haveValidReportPlugin = NULL != self->reportPlugin;

	if (!haveValidReportPlugin)
		TG__setDefaultReporter(self);

	return self->reportPlugin;
}

static inline void TG__recordFailure(TestGroup *self, const char *msg)
{
	++self->outcomeCounters.failure;

	TGReporter * const reporter = TG__getReporter(self);
	TGR_reportFail(reporter, msg);
}

static inline void TG__recordError(TestGroup *self, const char *msg)
{
	++self->outcomeCounters.error;

	TGReporter *const reporter =TG__getReporter(self);
	TGR_reportError(reporter, msg);
}

static inline void TG__recoverFromFailureOrError(TestGroup *self)
{
	longjmp(self->testingState.savedState,1);
}

static inline void TG__recordSuccess(TestGroup *self)
{
	++self->outcomeCounters.passed;

	TGReporter *const reporter = TG__getReporter(self);
	TGR_reportPass(reporter);
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
	TestDescriptor *const descriptor = TG__getCurrentTestDescriptor(self);
	const int descriptorIsValid = NULL != descriptor;

	if (descriptorIsValid)
		return descriptor->testFunction;
	else
		return (TG_Test)0;
}
static inline const char *TG__describeCurrentTest(TestGroup *self)
{
	TestDescriptor *const descriptor = TG__getCurrentTestDescriptor(self);
	const int descriptorIsValid = NULL != descriptor;

	if (descriptorIsValid)
		return descriptor->description;
	else
		return NULL;
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

static inline void TG__callAndRecordTest(TestGroup *self)
{
	++self->outcomeCounters.run;
	TG__doBeforeTest(self);
	TG__doTest(self);
	TG__recordSuccess(self); /* in case of error this will not get executed */
}


static inline void TG__runTestAndManageErrors(TestGroup *self)
{
	int backFromAFailure = setjmp(self->testingState.savedState) != 0;

	self->testingState.canFail = !backFromAFailure;

	if(!backFromAFailure)
		TG__callAndRecordTest(self);

	self->testingState.canFail = 0;

}

static inline void TG__announceTest(TestGroup *self)
{
	const char *const description = TG__describeCurrentTest(self);
	TGReporter *reporter = TG__getReporter(self);
	TGR_reportTest(reporter, description);
}
static inline void TG__announceAndRunTest(TestGroup *self)
{
	TestDescriptor *const descriptor= TG__getCurrentTestDescriptor(self);
	const int isValidDescriptor = NULL != descriptor;
	if (isValidDescriptor)
	{
		TG__announceTest(self);
		TG__runTestAndManageErrors(self);
	}
}

static inline void TG__showOutcome(TestGroup *self)
{
	TGReporter *const reporter = TG__getReporter(self);

	TGR_reportSummary(reporter, &self->outcomeCounters);
}

static inline void TG__resetOutcomeCounts(TestGroup *self)
{
	memset(&self->outcomeCounters,0,sizeof(self->outcomeCounters));
}

static inline void TG__showTitle(TestGroup *self)
{
	TGReporter *reporter = TG__getReporter(self);
	TGR_reportBegin(reporter, self->groupName, self->testActions.numTests);
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

void TG_setReportPlugin(TestGroup *self,TGReporter *plugin)
{
	self->reportPlugin=plugin;
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


int TG_countPassed(TestGroup *self)
{
	return self->outcomeCounters.passed;
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
