/*
 * test__test_group_adicional.c
 *
 *  Created on: 22 abr. 2021
 *      Author: Fernando
 */

#include <test.h>
#include <string.h> /*memset*/
#include <tgreporter_stub.h>
#include <assertions.h> /* TG_fail, TG_error */
#include <test_run.h>

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
	REPORT_FAIL(tg, "Before action failed","Mock action that always fails");
}

static void mockActionError(TestGroup *tg)
{
	countActionRun();
	REPORT_ERROR(tg, "Before action encountered an error","Mock action that simulates an error.");
}



static void mockTestPass(TestGroup *tg)
{
	countTestRun();
	(void)tg;
}

static void mockTestFail(TestGroup *tg)
{
	countTestRun();
	REPORT_FAIL(tg,"Test failed.","Mock test that fails.");
}

static void mockTestError(TestGroup *tg)
{
	countTestRun();
	REPORT_ERROR(tg,"Test encountered an error.","Mock test that simulates an error.");
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

static void setupMockTests(MockTestRunConfig *mtc)
{
    MTR_init(mtc);
    initMockTests();
    MTR_setTests(mtc,mockTests, NUM_MOCK_TESTS);
}

static inline int testRunsMatch(int expectedTestRuns)
{
    return state.counters.testRun == expectedTestRuns;
}
static inline int actionRunsMatch(int expectedActionRuns)
{
    return state.counters.actionRun == expectedActionRuns;
}
#define REPORT_UNEXPECTED_RESULT(tg,message,func,file,line) TG_fail(tg,message,"Una ejecuci??n de grupo de pruebas mock dio un resultado inesperado.",func,file,line)
#define RUN_AND_ASSERT(tg,mtc,actionRuns,testRuns,outcome) runAndAssert(tg,mtc,actionRuns,testRuns,outcome,__func__,__FILE__,__LINE__)
static inline void runAndAssert(TestGroup *tg, MockTestRunConfig *mtc, int actionRuns,int testRuns,const TestGroupOutcome *outcome,const char *func,const char *file, int line)
{
    const int outcomeAsExpected = MTR_runAndVerifyOutcome(mtc, outcome);

    if (!actionRunsMatch(actionRuns))
        REPORT_UNEXPECTED_RESULT(tg,"Las acciones no se ejecutaron en la forma programada.",func,file,line);
    if (!testRunsMatch(testRuns))
        REPORT_UNEXPECTED_RESULT(tg,"La ejecuci??n de pruebas fue distinta de los esperado.",func,file,line);
    if (!outcomeAsExpected)
        REPORT_UNEXPECTED_RESULT(tg,"Los resultados del grupo de pruebas no fueron los esperados.",func,file,line);

}

/* BEGIN TESTS DEFFINITIONS */

static void test_BeforeTestActionSucceed(TestGroup *tg)
{
	MockTestRunConfig mtc;
	setupMockTests(&mtc);

	MTR_setBeforeTestAction(&mtc, mockActionSucceed);

	const TestGroupOutcome expectedOutcome = {
	        .run=NUM_MOCK_TESTS,
            .passed=NUM_MOCK_PASS,
            .failed=NUM_MOCK_FAIL,
            .error=NUM_MOCK_ERROR};
	const int beforeActionRuns = NUM_MOCK_TESTS;
	const int testRuns = NUM_MOCK_TESTS;
	RUN_AND_ASSERT(tg, &mtc, beforeActionRuns, testRuns, &expectedOutcome);

}

static void test_BeforeTestActionFail(TestGroup *tg)
{
    MockTestRunConfig mtc;
    setupMockTests(&mtc);

    MTR_setBeforeTestAction(&mtc, mockActionFail);

    const TestGroupOutcome expectedOutcome = {
            .run=NUM_MOCK_TESTS,
            .passed=0,
            .failed=NUM_MOCK_TESTS,
            .error=0};

    const int beforeActionRuns = NUM_MOCK_TESTS;
    const int testRuns = 0;
    RUN_AND_ASSERT(tg, &mtc, beforeActionRuns, testRuns, &expectedOutcome);
}

static void test_BeforeTestActionError(TestGroup *tg)
{
    MockTestRunConfig mtc;
    setupMockTests(&mtc);

    MTR_setBeforeTestAction(&mtc, mockActionError);

    const TestGroupOutcome expectedOutcome = {
            .run=NUM_MOCK_TESTS,
            .passed=0,
            .failed=0,
            .error=NUM_MOCK_TESTS};

    const int beforeActionRuns = NUM_MOCK_TESTS;
    const int testRuns = 0;
    RUN_AND_ASSERT(tg, &mtc, beforeActionRuns, testRuns, &expectedOutcome);
}
static void test_AfterTestActionSucceed(TestGroup *tg)
{
    MockTestRunConfig mtc;
    setupMockTests(&mtc);

    MTR_setAfterTestAction(&mtc, mockActionSucceed);

    const TestGroupOutcome expectedOutcome = {
            .run=NUM_MOCK_TESTS,
            .passed=NUM_MOCK_PASS,
            .failed=NUM_MOCK_FAIL,
            .error=NUM_MOCK_ERROR};

    const int afterActionRuns = NUM_MOCK_TESTS;
    const int testRuns = NUM_MOCK_TESTS;
    RUN_AND_ASSERT(tg, &mtc, afterActionRuns, testRuns, &expectedOutcome);
}

static void test_AfterTestActionFail(TestGroup *tg)
{
    MockTestRunConfig mtc;
    setupMockTests(&mtc);

    MTR_setAfterTestAction(&mtc, mockActionFail);

    const TestGroupOutcome expectedOutcome = {
            .run=NUM_MOCK_TESTS,
            .passed=0,
            .failed=NUM_MOCK_TESTS-NUM_MOCK_ERROR,
            .error=NUM_MOCK_ERROR};

    const int afterActionRuns = NUM_MOCK_TESTS;
    const int testRuns = NUM_MOCK_TESTS;
    RUN_AND_ASSERT(tg, &mtc, afterActionRuns, testRuns, &expectedOutcome);
}

static void test_AfterTestActionError(TestGroup *tg)
{
    MockTestRunConfig mtc;
    setupMockTests(&mtc);

    MTR_setAfterTestAction(&mtc, mockActionError);

    const TestGroupOutcome expectedOutcome = {
            .run=NUM_MOCK_TESTS,
            .passed=0,
            .failed=0,
            .error=NUM_MOCK_TESTS};

    const int afterActionRuns = NUM_MOCK_TESTS;
    const int testRuns = NUM_MOCK_TESTS;
    RUN_AND_ASSERT(tg, &mtc, afterActionRuns, testRuns, &expectedOutcome);
}

static void test_BeforeGroupActionSucceed(TestGroup *tg)
{
    MockTestRunConfig mtc;
    setupMockTests(&mtc);

    MTR_setBeforeGroupAction(&mtc, mockActionSucceed);

    const TestGroupOutcome expectedOutcome = {
            .run=NUM_MOCK_TESTS,
            .passed=NUM_MOCK_PASS,
            .failed=NUM_MOCK_FAIL,
            .error=NUM_MOCK_ERROR};

    const int beforeActionRuns = 1;
    const int testRuns = NUM_MOCK_TESTS;
    RUN_AND_ASSERT(tg, &mtc, beforeActionRuns, testRuns, &expectedOutcome);
}

static void test_BeforeGroupActionFail(TestGroup *tg)
{
    MockTestRunConfig mtc;
    setupMockTests(&mtc);

    MTR_setBeforeGroupAction(&mtc, mockActionFail);

    const TestGroupOutcome expectedOutcome = {
            .run=0,
            .passed=0,
            .failed=1,
            .error=0};

    const int beforeActionRuns = 1;
    const int testRuns = 0;
    RUN_AND_ASSERT(tg, &mtc, beforeActionRuns, testRuns, &expectedOutcome);
}

static void test_BeforeGroupActionError(TestGroup *tg)
{
    MockTestRunConfig mtc;
    setupMockTests(&mtc);

    MTR_setBeforeGroupAction(&mtc, mockActionError);

    const TestGroupOutcome expectedOutcome = {.run=0,.passed=0,.failed=0,.error=1};

    const int beforeActionRuns = 1;
    const int testRuns = 0;
    RUN_AND_ASSERT(tg, &mtc, beforeActionRuns, testRuns, &expectedOutcome);
}

static void test_AfterGroupActionSuccess(TestGroup *tg)
{
    MockTestRunConfig mtc;
    setupMockTests(&mtc);

    MTR_setAfterGroupAction(&mtc, mockActionSucceed);

    const TestGroupOutcome expectedOutcome = {
            .run=NUM_MOCK_TESTS,
            .passed=NUM_MOCK_PASS,
            .failed=NUM_MOCK_FAIL,
            .error=NUM_MOCK_ERROR};

    const int afterActionRuns = 1;
    const int testRuns = NUM_MOCK_TESTS;
    RUN_AND_ASSERT(tg, &mtc, afterActionRuns, testRuns, &expectedOutcome);
}

static void test_AfterGroupActionFail(TestGroup *tg)
{
    MockTestRunConfig mtc;
    setupMockTests(&mtc);

    MTR_setAfterGroupAction(&mtc, mockActionFail);

    const TestGroupOutcome expectedOutcome = {
            .run=NUM_MOCK_TESTS,
            .passed=NUM_MOCK_PASS,
            .failed=NUM_MOCK_FAIL+1,
            .error=NUM_MOCK_ERROR};

    const int afterActionRuns = 1;
    const int testRuns = NUM_MOCK_TESTS;
    RUN_AND_ASSERT(tg, &mtc, afterActionRuns, testRuns, &expectedOutcome);
}

static void test_AfterGroupActionError(TestGroup *tg)
{
    MockTestRunConfig mtc;
    setupMockTests(&mtc);

    MTR_setAfterGroupAction(&mtc, mockActionError);

    const TestGroupOutcome expectedOutcome = {
            .run=NUM_MOCK_TESTS,
            .passed=NUM_MOCK_PASS,
            .failed=NUM_MOCK_FAIL,
            .error=NUM_MOCK_ERROR+1};

    const int afterActionRuns = 1;
    const int testRuns = NUM_MOCK_TESTS;
    RUN_AND_ASSERT(tg, &mtc, afterActionRuns, testRuns, &expectedOutcome);
}
static TestDescriptor tests[]={
		{"Ejecuta acci??n exitosa antes de cada prueba",test_BeforeTestActionSucceed},
		{"Ejecuta acci??n que falla antes de intentar cada prueba",test_BeforeTestActionFail},
		{"Ejecuta acci??n que declara un error antes de intentar cada prueba",test_BeforeTestActionError},
		{"Ejecuta acci??n exitosa despu??s de cada prueba",test_AfterTestActionSucceed},
		{"Ejecuta acci??n que falla despu??s de cada prueba",test_AfterTestActionFail},
		{"Ejecuta acci??n que declara un error despu??s de cada prueba",test_AfterTestActionError},
		{"Ejecuta acci??n exitosa antes del grupo de pruebas",test_BeforeGroupActionSucceed},
        {"Ejecuta acci??n que falla antes del grupo de pruebas",test_BeforeGroupActionFail},
        {"Ejecuta acci??n que declara un error antes del grupo de pruebas",test_BeforeGroupActionError},
        {"Ejecuta acci??n exitosa despu??s del grupo de pruebas",test_AfterGroupActionSuccess},
        {"Ejecuta acci??n que falla despu??s del grupo de pruebas",test_AfterGroupActionFail},
        {"Ejecuta acci??n que declara un error despu??s del grupo de pruebas",test_AfterGroupActionError},
};
static int numTests = sizeof(tests)/sizeof(*tests);



void testRun_TestGroup_beforeAndAfter(TestGroup *base)
{
	TestGroup * tg = &state.tg;
	TG_init(tg,"Pruebas de acciones antes y despu??s de grupos y de pruebas.");
	TG_setTests(tg, tests, numTests);
	TG_setReportPlugin(tg, SubgrupoReporter_comoTGReporter(reporterGrupo()));
	TG_runTests(tg);

	ASSERT_TRUE(base,"Pasaron las pruebas de acciones de antes y despu??s",
	        TG_allTestsPassed(tg));
}
