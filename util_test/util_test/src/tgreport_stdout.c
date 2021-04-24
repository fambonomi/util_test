/*
 * tgreport_default.c
 *
 *  Created on: 22 abr. 2021
 *      Author: Fernando
 */

#include <tgreporter_stdout.h>
#include <stdio.h> /*printf*/

static inline const char * changeNullPointerToEmptyString(const char *str)
{
	const int isNullPointer = NULL == str;

	if (isNullPointer)
		return "";
	else
		return str;
}
static void reportBegin(TGReporter *self,const char *groupName, int numTests)
{
	const char *const name = changeNullPointerToEmptyString(groupName);
	printf("---Begining---\nTest group: %s\nNumber of tests: %d\n\n",
			name,numTests);
	(void)self;
}

static void reportTest(TGReporter *self,const char *testName)
{
	const char *const msg = changeNullPointerToEmptyString(testName);
	printf(msg);
	(void)self;
}

static void reportPass(TGReporter *self)
{
	printf(" [OK]\n");
	(void)self;
}

static void reportFail(TGReporter *self,const char *message)
{
	const char *const msg = changeNullPointerToEmptyString(message);
	printf(" [FAIL]\n");
	printf("    Failure: %s\n",msg);
	(void)self;
}

static void reportError(TGReporter *self,const char *message)
{
	const char *const msg = changeNullPointerToEmptyString(message);
	printf(" [ERROR]\n");
	printf("    Error: %s\n",msg);
	(void)self;
}

static inline void printSummary(const TestGroupOutcome *summary)
{
	printf("\nOutcome:\nTests run: %d, passed: %d, failures: %d, errors: %d\n---End---\n\n",
			summary->run,
			summary->passed,
			summary->failed,
			summary->error);

}
static void reportSummary(TGReporter *self,const TestGroupOutcome *summary)
{
	const int isSummaryValid = NULL != summary;
	if (isSummaryValid)
		printSummary(summary);
	(void)self;
}


static TGReporter report;

TGReporter * TGReporter_stdout_getInstance(void)
{
	report.begin = reportBegin;
	report.test = reportTest;
	report.pass = reportPass;
	report.error = reportError;
	report.fail = reportFail;
	report.summary = reportSummary;
	return &report;
}
