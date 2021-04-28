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

    ASSERT_INT_EQUAL(tg,((A==B)?"This should pass":"This should fail"),
                     A,B);
}

static void mockTest_assertIntNotEqual(TestGroup *tg)
{
    const long long int A = mockTestParams.integers.A;
    const long long int B = mockTestParams.integers.B;

    ASSERT_INT_NOT_EQUAL(tg,((A!=B)?"This should pass":"This should fail"),
                     A,B);
}

static void mockTest_assertMemoryEqual(TestGroup *tg)
{
    const void *const A = mockTestParams.memBlocks.A;
    const void *const B = mockTestParams.memBlocks.B;
    const size_t length = mockTestParams.memBlocks.length;
    int expectedOutcome = 3;
    static const char* outcomes[]={
            "This should fail",
            "This should pass",
            "This should be an error",
            "Internal error!"
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
            "This should pass",
            "This should fail",
            "This should be an error",
            "Internal error!"
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
    TG_init(&group,"Mock group");

    TG_setTests(&group,testList,1);
    TG_runTests(&group);

    const int passed = PASSED == expectation;
    const int failed = FAILED == expectation;
    const int error = ERROR == expectation;

    return verifyExpecedResults(&group, passed, failed, error);
}
static void mockTestResultUnexpected(TestGroup *tg,const char* message)
{
    REPORT_ERROR(tg,message,"Mock test yields unexpected result.");

}
static void test_assertTrue_true(TestGroup *tg)
{
    if (!testResultsAsExpected("assertTrue of true",
            mockTest_assertTrue_true, PASSED))
        mockTestResultUnexpected(tg, "assertTrue of true must succeed");
}

static void test_assertTrue_false(TestGroup *tg)
{
    if (!testResultsAsExpected("assertTrue of false",
            mockTest_assertTrue_false, FAILED))
        mockTestResultUnexpected(tg,"assertTrue of false must fail");

}

static void test_assertFalse_true(TestGroup *tg)
{
    if (!testResultsAsExpected("assertFalse of true",
            mockTest_assertFalse_true, FAILED))
        mockTestResultUnexpected(tg,"assertFalse of true must fail");

}

static void test_assertFalse_false(TestGroup *tg)
{
    if (!testResultsAsExpected("assertFalse of false",
            mockTest_assertFalse_false, PASSED))
        mockTestResultUnexpected(tg,"assertFalse of false must pass");

}

static void test_assertIntEqual_pass(TestGroup *tg)
{
    mockTestParams.integers.A=0x0807060504030201LL;
    mockTestParams.integers.B=mockTestParams.integers.A;
    if (!testResultsAsExpected("assertIntEqual for A==B",
            mockTest_assertIntEqual, PASSED))
        mockTestResultUnexpected(tg,"assertIntEqual must pass if A==B");
}

static void test_assertIntEqual_fail(TestGroup *tg)
{
    mockTestParams.integers.A=0x0807060504030201LL;
    mockTestParams.integers.B=-mockTestParams.integers.A;
    if (!testResultsAsExpected("assertIntEqual for A!=B",
            mockTest_assertIntEqual, FAILED))
        mockTestResultUnexpected(tg,"assertIntEqual must fail if A!=B");
}

static void test_assertIntNotEqual_fail(TestGroup *tg)
{
    mockTestParams.integers.A=0x5a5a5a5aa5a5a5a5LL;
    mockTestParams.integers.B=mockTestParams.integers.A;
    if (!testResultsAsExpected("assertIntNotEqual for A==B",
            mockTest_assertIntNotEqual, FAILED))
        mockTestResultUnexpected(tg,"assertIntNotEqual must fail if A==B");
}

static void test_assertIntNotEqual_pass(TestGroup *tg)
{
    mockTestParams.integers.A=0x5a5a5a5aa5a5a5a5LL;
    mockTestParams.integers.B=0x0f0f0f0ff0f0f0f0LL;
    if (!testResultsAsExpected("assertIntNotEqual for A!=B",
            mockTest_assertIntNotEqual, PASSED))
        mockTestResultUnexpected(tg,"assertIntNotEqual must pass if A!=B");
}

static void test_assertMemoryEqual_pass(TestGroup *tg)
{
    char mem1[]="This is a sentence.";
    char mem2[]="This is a sentence.";
    mockTestParams.memBlocks.A=mem1;
    mockTestParams.memBlocks.B=mem2;
    mockTestParams.memBlocks.length=sizeof(mem1);

    if(!testResultsAsExpected("assertMemoryEqual for blocks with identical contents",
            mockTest_assertMemoryEqual, PASSED))
        mockTestResultUnexpected(tg, "assertMemoryEqual must pass for blocks with identical contents");
}

static void test_assertMemoryEqual_fail(TestGroup *tg)
{
    char mem1[]="This is a sentence. ";
    char mem2[]="This is another one.";
    mockTestParams.memBlocks.A=mem1;
    mockTestParams.memBlocks.B=mem2;
    mockTestParams.memBlocks.length=sizeof(mem1);

    if(!testResultsAsExpected("assertMemoryEqual for blocks with different contents",
            mockTest_assertMemoryEqual, FAILED))
        mockTestResultUnexpected(tg, "assertMemoryEqual must fail for blocks with different contents");
}

static void test_assertMemoryEqual_error(TestGroup *tg)
{
    mockTestParams.memBlocks.A=(const void*) 0;
    mockTestParams.memBlocks.B=(const void*) 0;
    mockTestParams.memBlocks.length=0;

    if(!testResultsAsExpected("assertMemoryEqual with null pointer",
            mockTest_assertMemoryEqual, ERROR))
        mockTestResultUnexpected(tg, "assertMemoryEqual should give an error if asked to dereference a null pointer");
}

static void test_assertMemoryNotEqual_fail(TestGroup *tg)
{
    char mem1[]="This is a sentence.";
    char mem2[]="This is a sentence.";
    mockTestParams.memBlocks.A=mem1;
    mockTestParams.memBlocks.B=mem2;
    mockTestParams.memBlocks.length=sizeof(mem1);

    if(!testResultsAsExpected("assertMemoryNotEqual for blocks with identical contents",
            mockTest_assertMemoryNotEqual, FAILED))
        mockTestResultUnexpected(tg, "assertMemoryNotEqual must fail for blocks with identical contents");
}

static void test_assertMemoryNotEqual_pass(TestGroup *tg)
{
    char mem1[]="This is a sentence. ";
    char mem2[]="This is another one.";
    mockTestParams.memBlocks.A=mem1;
    mockTestParams.memBlocks.B=mem2;
    mockTestParams.memBlocks.length=sizeof(mem1);

    if(!testResultsAsExpected("assertMemoryNotEqual for blocks with different contents",
            mockTest_assertMemoryNotEqual, PASSED))
        mockTestResultUnexpected(tg, "assertMemoryNotEqual must pass for blocks with different contents");
}

static void test_assertMemoryNotEqual_error(TestGroup *tg)
{
    mockTestParams.memBlocks.A=(const void*) 0;
    mockTestParams.memBlocks.B=(const void*) 0;
    mockTestParams.memBlocks.length=0;

    if(!testResultsAsExpected("assertMemoryNotEqual with null pointer",
            mockTest_assertMemoryNotEqual, ERROR))
        mockTestResultUnexpected(tg, "assertMemoryNotEqual should give an error if asked to dereference a null pointer");
}

static TestDescriptor tests[]={
        {"# Asserting that something true is true passes\n",test_assertTrue_true},
        {"# Asserting that something false is true fails\n",test_assertTrue_false},
        {"# Asserting that something true is false fails\n",test_assertFalse_true},
        {"# Asserting that something false is false passes\n",test_assertFalse_false},
        {"# Asserting that equal ints are equal passes\n",test_assertIntEqual_pass},
        {"# Asserting that different ints are equal fails\n",test_assertIntEqual_fail},
        {"# Asserting that equal ints are not equal fails\n",test_assertIntNotEqual_fail},
        {"# Asserting that different ints are not equal passes\n",test_assertIntNotEqual_pass},
        {"# Asserting that the contents of two memory blocks are equal when they are equal\n",
                test_assertMemoryEqual_pass},
        {"# Asserting that the contents of two memory blocks are equal when they are different\n",
                test_assertMemoryEqual_fail},
        {"# ASSERT_MEMORY_EQUAL is given null pointers\n",
                test_assertMemoryEqual_error},
        {"# Asserting that the contents of two memory blocks are different when they are different\n",
                test_assertMemoryNotEqual_pass},
        {"# Asserting that the contents of two memory blocks are different when they are equal\n",
                test_assertMemoryNotEqual_fail},
        {"# ASSERT_MEMORY_NOT_EQUAL is given null pointers\n",
                test_assertMemoryNotEqual_error},
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
