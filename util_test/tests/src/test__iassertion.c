/*
 * test__assertion.c
 *
 *  Created on: 26 abr. 2021
 *      Author: Fernando
 */
#include <test.h>
#include <iassertion_impl.h>
#include <assertions.h>
#include <stdint.h> /*intptr_t*/


static inline void empty_assertion_consistency(
        TestGroup *tg,const TestAssertion *assertion,TAKind initKind)
{
    const intptr_t nullPtr = (intptr_t)NULL;
    ASSERT_INT_NOT_EQUAL(tg,"After initialization message should be a valid string",
            nullPtr,(intptr_t) TA_getMessage(assertion));
    ASSERT_INT_NOT_EQUAL(tg,"After initialization reason should be a valid string",
            nullPtr,(intptr_t) TA_getReason(assertion));
    ASSERT_INT_NOT_EQUAL(tg,"After initialization file should be a valid string",
            nullPtr,(intptr_t)TA_getFile(assertion));
    ASSERT_INT_NOT_EQUAL(tg,"After initialization func should be a valid string",
            nullPtr,(intptr_t)TA_getFunc(assertion));
    ASSERT_INT_EQUAL(tg,"After initialization kind should be as specified in init",
            initKind,TA_getKind(assertion));
    ASSERT_INT_EQUAL(tg,"After initialization result should be Pass",
            TA_PASS,TA_getResult(assertion));
}
static inline void test_empty_assertion_consistency(TestGroup *tg,TestAssertion *assertion,TAKind kind)
{
    TA_init(assertion, kind);
    empty_assertion_consistency(tg, assertion, kind);
}

static void test_empty_bool_assertion(TestGroup *tg)
{
    TestAssertion assertion={0};
    test_empty_assertion_consistency(tg,&assertion, TAK_BOOL);
}

static void test_empty_num_assertion(TestGroup *tg)
{
    TestAssertion assertion={0};
    test_empty_assertion_consistency(tg,&assertion, TAK_NUM);
}

static void test_empty_mem_assertion(TestGroup *tg)
{
    TestAssertion assertion={.detail={.mem_kind={.A=(void *)0x5a5a5a5a,.B=(void *)0x96969696}}};
    test_empty_assertion_consistency(tg,&assertion, TAK_MEM);
    const intptr_t nullPtr = (intptr_t) NULL;

    ASSERT_INT_EQUAL(tg,"After initialization A should be a null pointer",
            nullPtr,(intptr_t)TA_mem_getPA(&assertion));
    ASSERT_INT_EQUAL(tg,"After initialization B should be a null pointer",
            nullPtr,(intptr_t)TA_mem_getPB(&assertion));
    ASSERT_INT_EQUAL(tg,"After initialization length should be 0",
            0,TA_mem_getLength(&assertion));
}

static inline void test__result(TestGroup *tg,TAResult result)
{
    TestAssertion assertion;
    const int validResult = result >= 0 && result <= TA_MAX;

    ASSERT_TRUE(tg,"Improper usage of test__result!",validResult);

    TA_init(&assertion, TAK_BOOL);
    TA_setResult(&assertion, result);

    ASSERT_INT_EQUAL(tg,"TA_getResult (A) should give the result passed to TA_setResult (B)",
            TA_getResult(&assertion),result);
}
static inline void test__invalidResult(TestGroup *tg,TAResult result)
{
    TestAssertion assertion;
    const int validResult = result >= 0 && result <= TA_MAX;

    ASSERT_FALSE(tg,"Improper usage of test__invalidResult!",validResult);

    TA_init(&assertion, TAK_BOOL);
    TA_setResult(&assertion, result);

    ASSERT_INT_NOT_EQUAL(tg,"TA_getResult (A) shouldnt give the INVALID result passed to TA_setResult (B)",
            TA_getResult(&assertion),result);

}

static void test_assertionResult(TestGroup *tg)
{
    test__result(tg,TA_PASS);
    test__result(tg,TA_FAIL);
    test__result(tg,TA_ERROR);
}

static void test_assertionInvalidResult(TestGroup *tg)
{
    test__invalidResult(tg,-1);
    test__invalidResult(tg,TA_MAX+1);
}

static void test_positionParameters(TestGroup *tg)
{
    const char *const file = __FILE__;
    const int line = __LINE__;
    const char *const func = __func__;

    TestAssertion assertion;
    TA_init(&assertion,TAK_BOOL);

    TA_setFile(&assertion,file);
    TA_setLine(&assertion,line);
    TA_setFunc(&assertion,func);

    ASSERT_INT_EQUAL(tg,"A=TA_getFile() should be the NON-NULL pointer given to TA_setFile(B)",
            (intptr_t)TA_getFile(&assertion),(intptr_t)file);
    ASSERT_INT_EQUAL(tg,"A=TA_getFunc() should be the NON-NULL pointer given to TA_setFunc(B)",
            (intptr_t)TA_getFunc(&assertion),(intptr_t)func);
    ASSERT_INT_EQUAL(tg,"A=TA_getLine() should be the number given to TA_setLine(B)",
            TA_getLine(&assertion),line);

}

static void test_invalidPositionParameters(TestGroup *tg)
{
    const char *const file = NULL;
    const int line = -1;
    const char *const func = NULL;

    TestAssertion assertion;
    TA_init(&assertion,TAK_BOOL);

    TA_setFile(&assertion,file);
    TA_setLine(&assertion,line);
    TA_setFunc(&assertion,func);

    ASSERT_INT_NOT_EQUAL(tg,"A=TA_getFile() should be the default value if a NULL pointer is given to TA_setFile(B)",
            (intptr_t)TA_getFile(&assertion),(intptr_t)file);
    ASSERT_INT_NOT_EQUAL(tg,"A=TA_getFunc() should be the default value if a NULL pointer is given to TA_setFunc(B)",
            (intptr_t)TA_getFunc(&assertion),(intptr_t)func);
    ASSERT_INT_NOT_EQUAL(tg,"A=TA_getLine() should be 0 if a negative number given to TA_setLine(B)",
            TA_getLine(&assertion),line);

}



static TestDescriptor tests[]=
{
        {"Bool assertion struct state after initialization",test_empty_bool_assertion},
        {"Num assertion struct state after initialization",test_empty_num_assertion},
        {"Mem assertion struct state after initialization",test_empty_mem_assertion},
        {"TA_getResult returns the VALID result given to TA_setResult",test_assertionResult},
        {"TA_getResult doesn't return the INVALID result given to TA_setResult",test_assertionInvalidResult},
        {"Correct position parameters are returned as they were set",test_positionParameters},
        {"Invalid position parameters are not set, default values are set instead",test_invalidPositionParameters},

};
static const int numTests = sizeof(tests)/sizeof(*tests);
/**
 * public interface
 */

int testRun_assertionInterface(void)
{
    static TestGroup tg;
    TG_init(&tg, "Assertion interface tests");
    TG_setTests(&tg, tests, numTests);

    TG_runTests(&tg);

    return !TG_allTestsPassed(&tg);
}
