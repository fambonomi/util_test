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
	state.counters.actionRun=0;
	state.counters.testRun=0;
}

typedef struct MockTestRunConfig{
    const char* name;
    TG_Before beforeGroupAction;
    TG_Before beforeTestAction;
    TG_After afterTestAction;
    TG_After afterGroupAction;
    TestDescriptor *tests;
    int numTests;
}MockTestRunConfig;

static inline void MTR_init(MockTestRunConfig *cfg)
{
    memset(cfg,0,sizeof(*cfg));
    cfg->name="Mock test group";
}
static inline void MTR_setGroupName(MockTestRunConfig *cfg,const char* name)
{
    cfg->name = name;
}
static inline void MTR_setBeforeGroupAction(MockTestRunConfig *cfg, TG_Before action)
{
    cfg->beforeGroupAction=action;
}
static inline void MTR_setBeforeTestAction(MockTestRunConfig *cfg, TG_Before action)
{
    cfg->beforeTestAction=action;
}
static inline void MTR_setAfterTestAction(MockTestRunConfig *cfg, TG_After action)
{
    cfg->afterTestAction=action;
}
static inline void MTR_setAfterGroupAction(MockTestRunConfig *cfg, TG_After action)
{
    cfg->afterGroupAction=action;
}
static inline void MTR_setTests(MockTestRunConfig *cfg, TestDescriptor *tests,int numTests)
{
    cfg->tests = tests;
    cfg->numTests = numTests;
}

static inline void MTR__prepareMockTestRun(TestGroup *group, const MockTestRunConfig *cfg)
{
    TG_init(group,cfg->name);
    TG_setBeforeGroupAction(group, cfg->beforeGroupAction);
    TG_setBeforeTestAction(group, cfg->beforeTestAction);
    TG_setTests(group,cfg->tests,cfg->numTests);
    TG_setAfterTestAction(group, cfg->afterTestAction);
    TG_setAfterGroupAction(group, cfg->afterGroupAction);
    TG_setReportPlugin(group, TGReporter_stub_getInstance());
}
static inline int MTR__verifyTestOutcome(const TestGroupOutcome *actual,
                                         const TestGroupOutcome *expected)
{
    return actual->run == expected->run &&
           actual->passed == expected->passed &&
           actual->failed == expected->failed &&
           actual->error == expected->error;

}
static inline int MTR_runAndVerifyOutcome(const MockTestRunConfig *cfg, const TestGroupOutcome *expected)
{
    TestGroup group;

    MTR__prepareMockTestRun(&group, cfg);
    TG_runTests(&group);

    const TestGroupOutcome *actualOutcome = TG_getTestOutcome(&group);

    return MTR__verifyTestOutcome(actualOutcome, expected);
}

static void test_BeforeTestActionSucceed(TestGroup *tg)
{
	MockTestRunConfig mtc;

	MTR_init(&mtc);

	initMockTests();

	MTR_setBeforeTestAction(&mtc, mockActionSucceed);
	MTR_setTests(&mtc,mockTests, NUM_MOCK_TESTS);

	const TestGroupOutcome expectedOutcome = {.run=NUM_MOCK_TESTS,.passed=NUM_MOCK_PASS,.failed=NUM_MOCK_FAIL,.error=NUM_MOCK_ERROR};

    const int resultsAreAsExpected = MTR_runAndVerifyOutcome(&mtc, &expectedOutcome);

	const int ranTests = integersAreEqual(state.counters.testRun,NUM_MOCK_TESTS);

	const int ranBeforeTestAction =
			integersAreEqual(state.counters.actionRun,NUM_MOCK_TESTS);


	if (!ranBeforeTestAction)
		TG_fail(tg,"Failed to run action before all the tests!");
	if (!ranTests)
		TG_fail(tg,"Failed to run all the tests!");
	if (!resultsAreAsExpected)
		TG_fail(tg,"Test outcomes are different than expected!");

}

static void test_BeforeTestActionFail(TestGroup *tg)
{
    MockTestRunConfig mtc;

    MTR_init(&mtc);

    initMockTests();

    MTR_setBeforeTestAction(&mtc, mockActionFail);
    MTR_setTests(&mtc,mockTests, NUM_MOCK_TESTS);

    const TestGroupOutcome expectedOutcome = {.run=NUM_MOCK_TESTS,.passed=0,.failed=NUM_MOCK_TESTS,.error=0};
    const int resultsAreAsExpected = MTR_runAndVerifyOutcome(&mtc, &expectedOutcome);

	const int didntRunTestActions = integersAreEqual(state.counters.testRun,0);

	const int ranBeforeTestAction =
			integersAreEqual(state.counters.actionRun,NUM_MOCK_TESTS);


	if (!ranBeforeTestAction)
		TG_fail(tg,"Failed to run action before all the tests!");
	if (!didntRunTestActions)
		TG_fail(tg,"Ran test actions when beforeTest action failed!");
	if (!resultsAreAsExpected)
		TG_fail(tg,"Test outcomes are different than expected. If init declares a fail test fails!");

}

