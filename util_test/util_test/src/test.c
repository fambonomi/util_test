/*
 * test.c
 *
 *  Created on: 20 abr. 2021
 *      Author: Fernando
 */


#include <test_impl.h>
#include <setjmp.h> /*setjmp, longjmp*/
#include <string.h> /*memset*/
#include <stddef.h> /*NULL*/
#include <tgreporter_stdout.h>
#include <iassertion_impl.h>

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

static inline const char* stringOrDefault(const char *message,const char *defaultValue)
{
	const int isMessageNull = NULL == message;
	const int isDefaultNull = NULL == defaultValue;

	if (isMessageNull)
	{
		if(isDefaultNull)
			return "<NULL>";
		else
			return defaultValue;
	}
	return message;
}

static inline void TG__flagFailure(TestGroup *self, const TestAssertion *ta)
{
	self->testingState.hasFailed=1;

	TGReporter * const reporter = TG__getReporter(self);
	TGR_reportFail(reporter, ta);
}

static inline void TG__flagError(TestGroup *self, const TestAssertion *ta)
{
	self->testingState.hadError=1;

	TGReporter *const reporter =TG__getReporter(self);
	TGR_reportError(reporter, ta);
}

static inline void TG__recoverFromFailureOrError(TestGroup *self)
{
	longjmp(self->testingState.savedState,1);
}

static inline void TG__recordErrorOutcome(TestGroup *self)
{
	++self->outcomeCounters.error;

}
static inline void TG__recordFailureOutcome(TestGroup *self)
{
	++self->outcomeCounters.failed;
}
static inline void TG__recordAndFlagPassedOutcome(TestGroup *self)
{
	++self->outcomeCounters.passed;
	TGReporter *const reporter = TG__getReporter(self);
	TGR_reportPass(reporter);
}
static inline void TG__recordOutcome(TestGroup *self)
{
	const int hadError = self->testingState.hadError;
	const int hasFailed = !hadError && self->testingState.hasFailed;

	if (hadError)
		TG__recordErrorOutcome(self);
	else if (hasFailed)
		TG__recordFailureOutcome(self);
	else
		TG__recordAndFlagPassedOutcome(self);
}
static inline void TG__initTestOutcome(TestGroup *self)
{
	self->testingState.hadError=0;
	self->testingState.hasFailed=0;
}
static inline void TG__doBeforeAction(TestGroup *self, TG_Before action)
{
    const int isValid = (TG_Before) 0 != action;

    if (isValid)
        action(self);
}
static inline void TG__doBeforeGroup(TestGroup *self)
{
    const TG_Before action = self->testActions.beforeGroup;
    TG__doBeforeAction(self, action);
}
static inline void TG__doBeforeTest(TestGroup *self)
{
	const TG_Before action = self->testActions.beforeTest;
	TG__doBeforeAction(self,action);
}

static inline void TG__doAfterAction(TestGroup *self, TG_After action)
{
    const int isValid = (TG_After) 0 != action;

    if (isValid)
        action(self);
}

static inline void TG__doAfterTest(TestGroup *self)
{
	const TG_After action = self->testActions.afterTest;
	TG__doAfterAction(self, action);
}

static inline void TG__doAfterGroup(TestGroup *self)
{
    const TG_After action = self->testActions.afterGroup;
    TG__doAfterAction(self, action);
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
		return stringOrDefault(descriptor->description, "Test without description.");
	else
		return "Invalid test descriptor!";
}
static inline void TG__reportMissingTestFunction(TestGroup *self)
{
    TestAssertion ta;
    TA_init(&ta,TAK_DIRECT);
    TA_setResult(&ta, TA_ERROR);
    TA_setMessage(&ta, "Cannot proceed with test");
    TA_setReason(&ta, "Pointer to testing function is null pointer");
    TG_reportAssertionResult(self, &ta);
}
static inline void TG__doTest(TestGroup *self)
{
	TG_Test test = TG__getCurrentTestAction(self);
	const int validTest = (TG_Test) 0 != test;

	if (validTest)
		test(self);
	else
		TG__reportMissingTestFunction(self);
}

