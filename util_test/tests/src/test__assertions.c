/*
 * test_assertions.c
 *
 *  Created on: 23 abr. 2021
 *      Author: Fernando
 */

#include <test.h>
#include <assertions.h>
#include <tgreporter_stub.h>
#include <string.h> /*memcmp*/
#include <test_run.h>
#include <subgrupo_reporter.h>

static SubgrupoReporter reporterGrupoMock;

#define ESPERA_FALLA "Esta aserción debiera fallar"
#define ESPERA_PASA "Esta aserción debiera pasar"
#define ESPERA_ERROR "Al intentar esta aserción debe ocurrir un error"
#define INESPERADO "[<INCONSISTENCIA INTERNA>]"
static void mockTest_assertTrue_true(TestGroup *tg)
{
    ASSERT_TRUE(tg,ESPERA_PASA,1);
}
static void mockTest_assertTrue_false(TestGroup *tg)
{
    ASSERT_TRUE(tg,ESPERA_FALLA,0);
}
static void mockTest_assertFalse_true(TestGroup *tg)
{
    ASSERT_FALSE(tg,ESPERA_FALLA,1);
}
static void mockTest_assertFalse_false(TestGroup *tg)
{
    ASSERT_FALSE(tg,ESPERA_PASA,0);
}

static struct {
    struct {
        long long int A;
        long long int B;
    }integers;
    struct {
        const void *A;
        const void *B;
        size_t length;
    }memBlocks;
}mockTestParams;

static void mockTest_assertIntEqual(TestGroup *tg)
{
    const long long int A = mockTestParams.integers.A;
    const long long int B = mockTestParams.integers.B;

    ASSERT_INT_EQUAL(tg,((A==B)?ESPERA_PASA:ESPERA_FALLA),
                     A,B);
}

static void mockTest_assertIntNotEqual(TestGroup *tg)
{
    const long long int A = mockTestParams.integers.A;
    const long long int B = mockTestParams.integers.B;

    ASSERT_INT_NOT_EQUAL(tg,((A!=B)?ESPERA_PASA:ESPERA_FALLA),
                     A,B);
}

static void mockTest_assertMemoryEqual(TestGroup *tg)
{
    const void *const A = mockTestParams.memBlocks.A;
    const void *const B = mockTestParams.memBlocks.B;
    const size_t length = mockTestParams.memBlocks.length;
    int expectedOutcome = 3;
    static const char* outcomes[]={
            ESPERA_FALLA,
            ESPERA_PASA,
            ESPERA_ERROR,
            INESPERADO
    };

    if ((void*)0 == A ||
        (void*)0 == B)
        expectedOutcome = 2;
    else
        expectedOutcome = A==B || !memcmp(A,B,length);

    ASSERT_MEMORY_EQUAL(tg,outcomes[expectedOutcome&3],A,B,length);

}

static void mockTest_assertMemoryNotEqual(TestGroup *tg)
{
    const void *const A = mockTestParams.memBlocks.A;
    const void *const B = mockTestParams.memBlocks.B;
    const size_t length = mockTestParams.memBlocks.length;
    int expectedOutcome = 3;
    static const char* outcomes[]={
            ESPERA_PASA,
            ESPERA_FALLA,
            ESPERA_ERROR,
            INESPERADO
    };

    if ((void*)0 == A ||
        (void*)0 == B)
        expectedOutcome = 2;
    else
        expectedOutcome = A==B || !memcmp(A,B,length);

    ASSERT_MEMORY_NOT_EQUAL(tg,outcomes[expectedOutcome&3],A,B,length);

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
    TG_init(&group,"Grupo mock de una sola prueba");
    TG_setReportPlugin(&group, SubgrupoReporter_comoTGReporter(&reporterGrupoMock));
    TG_setTests(&group,testList,1);
    TG_runTests(&group);

    const int passed = PASSED == expectation;
    const int failed = FAILED == expectation;
    const int error = ERROR == expectation;

    return verifyExpecedResults(&group, passed, failed, error);
}
static void mockTestResultUnexpected(TestGroup *tg,const char* message)
{
    REPORT_ERROR(tg,message,"Prueba mock genera resultado distinto al esperado.");

}
#define MSG_ASSERT_BOOL(strAsercion,strPredicado) "ASSERT_"strAsercion" de un predicado "strPredicado
static void test_assertTrue_true(TestGroup *tg)
{
    if (!testResultsAsExpected(MSG_ASSERT_BOOL("TRUE","verdadero"),
            mockTest_assertTrue_true, PASSED))
        mockTestResultUnexpected(tg, ESPERA_PASA);
}

