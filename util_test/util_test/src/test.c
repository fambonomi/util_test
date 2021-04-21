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
	longjmp(self->savedState,1);
}

static inline void TG__recordSuccess(TestGroup *self)
{
	printf(" [OK]\n");
	++self->outcomeCounters.success;
}

static inline void TG__callAndRecordTest(TestGroup *self, TG_Test test)
{
	++self->outcomeCounters.run;
	test(self);
	TG__recordSuccess(self); /* in case of error this will not get executed */
}

static inline void TG__runAndManageErrors(TestGroup *self, TG_Test test)
{
	int backFromAFailure = setjmp(self->savedState) != 0;

	self->canFail = !backFromAFailure;

	if(!backFromAFailure)
		TG__callAndRecordTest(self,test);

	self->canFail = 0;

}

static inline void TG__validateAndRunTest(TestGroup *self, int i)
{
	const TestDescriptor *descriptor= &self->tests[i];
	const int validTest = descriptor->testFunction != (TG_Test)0;

	if (validTest){
		printf(descriptor->description);
		TG__runAndManageErrors(self, descriptor->testFunction);
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
			self->groupName,self->numTests);
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
	self->tests = tests;
	self->numTests=numTests;
}

void TG_doBeforeGroup(TestGroup *self, TG_Before beforeGroup)
{
	self->beforeGroup = beforeGroup;
}

void TG_doAfterGroup(TestGroup *self, TG_After afterGroup)
{
	self->afterGroup = afterGroup;
}
void TG_doBeforeTest(TestGroup *self,TG_Before beforeTest)
{
	self->beforeTest = beforeTest;
}

void TG_doAfterTest(TestGroup *self,TG_After afterTest)
{
	self->afterTest=afterTest;
}

int TG_countExecuted(TestGroup *self)
{
	return self->outcomeCounters.run;
}

void TG_runTests(TestGroup *self)
{
	int i;
	TG__resetOutcomeCounts(self);
	TG__showTitle(self);
	for(i=0;i<self->numTests;++i)
		TG__validateAndRunTest(self,i);

	TG__showOutcome(self);
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
	if(self->canFail)
	{
		TG__recordFailure(self,msg);
		TG__recoverFromFailureOrError(self);
	}
}

void TG_error(TestGroup *self, const char *msg)
{
	if(self->canFail)
	{
		TG__recordError(self,msg);
		TG__recoverFromFailureOrError(self);
	}
}
