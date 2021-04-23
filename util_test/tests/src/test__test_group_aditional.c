/*
 * test__test_group_adicional.c
 *
 *  Created on: 22 abr. 2021
 *      Author: Fernando
 */

#include <test.h>
#include <string.h> /*memset*/
#include <stdio.h> /*snprintf*/
#include <tgreporter_stub.h>

#define STRING_BUFFER_SIZE 256
struct {
	TestGroup tg;
	struct{
		TestGroup group;
	}mockTg;
	struct{
		int beforeTestActionRun;
		int testRun;
	}counters;
	char stringBuffer[STRING_BUFFER_SIZE];
}state;


static inline void countBeforeActionRun()
{
	++state.counters.beforeTestActionRun;
}

static inline void countTestRun()
{
	++state.counters.testRun;
}
static inline void resetCounters()
{
	memset(&state.counters,0,sizeof(state.counters));
}

static void mockBeforeTestSucceed(TestGroup *tg)
{
	countBeforeActionRun();
	(void)tg;
}

static void mockBeforeTestFail(TestGroup *tg)
{
	countBeforeActionRun();
	TG_fail(tg, "Before test action failed");
}

static void mockBeforeTestError(TestGroup *tg)
{
	countBeforeActionRun();
	TG_error(tg, "Before test action encountered an error");
}


static void mockTestPass(TestGroup *tg)
{
	countTestRun();
	(void)tg;
}

static void mockTestFail(TestGroup *tg)
{
	countTestRun();
	TG_fail(tg,"Test failed");
}

static void mockTestError(TestGroup *tg)
{
	countTestRun();
	TG_error(tg,"Test encountered an error");
}

static inline int integersAreEqual(int A,int B)
{
	return A==B;
}

enum{NUM_MOCK_TESTS = 3,
	NUM_MOCK_PASS = 1,
	NUM_MOCK_FAIL = 1,
	NUM_MOCK_ERROR = 1
};

static TestDescriptor mockTests[NUM_MOCK_TESTS]={
		{"Test that passes",mockTestPass},
		{"Test that fails",mockTestFail},
		{"Test that encounters an error",mockTestError}
};

static inline void initMockTests(void)
{
	TestGroup *const hMockTg = &state.mockTg.group;

	TG_init(hMockTg, "Mock test group");
	TG_setTests(hMockTg, mockTests, NUM_MOCK_TESTS);
	TG_setReportPlugin(hMockTg, TGReporter_stub_getInstance());
	state.counters.beforeTestActionRun=0;
	state.counters.testRun=0;

}

static void test_BeforeTestActionSucceed(TestGroup *tg)
{
	TestGroup *const hMockTg = &state.mockTg.group;

	initMockTests();

	TG_setBeforeTestAction(hMockTg, mockBeforeTestSucceed);
	TG_runTests(hMockTg);

	const int ranTests = integersAreEqual(state.counters.testRun,NUM_MOCK_TESTS);

	const int ranBeforeTestAction =
			integersAreEqual(state.counters.beforeTestActionRun,NUM_MOCK_TESTS);

	const int resultsAreAsExpected =
			(TG_countPassed(hMockTg) == NUM_MOCK_PASS) &&
			(TG_countFailed(hMockTg) == NUM_MOCK_FAIL) &&
			(TG_countErrors(hMockTg) == NUM_MOCK_ERROR);

	if (!ranBeforeTestAction)
		TG_fail(tg,"Failed to run action before all the tests!");
	if (!ranTests)
		TG_fail(tg,"Failed to run all the tests!");
	if (!resultsAreAsExpected)
		TG_fail(tg,"Test outcomes are different than expected!");

}

static void test_BeforeTestActionFail(TestGroup *tg)
{
	TestGroup *const hMockTg = &state.mockTg.group;

	initMockTests();

	TG_setBeforeTestAction(hMockTg, mockBeforeTestFail);
	TG_runTests(hMockTg);

	const int didntRunTestActions = integersAreEqual(state.counters.testRun,0);

	const int ranBeforeTestAction =
			integersAreEqual(state.counters.beforeTestActionRun,NUM_MOCK_TESTS);

	const int resultsAreAsExpected =
			(TG_countPassed(hMockTg) == 0) &&
			(TG_countFailed(hMockTg) == 3) &&
			(TG_countErrors(hMockTg) == 0);

	if (!ranBeforeTestAction)
		TG_fail(tg,"Failed to run action before all the tests!");
	if (!didntRunTestActions)
		TG_fail(tg,"Ran test actions when beforeTest action failed!");
	if (!resultsAreAsExpected)
		TG_fail(tg,"Test outcomes are different than expected!");

}

static void test_BeforeTestActionError(TestGroup *tg)
{
	TestGroup *const hMockTg = &state.mockTg.group;

	initMockTests();

	TG_setBeforeTestAction(hMockTg, mockBeforeTestError);
	TG_runTests(hMockTg);

	const int didntRunTestActions = integersAreEqual(state.counters.testRun,0);

	const int ranBeforeTestAction =
			integersAreEqual(state.counters.beforeTestActionRun,NUM_MOCK_TESTS);

	const int resultsAreAsExpected =
			(TG_countPassed(hMockTg) == 0) &&
			(TG_countFailed(hMockTg) == 0) &&
			(TG_countErrors(hMockTg) == 3);

	if (!ranBeforeTestAction)
		TG_fail(tg,"Failed to run action before all the tests!");
	if (!didntRunTestActions)
		TG_fail(tg,"Ran test actions when beforeTest action encountered error!");
	if (!resultsAreAsExpected)
		TG_fail(tg,"Test outcomes are different than expected!");

}


static TestDescriptor tests[]={
		{"Run action that succeeds before each test",test_BeforeTestActionSucceed},
		{"Run action that fails before each test",test_BeforeTestActionFail},
		{"Run action that encounters error before each test",test_BeforeTestActionError}
};
static int numTests = sizeof(tests)/sizeof(*tests);


int testRun_TestGroup_aditional(void)
{
	TestGroup * tg = &state.tg;
	TG_init(tg,"Test additional functions of TestGroup runner");
	TG_setTests(tg, tests, numTests);

	TG_runTests(tg);

	return TG_countErrors(tg)+TG_countFailed(tg);
}
