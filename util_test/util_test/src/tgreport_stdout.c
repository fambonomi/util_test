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

static const char *  operatorString(TAOperator op)
{
    switch (op)
    {
    case TAO_EQ:
        return "==";
    case TAO_NEQ:
        return "!=";
    case TAO_IS_FALSE:
        return "Is false";
    case TAO_IS_TRUE:
        return "Is true";
    default:
        break;
    }
    return "<missing operator>";
}

static void reportPosition(const TestAssertion *ta)
{
    printf("    In function: %s\n    In file: %s\n    In line: %d\n",
            TA_getFunc(ta),TA_getFile(ta),TA_getLine(ta));
}
static void reportFailBase(const TestAssertion *ta)
{
    printf(" [FAIL]\n    Failure:%s\n    Reason:%s\n",TA_getMessage(ta),TA_getReason(ta));
}
static void reportFail_bool(const TestAssertion *ta)
{
    const char *const A = TA_bool_getA(ta)? "true":"false";
    const char *const Op = operatorString(TA_bool_getOperator(ta));
    printf("    Failed Assert: %s %s\n",A,Op);
}
static void reportFail_num(const TestAssertion *ta)
{
    const long long int A = TA_num_getA(ta);
    const long long int B = TA_num_getB(ta);
    const char *const Op = operatorString(TA_num_getOperator(ta));
    printf("    Failed Assert: %lld %s %lld\n",A,Op,B);
}
static void reportFail_mem(const TestAssertion *ta)
{
    const void *const A = TA_mem_getA(ta);
    const void *const B = TA_mem_getB(ta);
    const size_t length = TA_mem_getLength(ta);
    const char *const Op = operatorString(TA_mem_getOperator(ta));
    printf("    Failed Assert: [%u bytes stored at %p] %s [%u bytes stored at %p]\n",
            length,A,Op,length,B);
}
static void reportFailedAssert(const TestAssertion *ta)
{
    switch(TA_getKind(ta))
    {
    case TAK_BOOL:
        reportFail_bool(ta);
        break;
    case TAK_NUM:
        reportFail_num(ta);
        break;
    case TAK_MEM:
        reportFail_mem(ta);
        break;
    default:
        break;
    }
}
static void reportFail(TGReporter *self,const TestAssertion *ta)
{
    reportFailBase(ta);
    reportFailedAssert(ta);
	reportPosition(ta);
	(void)self;
}
static void reportErrorBase(const TestAssertion *ta)
{
    printf(" [ERROR]\n    Error:%s\n    Reason:%s\n",TA_getMessage(ta),TA_getReason(ta));
}

static void reportError(TGReporter *self,const TestAssertion *ta)
{
    reportErrorBase(ta);
    reportFailedAssert(ta);
    reportPosition(ta);
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
