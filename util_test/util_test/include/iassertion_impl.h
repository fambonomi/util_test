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
            TAOperator operator;
            long long int A;
            long long int B;
        }num_kind;
        struct{
            TAOperator operator;
            const void *A;
            const void *B;
            size_t length;
        }mem_kind;
        struct{
            TAOperator operator;
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

void TA_bool_setOperator(TestAssertion *self, TAOperator operator);
void TA_bool_setA(TestAssertion *self,int A);
void TA_num_setOperator(TestAssertion *self,TAOperator operator);
void TA_num_setA(TestAssertion *self,long long int A);
void TA_num_setB(TestAssertion *self,long long int B);
void TA_mem_setOperator(TestAssertion *self,TAOperator operator);
void TA_mem_setA(TestAssertion *self,const void *A);
void TA_mem_setB(TestAssertion *self,const void *B);
void TA_mem_setLength(TestAssertion *self,size_t length);

void TA_setMessage(TestAssertion *self,const char *message);
void TA_setReason(TestAssertion *self,const char *reason);
#endif /* INCLUDE_IASSERTION_IMPL_H_ */
