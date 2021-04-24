/*
 * test_assertions.c
 *
 *  Created on: 23 abr. 2021
 *      Author: Fernando
 */

#include <test.h>
#include <assertions.h>
#include <tgreporter_stub.h>


static void mockTest_assertTrue_true(TestGroup *tg)
{
	ASSERT_TRUE(tg,"This should pass",1);
}
static void mockTest_assertTrue_false(TestGroup *tg)
{
	ASSERT_TRUE(tg,"This should fail",0);
}
static void mockTest_assertFalse_true(TestGroup *tg)
{
	ASSERT_FALSE(tg,"This should fail",1);
}
static void mockTest_assertFalse_false(TestGroup *tg)
{
	ASSERT_FALSE(tg,"This should pass",0);
}

static inline int verifyExpecedResults(TestGroup *tg,int passed,int failed,int error)
{
	const TestGroupOutcome *const outcome = TG_getTestOutcome(tg);

	return outcome->passed == passed &&
		   outcome->failed == failed &&
		   outcome->error == error;
}

typedef enum {PASSED,FAILED,ERROR} ExpectedResult;

static inline int testResultsAsExpected(const char * description,TG_Test test,ExpectedResult expectation)
{
	TestGroup group;
	TestDescriptor testList[]={{description,test}};
	TG_init(&group,"Mock group");

	TG_setTests(&group,testList,1);
	TG_runTests(&group);

	const int passed = PASSED == expectation;
	const int failed = FAILED == expectation;
	const int error = ERROR == expectation;

	return verifyExpecedResults(&group, passed, failed, error);
}
static void test_assertTrue_true(TestGroup *tg)
{
	if (!testResultsAsExpected("assertTrue of true",
			mockTest_assertTrue_true, PASSED))
		TG_error(tg,"assertTrue of true must succeed");

}

static void test_assertTrue_false(TestGroup *tg)
{
	if (!testResultsAsExpected("assertTrue of false",
			mockTest_assertTrue_false, FAILED))
		TG_error(tg,"assertTrue of false must fail");

}

static void test_assertFalse_true(TestGroup *tg)
{
	if (!testResultsAsExpected("assertFalse of true",
			mockTest_assertFalse_true, FAILED))
		TG_error(tg,"assertFalse of true must fail");

}

static void test_assertFalse_false(TestGroup *tg)
{
	if (!testResultsAsExpected("assertFalse of false",
			mockTest_assertFalse_false, PASSED))
		TG_error(tg,"assertFalse of false must pass");

}

static TestDescriptor tests[]={
		{"# Asserting that something true is true passes\n",test_assertTrue_true},
		{"# Asserting that something false is true fails\n",test_assertTrue_false},
		{"# Asserting that something true is false fails\n",test_assertFalse_true},
		{"# Asserting that something false is false passes\n",test_assertFalse_false}
};

static int numTests = sizeof(tests)/sizeof(*tests);

int testRun_assertions(void)
{
	TestGroup tg;

	TG_init(&tg, "Tests for assertion mechanisms");
	TG_setTests(&tg,tests,numTests);
	TG_runTests(&tg);

	return !TG_allTestsPassed(&tg);
}
