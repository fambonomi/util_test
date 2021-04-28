/*
 * assertions.c
 *
 *  Created on: 23 abr. 2021
 *      Author: Fernando
 */
#include <assertions.h>
#include <stdio.h> /*snprintf*/
#include <string.h> /*memcmp*/
#include <iassertion_impl.h>
#include <test_impl.h>

static void TA__setPosition(TestAssertion *ta,const char *func,const char *file,int line)
{
    TA_setFunc(ta, func);
    TA_setFile(ta, file);
    TA_setLine(ta, line);
}
static void TA__initAssertBoolFailed(TestAssertion *ta,int A,TAOperator op)
{
    TA_init(ta, TAK_BOOL);
    TA_setResult(ta, TA_FAIL);
    TA_bool_setA(ta, A);
    TA_bool_setOperator(ta,op);
}

static void TA__initAssertNumFailed(TestAssertion *ta,long long int A,long long int B,TAOperator op)
{
    TA_init(ta, TAK_NUM);
    TA_setResult(ta, TA_FAIL);
    TA_num_setOperator(ta, op);
    TA_num_setA(ta, A);
    TA_num_setB(ta, B);
}

static void TA__initAssertMemoryFail(TestAssertion *ta,const void *A,const void *B,size_t length,TAOperator op)
{
    TA_init(ta, TAK_MEM);
    TA_setResult(ta, TA_FAIL);
    TA_mem_setOperator(ta, op);
    TA_mem_setA(ta, A);
    TA_mem_setB(ta, B);
    TA_mem_setLength(ta, length);
}
static void TA__initAssertBadPtr(TestAssertion *ta,const void *A,const void *B,size_t length,TAOperator op)
{
    TA_init(ta,TAK_MEM);
    TA_setResult(ta, TA_ERROR);
    TA_mem_setOperator(ta, op);
    TA_mem_setA(ta, A);
    TA_mem_setB(ta, B);
    TA_mem_setLength(ta, length);
}

static void TG__assertMemoryBadPtrGuard(
        TestGroup *tg,TestAssertion *ta, const char *message, const void *A,
        const void *B, size_t length, TAOperator op,
        const char *func,const char *file,int line)
{
    const char *const reason = "Asked to dereference null pointer.";
    const int validPointers = (const void*) 0 != A &&
                              (const void*) 0 != B;
    if(!validPointers)
    {
        TA__initAssertBadPtr(ta, A, B, length, op);
        TA_setMessage(ta, message);
        TA_setReason(ta, reason);
        TA__setPosition(ta, func, file, line);
        TG_reportAssertionResult(tg, ta);
    }
}

static int memoryEqual(const void *A,const void *B,size_t length)
{
    const int logicallyEqual = A == B ||
                               0 == length;

    return logicallyEqual || !memcmp(A,B,length);

}

/**
 * Public API
 */

void TG_assertTrue(TestGroup *tg,const char* message,int value,const char *func,const char *file,int line)
{
    TestAssertion ta;
    if(!value)
    {
        TA__initAssertBoolFailed(&ta, value, TAO_IS_TRUE);
        TA_setMessage(&ta, message);
        TA_setReason(&ta, "Value that should be true is false");
        TA__setPosition(&ta, func, file, line);
        TG_reportAssertionResult(tg, &ta);
    }
}

void TG_assertFalse(TestGroup *tg,const char* message,int value,const char *func,const char *file,int line)
{
    TestAssertion ta;
    if(value)
    {
        TA__initAssertBoolFailed(&ta, value, TAO_IS_FALSE);
        TA_setMessage(&ta, message);
        TA_setReason(&ta, "Value that should be false is true");
        TA__setPosition(&ta, func, file, line);
        TG_reportAssertionResult(tg, &ta);
    }
}

void TG_assertIntEqual(TestGroup *tg,const char* message,long long int A,long long int B,const char *func,const char *file,int line)
{
    TestAssertion ta;
    const char* reason = "Integers A and B should have been equal";
    if (A != B){
        TA__initAssertNumFailed(&ta, A, B, TAO_EQ);
        TA_setMessage(&ta, message);
        TA_setReason(&ta, reason);
        TA__setPosition(&ta, func, file, line);
        TG_reportAssertionResult(tg, &ta);
    }
}

void TG_assertIntNotEqual(TestGroup *tg,const char* message,long long int A,long long int B,const char *func,const char *file,int line)
{
    TestAssertion ta;
    const char* reason = "Integers A and B should have been different";
    if (A == B){
        TA__initAssertNumFailed(&ta, A, B, TAO_NEQ);
        TA_setMessage(&ta, message);
        TA_setReason(&ta, reason);
        TA__setPosition(&ta, func, file, line);
        TG_reportAssertionResult(tg, &ta);
    }
}

void TG_assertMemoryEqual(TestGroup *tg,const char* message,
                          const void *A,const void *B, size_t length,
                          const char *func,const char *file, int line)
{
    TestAssertion ta;
    const TAOperator op = TAO_EQ;
    const char* reason = "Content of memory blocks should be the same but is different";

    TG__assertMemoryBadPtrGuard(tg, &ta, message, A, B, length, op, func, file, line);

    if (!memoryEqual(A, B, length))
    {
        TA__initAssertMemoryFail(&ta, A, B, length, op);
        TA_setMessage(&ta, message);
        TA_setReason(&ta, reason);
        TA__setPosition(&ta, func, file, line);
        TG_reportAssertionResult(tg, &ta);
    }
}


void TG_assertMemoryNotEqual(TestGroup *tg,const char* message,
                          const void *A,const void *B, size_t length,
                          const char *func,const char *file, int line)
{
    TestAssertion ta;
    const TAOperator op = TAO_NEQ;
    const char* reason = "Content of memory blocks should be different but is the same";

    TG__assertMemoryBadPtrGuard(tg, &ta, message, A, B, length, op, func, file, line);

    if (memoryEqual(A, B, length))
    {
        TA__initAssertMemoryFail(&ta, A, B, length, op);
        TA_setMessage(&ta, message);
        TA_setReason(&ta, reason);
        TA__setPosition(&ta, func, file, line);
        TG_reportAssertionResult(tg, &ta);
    }
}

void TG_fail(TestGroup *tg, const char *message, const char *reason,const char *func,const char *file, int line)
{
    TestAssertion ta;

    TA_init(&ta, TAK_DIRECT);
    TA_setResult(&ta, TA_FAIL);
    TA_setMessage(&ta, message);
    TA_setReason(&ta, reason);
    TA__setPosition(&ta, func, file, line);
    TG_reportAssertionResult(tg, &ta);
}

void TG_error(TestGroup *tg,const char *message, const char *reason,const char *func,const char *file, int line)
{
    TestAssertion ta;

    TA_init(&ta, TAK_DIRECT);
    TA_setResult(&ta, TA_ERROR);
    TA_setMessage(&ta, message);
    TA_setReason(&ta, reason);
    TA__setPosition(&ta, func, file, line);
    TG_reportAssertionResult(tg, &ta);
}
