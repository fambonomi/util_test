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

static inline void boolAssertion_testOperator(TestGroup *tg, TestAssertion *ta, TAOperator op)
{
    TA_bool_setOperator(ta,op);
    ASSERT_INT_EQUAL(tg,"Operator set (A) should coincide with operator gotten (B) in boolean assert details.",
            op,TA_bool_getOperator(ta));
}
static inline void boolAssertion_testOperand(TestGroup *tg, TestAssertion *ta, int A)
{
    TA_bool_setA(ta,A);
    ASSERT_INT_EQUAL(tg,"Operand set (A) should coincide with operand gotten (B) in boolean assert details.",
            A,TA_bool_getA(ta));

}
static void test_boolAssertionOperator(TestGroup *tg)
{
    TestAssertion assertion={0};
    TA_init(&assertion,TAK_BOOL);

    boolAssertion_testOperator(tg,&assertion,TAO_IS_FALSE);
    boolAssertion_testOperator(tg,&assertion,TAO_IS_TRUE);
}
static void test_boolAssertionOperand(TestGroup *tg)
{
    TestAssertion assertion={0};
    TA_init(&assertion,TAK_BOOL);

    boolAssertion_testOperand(tg,&assertion,0);
    boolAssertion_testOperand(tg,&assertion,1);
}

static inline void numAssertion_testOperator(TestGroup *tg,TestAssertion *ta, TAOperator op)
{
    TA_num_setOperator(ta,op);
    ASSERT_INT_EQUAL(tg,"Operator set (A) should coincide with operator gotten (B) in numeric assert details.",
            op,TA_num_getOperator(ta));
}

static inline void numAssertion_testOperand(TestGroup *tg, TestAssertion *ta, long long int A, long long int B)
{
    TA_num_setA(ta,A);
    TA_num_setB(ta,B);
    ASSERT_INT_EQUAL(tg,"First operand value set (A) should coincide with value gotten (B) in numeric assert details.",
            A,TA_num_getA(ta));
    ASSERT_INT_EQUAL(tg,"Second operand value set (A) should coincide with value gotten (B) in numeric assert details.",
            B,TA_num_getB(ta));

}
static void test_numAssertionOperator(TestGroup *tg)
{
    TestAssertion assertion={0};
    TA_init(&assertion,TAK_NUM);

    numAssertion_testOperator(tg, &assertion, TAO_EQ);
    numAssertion_testOperator(tg, &assertion, TAO_NEQ);
}


static void test_numAssertionOperand(TestGroup *tg)
{
    TestAssertion assertion={0};
    TA_init(&assertion,TAK_NUM);

    numAssertion_testOperand(tg, &assertion, 0x9669966996699669LL,0xc33cc33cc33cc33cLL);
    numAssertion_testOperand(tg, &assertion, 0x0123456789ABCDEFLL,0xFEDCBA9876543210LL);
}

static inline void memAssertion_testOperator(TestGroup *tg, TestAssertion *ta, TAOperator op)
{
    TA_mem_setOperator(ta,op);
    ASSERT_INT_EQUAL(tg,"Operator set (A) should coincide with operator gotten (B) in memory assert",
            op,TA_mem_getOperator(ta));
}
static void test_memAssertionOperator(TestGroup *tg)
{
    TestAssertion assertion={0};
    TA_init(&assertion,TAK_MEM);

    memAssertion_testOperator(tg, &assertion, TAO_EQ);
    memAssertion_testOperator(tg, &assertion, TAO_NEQ);

}

static inline void memAssertion_testOperands(TestGroup *tg, TestAssertion *ta, const void *A,const void *B,size_t length)
{
    TA_mem_setA(ta,A);
    ASSERT_INT_EQUAL(tg,"First operand pointer address set (A) should coincide with addres gotten (B) in memory assert details.",
            (intptr_t)A,(intptr_t)TA_mem_getA(ta));
    TA_mem_setB(ta,B);
    ASSERT_INT_EQUAL(tg,"Second operand pointer address set (A) should coincide with addres gotten (B) in memory assert details.",
            (intptr_t)B,(intptr_t)TA_mem_getB(ta));
    TA_mem_setLength(ta,length);
    ASSERT_INT_EQUAL(tg,"Memory length set (A) should coincide with memory length gotten (B) in memory assert details.",
            length,TA_mem_getLength(ta));

}
static void test_memAssertionOperands(TestGroup *tg)
{
    TestAssertion assertion={0};
    TA_init(&assertion,TAK_MEM);
    uint8_t A1=0,B1=1;
    uint64_t A2=0,B2=1;

    memAssertion_testOperands(tg, &assertion, &A1,&B1,sizeof(A1));
    memAssertion_testOperands(tg, &assertion, &A2,&B2,sizeof(A2));

}

