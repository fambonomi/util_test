/*
 * test_assertion.h
 *
 *  Created on: 26 abr. 2021
 *      Author: Fernando
 */

#ifndef INCLUDE_IASSERTION_IMPL_H_
#define INCLUDE_IASSERTION_IMPL_H_
#include <stddef.h> /*size_t*/
#include <iassertion.h>

struct TestAssertion{
    TAResult result;
    const char *message;
    const char *reason;
    struct{
        const char *func;
        const char *file;
        int line;
    } position;
    TAKind detailKind;
    union{
        struct{
            TAOperator assertion;
            long long int A;
            long long int B;
        }num_kind;
        struct{
            TAOperator assertion;
            const void *A;
            const void *B;
            size_t length;
        }mem_kind;
        struct{
            TAOperator assertion;
            int A;
        }bool_kind;
    }detail;
};

void TA_init(TestAssertion *self, TAKind kind);
void TA_setKind(TestAssertion *self,TAKind kind);
void TA_setResult(TestAssertion *self,TAResult result);
void TA_setFile(TestAssertion *self,const char *file);
void TA_setLine(TestAssertion *self,int line);
void TA_setFunc(TestAssertion *self,const char *func);

#endif /* INCLUDE_IASSERTION_IMPL_H_ */