static void test_BeforeTestActionError(TestGroup *tg)
{
    MockTestRunConfig mtc;

    MTR_init(&mtc);

    initMockTests();

    MTR_setBeforeTestAction(&mtc, mockActionError);
    MTR_setTests(&mtc,mockTests, NUM_MOCK_TESTS);

    const TestGroupOutcome expectedOutcome = {.run=NUM_MOCK_TESTS,.passed=0,.failed=0,.error=NUM_MOCK_TESTS};
    const int resultsAreAsExpected = MTR_runAndVerifyOutcome(&mtc, &expectedOutcome);

    const int didntRunTestActions = integersAreEqual(state.counters.testRun,0);

    const int ranBeforeTestAction =
            integersAreEqual(state.counters.actionRun,NUM_MOCK_TESTS);


	if (!ranBeforeTestAction)
		TG_fail(tg,"Failed to run action before each test!");
	if (!didntRunTestActions)
		TG_fail(tg,"Ran test actions when beforeTest action encountered error!");
	if (!resultsAreAsExpected)
		TG_fail(tg,"Test outcomes are different than expected!");

}
static void test_AfterTestActionSucceed(TestGroup *tg)
{
    MockTestRunConfig mtc;

    MTR_init(&mtc);

    initMockTests();

    MTR_setAfterTestAction(&mtc, mockActionSucceed);
    MTR_setTests(&mtc,mockTests, NUM_MOCK_TESTS);

    const TestGroupOutcome expectedOutcome = {.run=NUM_MOCK_TESTS,.passed=NUM_MOCK_PASS,.failed=NUM_MOCK_FAIL,.error=NUM_MOCK_ERROR};
    const int resultsAreAsExpected = MTR_runAndVerifyOutcome(&mtc, &expectedOutcome);

	const int ranTestActions = integersAreEqual(state.counters.testRun,NUM_MOCK_TESTS);

	const int ranAfterTestAction =
			integersAreEqual(state.counters.actionRun,NUM_MOCK_TESTS);

	if (!ranAfterTestAction)
		TG_fail(tg, "Failed to run action after each test (actions should run even if the test fails/errors)!");
	if (!ranTestActions)
		TG_fail(tg,"Failed to run test bodies!");
	if (!resultsAreAsExpected)
		TG_fail(tg, "Test outcomes are different than expected!");
}

static void test_AfterTestActionFail(TestGroup *tg)
{
    MockTestRunConfig mtc;

    MTR_init(&mtc);

    initMockTests();

    MTR_setAfterTestAction(&mtc, mockActionFail);
    MTR_setTests(&mtc,mockTests, NUM_MOCK_TESTS);

    const TestGroupOutcome expectedOutcome = {.run=NUM_MOCK_TESTS,.passed=0,.failed=NUM_MOCK_TESTS-NUM_MOCK_ERROR,.error=NUM_MOCK_ERROR};
    const int resultsAreAsExpected = MTR_runAndVerifyOutcome(&mtc, &expectedOutcome);

    const int ranTestActions = integersAreEqual(state.counters.testRun,NUM_MOCK_TESTS);

	const int ranAfterTestAction =
			integersAreEqual(state.counters.actionRun,NUM_MOCK_TESTS);


	if (!ranAfterTestAction)
		TG_fail(tg, "Failed to run action after each test (actions should run even if the test fails/errors)!");
	if (!ranTestActions)
		TG_fail(tg,"Failed to run test bodies (as there is no init step that can fail tests should allways run)!");
	if (!resultsAreAsExpected)
		TG_fail(tg, "Test outcomes are different than expected. If de-init declares a fail, the outcome can be either fail or error (if the test declared an error)!");
}

static void test_AfterTestActionError(TestGroup *tg)
{
    MockTestRunConfig mtc;

    MTR_init(&mtc);

    initMockTests();

    MTR_setAfterTestAction(&mtc, mockActionError);
    MTR_setTests(&mtc,mockTests, NUM_MOCK_TESTS);

    const TestGroupOutcome expectedOutcome = {.run=NUM_MOCK_TESTS,.passed=0,.failed=0,.error=NUM_MOCK_TESTS};
    const int resultsAreAsExpected = MTR_runAndVerifyOutcome(&mtc, &expectedOutcome);

	const int ranTestActions = integersAreEqual(state.counters.testRun,NUM_MOCK_TESTS);

	const int ranAfterTestAction =
			integersAreEqual(state.counters.actionRun,NUM_MOCK_TESTS);

	if (!ranAfterTestAction)
		TG_fail(tg, "Failed to run action after each test (actions should run even if the test fails/errors)!");
	if (!ranTestActions)
		TG_fail(tg,"Failed to run test bodies (as there is no init step that can fail tests should allways run)!");
	if (!resultsAreAsExpected)
		TG_fail(tg, "Test outcomes are different than expected. If de-init declares an error the outcome can only be error!");
}