static void test_assertTrue_false(TestGroup *tg)
{
    if (!testResultsAsExpected(MSG_ASSERT_BOOL("TRUE","falso"),
            mockTest_assertTrue_false, FAILED))
        mockTestResultUnexpected(tg,ESPERA_FALLA);

}

static void test_assertFalse_true(TestGroup *tg)
{
    if (!testResultsAsExpected(MSG_ASSERT_BOOL("FALSE","verdadero"),
            mockTest_assertFalse_true, FAILED))
        mockTestResultUnexpected(tg,ESPERA_FALLA);

}

static void test_assertFalse_false(TestGroup *tg)
{
    if (!testResultsAsExpected(MSG_ASSERT_BOOL("FALSE","falso"),
            mockTest_assertFalse_false, PASSED))
        mockTestResultUnexpected(tg,ESPERA_PASA);

}

#define MSG_ASSERT_INT(predicado,relacion) "ASSERT_INT_"predicado" de un par de enteros "relacion
static void test_assertIntEqual_pass(TestGroup *tg)
{
    mockTestParams.integers.A=0x0807060504030201LL;
    mockTestParams.integers.B=mockTestParams.integers.A;
    if (!testResultsAsExpected(MSG_ASSERT_INT("EQUAL","iguales"),
            mockTest_assertIntEqual, PASSED))
        mockTestResultUnexpected(tg,ESPERA_PASA);
}

static void test_assertIntEqual_fail(TestGroup *tg)
{
    mockTestParams.integers.A=0x0807060504030201LL;
    mockTestParams.integers.B=-mockTestParams.integers.A;
    if (!testResultsAsExpected(MSG_ASSERT_INT("EQUAL","distintos"),
            mockTest_assertIntEqual, FAILED))
        mockTestResultUnexpected(tg,ESPERA_FALLA);
}

static void test_assertIntNotEqual_fail(TestGroup *tg)
{
    mockTestParams.integers.A=0x5a5a5a5aa5a5a5a5LL;
    mockTestParams.integers.B=mockTestParams.integers.A;
    if (!testResultsAsExpected(MSG_ASSERT_INT("NOT_EQUAL","iguales"),
            mockTest_assertIntNotEqual, FAILED))
        mockTestResultUnexpected(tg,ESPERA_FALLA);
}

static void test_assertIntNotEqual_pass(TestGroup *tg)
{
    mockTestParams.integers.A=0x5a5a5a5aa5a5a5a5LL;
    mockTestParams.integers.B=0x0f0f0f0ff0f0f0f0LL;
    if (!testResultsAsExpected(MSG_ASSERT_INT("NOT_EQUAL","distintos"),
            mockTest_assertIntNotEqual, PASSED))
        mockTestResultUnexpected(tg,ESPERA_PASA);
}
#define MSG_ASSERT_MEMORY(predicado,relacion) "ASSERT_MEMORY_"predicado" de un par de bloques de memoria "relacion
static void test_assertMemoryEqual_pass(TestGroup *tg)
{
    char mem1[]="This is a sentence.";
    char mem2[]="This is a sentence.";
    mockTestParams.memBlocks.A=mem1;
    mockTestParams.memBlocks.B=mem2;
    mockTestParams.memBlocks.length=sizeof(mem1);

    if(!testResultsAsExpected(MSG_ASSERT_MEMORY("EQUAL"," de igual contenido"),
            mockTest_assertMemoryEqual, PASSED))
        mockTestResultUnexpected(tg, ESPERA_PASA);
}

static void test_assertMemoryEqual_fail(TestGroup *tg)
{
    char mem1[]="This is a sentence. ";
    char mem2[]="This is another one.";
    mockTestParams.memBlocks.A=mem1;
    mockTestParams.memBlocks.B=mem2;
    mockTestParams.memBlocks.length=sizeof(mem1);

    if(!testResultsAsExpected(MSG_ASSERT_MEMORY("EQUAL"," con contenidos diferentes"),
            mockTest_assertMemoryEqual, FAILED))
        mockTestResultUnexpected(tg, ESPERA_FALLA);
}