static inline void assertion_testMessage(TestGroup *tg, TestAssertion *ta,const char*message)
{
    TA_setMessage(ta,message);
    ASSERT_INT_EQUAL(tg,"Message address set (A) should equal message address gotten (B) in assertion.",
            (intptr_t)message,(intptr_t)TA_getMessage(ta));
}
static inline void assertion_testNullMessage(TestGroup *tg, TestAssertion *ta)
{
    TA_setMessage(ta, NULL);
    ASSERT_INT_NOT_EQUAL(tg,"When attempting to set message address to NULL (A) it should be set to default instead, gotten (B) in assertion",
            (intptr_t)NULL,(intptr_t)TA_getMessage(ta));
}
static void test_assertionMessage(TestGroup *tg)
{
    TestAssertion assertion = {0};
    TA_init(&assertion,TAK_BOOL);
    assertion_testMessage(tg, &assertion, "msg1");
    assertion_testMessage(tg, &assertion,"msg2");
}
static void test_assertionNullMessage(TestGroup *tg)
{
    TestAssertion assertion = {0};
    TA_init(&assertion,TAK_BOOL);
    assertion_testNullMessage(tg, &assertion);
}

static inline void assertion_testReason(TestGroup *tg,TestAssertion *ta,const char * reason)
{
    TA_setReason(ta,reason);
    ASSERT_INT_EQUAL(tg,"Address of reason set (A) should coincide with address gotten (B) in assertion.",
            (intptr_t)reason,(intptr_t)TA_getReason(ta));
}

static void test_assertionReason(TestGroup *tg)
{
    TestAssertion assertion = {0};
    TA_init(&assertion,TAK_BOOL);
    assertion_testReason(tg,&assertion,"reason 1");
    assertion_testReason(tg,&assertion,"reason 2");
}
static inline void assertion_testNullReason(TestGroup *tg,TestAssertion *ta)
{
    TA_setReason(ta,NULL);
    ASSERT_INT_NOT_EQUAL(tg,"Attempted set address of reason (A) should result in reason being reset instead, gotten (B) in assertion.",
            (intptr_t)NULL,(intptr_t)TA_getReason(ta));

}
static void test_assertionNullReason(TestGroup *tg)
{
    TestAssertion assertion = {0};
    TA_init(&assertion,TAK_BOOL);
    assertion_testNullReason(tg, &assertion);
}
static void test_assertionSetKind(TestGroup *tg)
{
    TestAssertion assertion ={0};
    TA_init(&assertion,TAK_BOOL);

    TA_setKind(&assertion, TAK_MEM);

    ASSERT_INT_EQUAL(tg,"After setting a new assertion king (A) it should be the new kind, gotten (B).",
            TAK_MEM,TA_getKind(&assertion));
}

static void test_assertionInvalidKind(TestGroup *tg)
{
    const char *const msg="Attempting to set assertion kind to value out of range (A) should result in its reset instead, gotten (B)";
    TestAssertion assertion = {0};
    const int invalidKind = TAK_MAX+1;
    TA_init(&assertion,invalidKind);

    ASSERT_INT_NOT_EQUAL(tg,msg,invalidKind,TA_getKind(&assertion));

    TA_setKind(&assertion, invalidKind);

    ASSERT_INT_NOT_EQUAL(tg,msg,invalidKind,TA_getKind(&assertion));

}
static void test_assertionDirectKind(TestGroup *tg)
{
    const char *const msg="After setting assertion kind to TAK_DIRECT (A) the assertion kind should be that value, gotten (B)";
    TestAssertion assertion = {0};
    TA_init(&assertion,TAK_DIRECT);

    ASSERT_INT_EQUAL(tg,msg,TAK_DIRECT,TA_getKind(&assertion));

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
        {"Boolean assertion operator data",test_boolAssertionOperator},
        {"Boolean assertion operand data",test_boolAssertionOperand},
        {"Numerical assertion operator data",test_numAssertionOperator},
        {"Numerical assertion operand data",test_numAssertionOperand},
        {"Memory assertion operator data",test_memAssertionOperator},
        {"Memory assertion operand data",test_memAssertionOperands},
        {"Assertion message is correctly set",test_assertionMessage},
        {"Assertion message address cannot be set to NULL",test_assertionNullMessage},
        {"Assertion reason is correctly set",test_assertionReason},
        {"Assertion reason address cannot be set to NULL",test_assertionNullReason},
        {"It should be possible to set a new assertion kind with setKind",test_assertionSetKind},
        {"Attempting to set assertion kind to a value out of range should instead set it to a default value",
        test_assertionInvalidKind},
        {"Direct assertion kind (for assertion results without details) should be a valid kind",
                test_assertionDirectKind},
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
