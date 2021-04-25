/*
 * test__test_group_nullStrings.c
 *
 *  Created on: 23 abr. 2021
 *      Author: Fernando
 */

#include <test.h>


static void stub_test(TestGroup *tg)
{
	(void)tg;
}

static inline void assertOutcomesAreEqual(TestGroup *tg,
		char *message,
		const TestGroupOutcome *expected,
		const TestGroupOutcome *actual)
{
	const int outcomesAreEqual = expected->run == actual->run &&
								 expected->passed == actual->passed &&
								 expected->failed == actual->failed &&
								 expected->error == actual->error;

	if (!outcomesAreEqual)
		TG_fail(tg, message);
}
static void test_unamedGroup(TestGroup *tg)
{
	TestGroup mockGroup;
	TestDescriptor tests[] ={{"named test",stub_test}};

	TG_init(&mockGroup, NULL);
	TG_setTests(&mockGroup, tests, 1);
	TG_runTests(&mockGroup);

	const TestGroupOutcome expectedOutcome ={.error=0,.failed=0,.passed=1,.run=1};
	const TestGroupOutcome *actualOutcome = TG_getTestOutcome(&mockGroup);
	assertOutcomesAreEqual(tg,"Test group with null name didn't run as expected!",
			&expectedOutcome, actualOutcome);
}


int testRun_TestGroup_nullStrings(void)
{
	TestGroup group;
	static TestDescriptor tests[]={
			{"Group with null name shall run normally.",test_unamedGroup}
	};
	static int numTests = sizeof(tests)/sizeof(*tests);
	TG_init(&group,NULL);
	TG_setTests(&group, tests, numTests);

	TG_runTests(&group);

	// a return of zero means all tests passed
	return !TG_allTestsPassed(&group);
}