static inline void TG__callAndRecordTest(TestGroup *self)
{
	++self->outcomeCounters.run;
	TG__doBeforeTest(self);
	TG__doTest(self);
}


static inline void TG__runActionThatCanFail(TestGroup *self,void (*action)(TestGroup *self))
{
	int backFromAFailure = setjmp(self->testingState.savedState) != 0;

	self->testingState.canFail = !backFromAFailure;

	if(!backFromAFailure)
		action(self);

	self->testingState.canFail = 0;

}

static inline void TG__runTestAndManageErrors(TestGroup *self)
{
	TG__initTestOutcome(self);
	/* doBeforeTest is executed in the same error frame than test to prevent test from running on a failed initialization*/
	TG__runActionThatCanFail(self, TG__callAndRecordTest);
	/* doAfterTest is executed here so that de-initialization can proceed even if the test has failed */
	TG__runActionThatCanFail(self, TG__doAfterTest);

	TG__recordOutcome(self); /* in case of error this will not get executed */
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
	self->outcomeCounters.run=0;
	self->outcomeCounters.passed=0;
	self->outcomeCounters.failed=0;
	self->outcomeCounters.error=0;
}

static inline void TG__showTitle(TestGroup *self)
{
	TGReporter *reporter = TG__getReporter(self);
	const char *const groupName =
			stringOrDefault(self->groupName, "Unnamed test group.");
	const int numberOfTests = self->testActions.numTests;

	TGR_reportBegin(reporter, groupName, numberOfTests);
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
static inline void TG__finishTestRun(TestGroup *self)
{
    self->testingState.currentTest=self->testActions.numTests;
}
static inline void TG__updateAfterBeforeGroupAction(TestGroup *self)
{
    const int hadError = self->testingState.hadError;
    const int hasFailed = !hadError && self->testingState.hasFailed;
    if (hadError)
        TG__recordErrorOutcome(self);
    else if (hasFailed)
        TG__recordFailureOutcome(self);

    if(hadError || hasFailed)
        TG__finishTestRun(self);
}

static inline void TG__initTestRun(TestGroup *self)
{
	TG__showTitle(self);
	TG__resetOutcomeCounts(self);
	TG__selectFirstTest(self);
	TG__initTestOutcome(self);
    TG__runActionThatCanFail(self, TG__doBeforeGroup);
    TG__updateAfterBeforeGroupAction(self);
}

static inline void TG__finalizeTestRun(TestGroup *self)
{
    TG__initTestOutcome(self);
    TG__runActionThatCanFail(self, TG__doAfterGroup);
    TG__updateAfterBeforeGroupAction(self);
	TG__showOutcome(self);
}

static void TG__flagAssertion(TestGroup *self, const TestAssertion *ta)
{
    const TAResult result = TA_getResult(ta);
    const int isFailure = TA_FAIL == result;
    const int isError = TA_ERROR == result;

    if(isFailure)
        TG__flagFailure(self,ta);
    else if(isError)
        TG__flagError(self,ta);

    if(isFailure || isError)
        TG__recoverFromFailureOrError(self);
}
/**
 * Public implementation API
 */

void TG_reportAssertionResult(TestGroup *self, const TestAssertion *ta)
{
    if (self->testingState.canFail)
        TG__flagAssertion(self,ta);
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
	return self->outcomeCounters.failed;
}

int TG_countErrors(TestGroup *self)
{
	return self->outcomeCounters.error;
}

const TestGroupOutcome *TG_getTestOutcome(TestGroup *self)
{
	return &self->outcomeCounters;
}
int TG_allTestsPassed(TestGroup *self)
{
	const int numberOfTests = self->testActions.numTests;
	const int testsPassed = self->outcomeCounters.passed;

	return testsPassed == numberOfTests;
}
