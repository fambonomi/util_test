/*
 * test__test_group_nullStrings.c
 *
 *  Created on: 23 abr. 2021
 *      Author: Fernando
 */

#include <test.h>
#include <assertions.h>
#include <test_run.h>

static TGReporter *reporterSubgrupo;

static void stub_test(TestGroup *tg)
{
	(void)tg;
}

static inline void assertOutcomesAreEqual(TestGroup *tg,
		const char *message,
		const TestGroupOutcome *expected,
		const TestGroupOutcome *actual)
{
	const int outcomesAreEqual = expected->run == actual->run &&
								 expected->passed == actual->passed &&
								 expected->failed == actual->failed &&
								 expected->error == actual->error;

	if (!outcomesAreEqual)
		REPORT_FAIL(tg, message,"Mock tests outcome was different than expected.");
}
static void initAndRunTests(TestGroup *mockGroup,const char *name, TestDescriptor *tests,int numTests)
{
    TG_init(mockGroup, name);
    TG_setTests(mockGroup, tests, numTests);
    TG_setReportPlugin(mockGroup, reporterSubgrupo);
    TG_runTests(mockGroup);

}
static void test_unamedGroup(TestGroup *tg)
{
    TestGroup mockGroup;
	TestDescriptor tests[] ={{"named test",stub_test}};

	initAndRunTests(&mockGroup,NULL,tests,1);

	const TestGroupOutcome expectedOutcome ={.error=0,.failed=0,.passed=1,.run=1};
	const TestGroupOutcome *actualOutcome = TG_getTestOutcome(&mockGroup);
	assertOutcomesAreEqual(tg,"Un grupo con nombre nulo debe ejecutarse normalmente",
			&expectedOutcome, actualOutcome);
}
static void test_nullFP(TestGroup *tg)
{
    TestGroup mockGroup;
    TestDescriptor tests[] ={{"named test",(TG_Test)0}};

    initAndRunTests(&mockGroup, "Grupo con un puntero a función nulo", tests, 1);

    const TestGroupOutcome expectedOutcome ={.error=1,.failed=0,.passed=0,.run=1};
    const TestGroupOutcome *actualOutcome = TG_getTestOutcome(&mockGroup);
    assertOutcomesAreEqual(tg,"Una prueba con puntero a función nulo en su descriptor debe generar un error",
            &expectedOutcome, actualOutcome);
}
static void test_unnamedTest(TestGroup *tg)
{
    TestGroup mockGroup;
    TestDescriptor tests[] ={{NULL,stub_test}};

    initAndRunTests(&mockGroup, "Grupo con un puntero a nombre de prueba", tests, 1);

    const TestGroupOutcome expectedOutcome ={.error=0,.failed=0,.passed=1,.run=1};
    const TestGroupOutcome *actualOutcome = TG_getTestOutcome(&mockGroup);
    assertOutcomesAreEqual(tg,"Una prueba cuyo nombre es un puntero nulo en el descriptor debe ejecutarse normalmente",
            &expectedOutcome, actualOutcome);
}

static void test_nullDescriptor(TestGroup *tg)
{
    TestGroup mockGroup;
    TestDescriptor *tests =NULL;

    initAndRunTests(&mockGroup, "Grupo con puntero a descriptores de pruebas nulo", tests, 1);

    const TestGroupOutcome expectedOutcome ={.error=0,.failed=0,.passed=0,.run=0};
    const TestGroupOutcome *actualOutcome = TG_getTestOutcome(&mockGroup);
    assertOutcomesAreEqual(tg,"Si el puntero a descriptores de pruebas es nulo el grupo no debe ejecutarse",
            &expectedOutcome, actualOutcome);
}


void testRun_TestGroup_nullStrings(TestGroup *base)
{
	TestGroup group;
	static TestDescriptor tests[]={
			{"Grupo cuyo nombre es puntero nulo debe ejecutarse con normalidad",test_unamedGroup},
			{"Una prueba cuyo puntero a función de prueba es nulo debe generar un error",test_nullFP},
			{"Si el nombre de una prueba es un puntero nulo, no debe afectar su funcionamiento",test_unnamedTest},
			{"Si el puntero a descriptores de prueba es nulo el grupo no debe ejecutarse",test_nullDescriptor},
	};
	static int numTests = sizeof(tests)/sizeof(*tests);

	static SubgrupoReporter sbr;
	SubgrupoReporter_init_conBase(&sbr, reporterGrupo(), "  | ");
	reporterSubgrupo = SubgrupoReporter_comoTGReporter(&sbr);

	TG_init(&group,"Ensayo de punteros nulos en definición de grupo de pruebas");
	TG_setTests(&group, tests, numTests);
	TG_setReportPlugin(&group, SubgrupoReporter_comoTGReporter(reporterGrupo()));
	TG_runTests(&group);

	ASSERT_TRUE(base,"Pasaron las pruebas de punteros nulos",
	        TG_allTestsPassed(&group));
}
