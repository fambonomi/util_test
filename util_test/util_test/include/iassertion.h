/*
 * test_assertion.h
 *
 *  Created on: 26 abr. 2021
 *      Author: Fernando
 */

#ifndef INCLUDE_IASSERTION_H_
#define INCLUDE_IASSERTION_H_

typedef struct TestAssertion TestAssertion;
typedef enum {TA_PASS=0, TA_FAIL, TA_ERROR,TA_MAX=TA_ERROR}TAResult;
typedef enum {TAK_NUM=0,TAK_MEM,TAK_BOOL,TAK_DIRECT,TAK_MAX=TAK_DIRECT}TAKind;
typedef enum {TAO_EQ=0,TAO_NEQ, TAO_IS_TRUE, TAO_IS_FALSE, TAO_MAX=TAO_IS_FALSE} TAOperator;

TAResult TA_getResult(const TestAssertion *self);
TAKind TA_getKind(const TestAssertion *self);
const char * TA_getMessage(const TestAssertion *self);
const char * TA_getReason(const TestAssertion *self);

const char * TA_getFunc(const TestAssertion *self);
const char * TA_getFile(const TestAssertion *self);
int TA_getLine(const TestAssertion *self);

const void * TA_mem_getPA(const TestAssertion *self);
const void * TA_mem_getPB(const TestAssertion *self);
size_t TA_mem_getLength(const TestAssertion *self);

TAOperator TA_bool_getOperator(const TestAssertion *self);
int TA_bool_getA(const TestAssertion *self);

TAOperator TA_num_getOperator(const TestAssertion *self);
long long int TA_num_getA(const TestAssertion *self);
long long int TA_num_getB(const TestAssertion *self);

TAOperator TA_mem_getOperator(const TestAssertion *self);
const void *TA_mem_getA(const TestAssertion *self);
const void *TA_mem_getB(const TestAssertion *self);
#endif /* INCLUDE_IASSERTION_H_ */
