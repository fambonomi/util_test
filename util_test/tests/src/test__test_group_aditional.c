/*
 * test__test_group_adicional.c
 *
 *  Created on: 22 abr. 2021
 *      Author: Fernando
 */

#include <test.h>
#include <string.h> /*memset*/
#include <tgreporter_stub.h>

#define STRING_BUFFER_SIZE 256
struct {
	TestGroup tg;
	struct{
		TestGroup group;
	}mockTg;
	struct{
		int actionRun;
		int testRun;
	}counters;
}state;


static inline void countActionRun()
{
	++state.counters.actionRun;
}

static inline void countTestRun()
{
	++state.counters.testRun;
}
static inline void resetCounters()
{
	memset(&state.counters,0,sizeof(state.counters));
}

static void mockActionSucceed(TestGroup *tg)
{
	countActionRun();
	(void)tg;
}

static void mockActionFail(TestGroup *tg)
{
	countActionRun();
	TG_fail(tg, "Before test action failed");
}

static void mockActionError(TestGroup *tg)
{
	countActionRun();
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

static inline int outcomesAreEqual(const TestGroupOutcome *A,const TestGroupOutcome *B)
{
	return A->run == B->run &&
		   A->passed == B->passed &&
		   A->failed == B->failed &&
		   A->error == B->error;
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
	state.counters.actionRun=0;
	state.counters.testRun=0;

}

static void test_BeforeTestActionSucceed(TestGroup *tg)
{
	TestGroup *const hMockTg = &state.mockTg.group;

	initMockTests();

	TG_setBeforeTestAction(hMockTg, mockActionSucceed);
	TG_runTests(hMockTg);

	const int ranTests = integersAreEqual(state.counters.testRun,NUM_MOCK_TESTS);

	const int ranBeforeTestAction =
			integersAreEqual(state.counters.actionRun,NUM_MOCK_TESTS);

	const TestGroupOutcome expectedOutcome = {.run=NUM_MOCK_TESTS,.passed=NUM_MOCK_PASS,.failed=NUM_MOCK_FAIL,.error=NUM_MOCK_ERROR};
	const int resultsAreAsExpected = outcomesAreEqual(&expectedOutcome, TG_getTestOutcome(hMockTg));

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

	TG_setBeforeTestAction(hMockTg, mockActionFail);
	TG_runTests(hMockTg);

	const int didntRunTestActions = integersAreEqual(state.counters.testRun,0);

	const int ranBeforeTestAction =
			integersAreEqual(state.counters.actionRun,NUM_MOCK_TESTS);

	const TestGroupOutcome expectedOutcome = {.run=NUM_MOCK_TESTS,.passed=0,.failed=NUM_MOCK_TESTS,.error=0};
	const int resultsAreAsExpected = outcomesAreEqual(&expectedOutcome, TG_getTestOutcome(hMockTg));

	if (!ranBeforeTestAction)
		TG_fail(tg,"Failed to run action before all the tests!");
	if (!didntRunTestActions)
		TG_fail(tg,"Ran test actions when beforeTest action failed!");
	if (!resultsAreAsExpected)
		TG_fail(tg,"Test outcomes are different than expected. If init declares a fail test fails!");

}

static void test_BeforeTestActionError(TestGroup *tg)
{
	TestGroup *const hMockTg = &state.mockTg.group;

	initMockTests();

	TG_setBeforeTestAction(hMockTg, mockActionError);
	TG_runTests(hMockTg);

	const int didntRunTestActions = integersAreEqual(state.counters.testRun,0);

	const int ranBeforeTestAction =
			integersAreEqual(state.counters.actionRun,NUM_MOCK_TESTS);

	const TestGroupOutcome expectedOutcome = {.run=NUM_MOCK_TESTS,.passed=0,.failed=0,.error=NUM_MOCK_TESTS};
	const int resultsAreAsExpected = outcomesAreEqual(&expectedOutcome, TG_getTestOutcome(hMockTg));


	if (!ranBeforeTestAction)
		TG_fail(tg,"Failed to run action before each test!");
	if (!didntRunTestActions)
		TG_fail(tg,"Ran test actions when beforeTest action encountered error!");
	if (!resultsAreAsExpected)
		TG_fail(tg,"Test outcomes are different than expected!");

}
static void test_AfterTestActionSucceed(TestGroup *tg)
{
	TestGroup *const hMockTg = &state.mockTg.group;

	initMockTests();

	TG_setAfterTestAction(hMockTg, mockActionSucceed);
	TG_runTests(hMockTg);

	const int ranTestActions = integersAreEqual(state.counters.testRun,NUM_MOCK_TESTS);

	const int ranAfterTestAction =
			integersAreEqual(state.counters.actionRun,NUM_MOCK_TESTS);

	const TestGroupOutcome expectedOutcome = {.run=NUM_MOCK_TESTS,.passed=NUM_MOCK_PASS,.failed=NUM_MOCK_FAIL,.error=NUM_MOCK_ERROR};
	const int resultsAreAsExpected = outcomesAreEqual(&expectedOutcome, TG_getTestOutcome(hMockTg));


	if (!ranAfterTestAction)
		TG_fail(tg, "Failed to run action after each test (actions should run even if the test fails/errors)!");
	if (!ranTestActions)
		TG_fail(tg,"Failed to run test bodies!");
	if (!resultsAreAsExpected)
		TG_fail(tg, "Test outcomes are different than expected!");
}

static void test_AfterTestActionFail(TestGroup *tg)
{
	TestGroup *const hMockTg = &state.mockTg.group;

	initMockTests();

	TG_setAfterTestAction(hMockTg, mockActionFail);
	TG_runTests(hMockTg);

	const int ranTestActions = integersAreEqual(state.counters.testRun,NUM_MOCK_TESTS);

	const int ranAfterTestAction =
			integersAreEqual(state.counters.actionRun,NUM_MOCK_TESTS);

	const TestGroupOutcome expectedOutcome = {.run=NUM_MOCK_TESTS,.passed=0,.failed=NUM_MOCK_TESTS-NUM_MOCK_ERROR,.error=NUM_MOCK_ERROR};
	const int resultsAreAsExpected = outcomesAreEqual(&expectedOutcome, TG_getTestOutcome(hMockTg));


	if (!ranAfterTestAction)
		TG_fail(tg, "Failed to run action after each test (actions should run even if the test fails/errors)!");
	if (!ranTestActions)
		TG_fail(tg,"Failed to run test bodies (as there is no init step that can fail tests should allways run)!");
	if (!resultsAreAsExpected)
		TG_fail(tg, "Test outcomes are different than expected. If de-init declares a fail, the outcome can be either fail or error (if the test declared an error)!");
}

static void test_AfterTestActionError(TestGroup *tg)
{
	TestGroup *const hMockTg = &state.mockTg.group;

	initMockTests();

	TG_setAfterTestAction(hMockTg, mockActionError);
	TG_runTests(hMockTg);

	const int ranTestActions = integersAreEqual(state.counters.testRun,NUM_MOCK_TESTS);

	const int ranAfterTestAction =
			integersAreEqual(state.counters.actionRun,NUM_MOCK_TESTS);

	const TestGroupOutcome expectedOutcome = {.run=NUM_MOCK_TESTS,.passed=0,.failed=0,.error=NUM_MOCK_TESTS};
	const int resultsAreAsExpected = outcomesAreEqual(&expectedOutcome, TG_getTestOutcome(hMockTg));


	if (!ranAfterTestAction)
		TG_fail(tg, "Failed to run action after each test (actions should run even if the test fails/errors)!");
	if (!ranTestActions)
		TG_fail(tg,"Failed to run test bodies (as there is no init step that can fail tests should allways run)!");
	if (!resultsAreAsExpected)
		TG_fail(tg, "Test outcomes are different than expected. If de-init declares an error the outcome can only be error!");
}

static TestDescriptor tests[]={
		{"Run action that succeeds before each test",test_BeforeTestActionSucceed},
		{"Run action that fails before each test",test_BeforeTestActionFail},
		{"Run action that encounters error before each test",test_BeforeTestActionError},
		{"Run action that succeeds after each test",test_AfterTestActionSucceed},
		{"Run action that fails after each test",test_AfterTestActionFail},
		{"Run action that declares an error after each test",test_AfterTestActionError}
};
static int numTests = sizeof(tests)/sizeof(*tests);



int testRun_TestGroup_beforeAndAfter(void)
{
	TestGroup * tg = &state.tg;
	TG_init(tg,"Test additional functions of TestGroup runner");
	TG_setTests(tg, tests, numTests);

	TG_runTests(tg);

	return TG_countErrors(tg)+TG_countFailed(tg);
}