static void test_BeforeGroupActionSucceed(TestGroup *tg)
{
    MockTestRunConfig mtc;

    MTR_init(&mtc);

    initMockTests();

    MTR_setBeforeGroupAction(&mtc, mockActionSucceed);
    MTR_setTests(&mtc,mockTests, NUM_MOCK_TESTS);

    const TestGroupOutcome expectedOutcome = {.run=NUM_MOCK_TESTS,.passed=NUM_MOCK_PASS,.failed=NUM_MOCK_FAIL,.error=NUM_MOCK_ERROR};
    const int resultsAreAsExpected = MTR_runAndVerifyOutcome(&mtc, &expectedOutcome);

    const int ranTestActions = integersAreEqual(state.counters.testRun,NUM_MOCK_TESTS);

    const int ranBeforeGroupAction =
            integersAreEqual(state.counters.actionRun,1);

    if (!ranBeforeGroupAction)
        TG_fail(tg, "Failed to run action before test group!");
    if (!ranTestActions)
        TG_fail(tg,"Failed to run test bodies (before group action succeeds so they should run)!");
    if (!resultsAreAsExpected)
        TG_fail(tg, "Test outcomes are different than expected.");
}

static void test_BeforeGroupActionFail(TestGroup *tg)
{
    MockTestRunConfig mtc;

    MTR_init(&mtc);

    initMockTests();

    MTR_setBeforeGroupAction(&mtc, mockActionFail);
    MTR_setTests(&mtc,mockTests, NUM_MOCK_TESTS);

    const TestGroupOutcome expectedOutcome = {.run=0,.passed=0,.failed=1,.error=0};
    const int resultsAreAsExpected = MTR_runAndVerifyOutcome(&mtc, &expectedOutcome);

    const int didntRunTestActions = integersAreEqual(state.counters.testRun,0);

    const int ranBeforeGroupAction =
            integersAreEqual(state.counters.actionRun,1);

    if (!ranBeforeGroupAction)
        TG_fail(tg, "Failed to run action before test group!");
    if (!didntRunTestActions)
        TG_fail(tg,"Ran tests after failing the before group action. Tests shouldn't be run in this case because the environment could be compromised.");
    if (!resultsAreAsExpected)
        TG_fail(tg, "Test outcomes are different than expected (should be 0 run and 1 failed).");
}

static void test_BeforeGroupActionError(TestGroup *tg)
{
    MockTestRunConfig mtc;

    MTR_init(&mtc);

    initMockTests();

    MTR_setBeforeGroupAction(&mtc, mockActionError);
    MTR_setTests(&mtc,mockTests, NUM_MOCK_TESTS);

    const TestGroupOutcome expectedOutcome = {.run=0,.passed=0,.failed=0,.error=1};
    const int resultsAreAsExpected = MTR_runAndVerifyOutcome(&mtc, &expectedOutcome);

    const int didntRunTestActions = integersAreEqual(state.counters.testRun,0);

    const int ranBeforeGroupAction =
            integersAreEqual(state.counters.actionRun,1);

    if (!ranBeforeGroupAction)
        TG_fail(tg, "Failed to run action before test group!");
    if (!didntRunTestActions)
        TG_fail(tg,"Ran tests after an error in the before group action. Tests shouldn't be run in this case because the environment could be compromised.");
    if (!resultsAreAsExpected)
        TG_fail(tg, "Test outcomes are different than expected (should be 0 run and 1 error).");
}

static TestDescriptor tests[]={
		{"Run action that succeeds before each test",test_BeforeTestActionSucceed},
		{"Run action that fails before each test",test_BeforeTestActionFail},
		{"Run action that encounters error before each test",test_BeforeTestActionError},
		{"Run action that succeeds after each test",test_AfterTestActionSucceed},
		{"Run action that fails after each test",test_AfterTestActionFail},
		{"Run action that declares an error after each test",test_AfterTestActionError},
		{"Run action that succeeds before the test group",test_BeforeGroupActionSucceed},
        {"Run action that fails before the test group",test_BeforeGroupActionFail},
        {"Run action that declares an error before the test group",test_BeforeGroupActionError},
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