static void test_assertMemoryEqual_error(TestGroup *tg)
{
    mockTestParams.memBlocks.A=(const void*) 0;
    mockTestParams.memBlocks.B=(const void*) 0;
    mockTestParams.memBlocks.length=0;

    if(!testResultsAsExpected(MSG_ASSERT_MEMORY("EQUAL"," con al menos un puntero nulo"),
            mockTest_assertMemoryEqual, ERROR))
        mockTestResultUnexpected(tg, ESPERA_ERROR);
}

static void test_assertMemoryNotEqual_fail(TestGroup *tg)
{
    char mem1[]="This is a sentence.";
    char mem2[]="This is a sentence.";
    mockTestParams.memBlocks.A=mem1;
    mockTestParams.memBlocks.B=mem2;
    mockTestParams.memBlocks.length=sizeof(mem1);

    if(!testResultsAsExpected(MSG_ASSERT_MEMORY("NOT_EQUAL"," de igual contenido"),
            mockTest_assertMemoryNotEqual, FAILED))
        mockTestResultUnexpected(tg, ESPERA_FALLA);
}

static void test_assertMemoryNotEqual_pass(TestGroup *tg)
{
    char mem1[]="This is a sentence. ";
    char mem2[]="This is another one.";
    mockTestParams.memBlocks.A=mem1;
    mockTestParams.memBlocks.B=mem2;
    mockTestParams.memBlocks.length=sizeof(mem1);

    if(!testResultsAsExpected(MSG_ASSERT_MEMORY("NOT_EQUAL"," con contenidos diferentes"),
            mockTest_assertMemoryNotEqual, PASSED))
        mockTestResultUnexpected(tg, ESPERA_PASA);
}

static void test_assertMemoryNotEqual_error(TestGroup *tg)
{
    mockTestParams.memBlocks.A=(const void*) 0;
    mockTestParams.memBlocks.B=(const void*) 0;
    mockTestParams.memBlocks.length=0;

    if(!testResultsAsExpected(MSG_ASSERT_MEMORY("NOT_EQUAL"," con al menos un puntero nulo"),
            mockTest_assertMemoryNotEqual, ERROR))
        mockTestResultUnexpected(tg, ESPERA_ERROR);
}

static TestDescriptor tests[]={
        {"# Aserción que un predicado verdadero es verdadero pasa",test_assertTrue_true},
        {"# Aserción que un predicado falso es verdadero falla",test_assertTrue_false},
        {"# Aserción que un predicado verdadero es falso falla",test_assertFalse_true},
        {"# Aserción que un predicado falso es falso pasa",test_assertFalse_false},
        {"# Aserción que números iguales son iguales pasa",test_assertIntEqual_pass},
        {"# Aserción que números diferentes son iguales falla",test_assertIntEqual_fail},
        {"# Aserción que números iguales son distintos falla",test_assertIntNotEqual_fail},
        {"# Aserción que números distintos son distintos pasa",test_assertIntNotEqual_pass},
        {"# Aserción que dos bloques de memoria de igual contenido son iguales pasa",
                test_assertMemoryEqual_pass},
        {"# Aserción que dos bloques de memoria con distintos contenidos son iguales falla",
                test_assertMemoryEqual_fail},
        {"# Aserción que dos bloques de memoria descritos por punteros nulos son iguales declara un error",
                test_assertMemoryEqual_error},
        {"# Aserción que dos bloques de memoria de igual contenido son distintos falla",
                test_assertMemoryNotEqual_pass},
        {"# Aserción que dos bloques de memoria con distintos contenidos son distintos pasa",
                test_assertMemoryNotEqual_fail},
        {"# Aserción que dos bloques de memoria descritos por punteros nulos son distintos declara un error",
                test_assertMemoryNotEqual_error},
};

static int numTests = sizeof(tests)/sizeof(*tests);


void testRun_assertions(TestGroup *base)
{
    TestGroup tg;
    SubgrupoReporter *const reporter=reporterGrupo();

    SubgrupoReporter_init_conBase(&reporterGrupoMock,reporter, "  | ");

    TG_init(&tg, "Pruebas para los mecanismos de asercíon");
    TG_setReportPlugin(&tg, SubgrupoReporter_comoTGReporter(reporter));
    TG_setTests(&tg,tests,numTests);
    TG_runTests(&tg);

    ASSERT_TRUE(base,"Las pruebas de los mecanismos de aserción fueron exitosas",
            TG_allTestsPassed(&tg));
}
