/*
 * test_assertion.c
 *
 *  Created on: 26 abr. 2021
 *      Author: Fernando
 */

#include <iassertion_impl.h>

#ifdef NO_INLINE
#define INLINE
#else
#define INLINE inline
#endif

static INLINE void TA__unsetMessage(TestAssertion *self)
{
    self->message = "No description given";
}
static INLINE void TA__unsetReason(TestAssertion *self)
{
    self->reason = "No reason given";
}
static INLINE void TA__unsetFile(TestAssertion *self)
{
    self->position.file = "<No file given>";
}
static INLINE void TA__unsetFunc(TestAssertion *self)
{
    self->position.func = "<No function given>";
}
static INLINE void TA_mem__init(TestAssertion *self)
{
    self->detail.mem_kind.A=NULL;
    self->detail.mem_kind.B=NULL;
    self->detail.mem_kind.length=0;
}

static INLINE void TA__initDefaultValues(TestAssertion *self)
{
    TA__unsetMessage(self);
    TA__unsetReason(self);
    TA__unsetFile(self);
    TA__unsetFunc(self);
    self->position.line = 0;
}

/**
 * Implementation public code
 */
void TA_init(TestAssertion *self, TAKind kind)
{
    TA_setKind(self,kind);

    TA__initDefaultValues(self);

    if (TAK_MEM == kind)
        TA_mem__init(self);
}

void TA_setKind(TestAssertion *self, TAKind kind)
{
    const int isValid = kind >=0 && kind <= TAK_MAX;
    if (isValid)
        self->detailKind=kind;
    else
        self->detailKind=TAK_NUM;

}

void TA_setResult(TestAssertion *self,TAResult result)
{
    const int isValid = result >= 0 && result <= TA_MAX;

    if (isValid)
        self->result=result;
    else
        self->result=TA_PASS;
}

void TA_setFile(TestAssertion *self,const char *file)
{
    const int isValid = NULL != file;
    if (isValid)
        self->position.file=file;
    else
        TA__unsetFile(self);
}
void TA_setLine(TestAssertion *self,int line)
{
    const int isValid = line > 0;
    if (isValid)
        self->position.line = line;
    else
        self->position.line = 0;
}
void TA_setFunc(TestAssertion *self,const char *func)
{
    const int isValid = NULL != func;
    if (isValid)
        self->position.func=func;
    else
        TA__unsetFunc(self);
}
void TA_bool_setOperator(TestAssertion *self, TAOperator operator)
{
    self->detail.bool_kind.operator = operator;
}
void TA_bool_setA(TestAssertion *self,int A)
{
    self->detail.bool_kind.A=!!A;
}

void TA_num_setOperator(TestAssertion *self,TAOperator operator)
{
    self->detail.num_kind.operator = operator;
}
void TA_num_setA(TestAssertion *self,long long int A)
{
    self->detail.num_kind.A=A;
}
void TA_num_setB(TestAssertion *self,long long int B)
{
    self->detail.num_kind.B=B;
}
void TA_mem_setOperator(TestAssertion *self,TAOperator operator)
{
    self->detail.mem_kind.operator = operator;
}
void TA_mem_setA(TestAssertion *self,const void *A)
{
    self->detail.mem_kind.A=A;
}
void TA_mem_setB(TestAssertion *self,const void *B)
{
    self->detail.mem_kind.B=B;
}
void TA_mem_setLength(TestAssertion *self,size_t length)
{
    self->detail.mem_kind.length = length;
}
void TA_setMessage(TestAssertion *self,const char *message)
{
    const int nullMessage = NULL == message;
    if (nullMessage)
        TA__unsetMessage(self);
    else
        self->message=message;
}
void TA_setReason(TestAssertion *self,const char *reason)
{
    const int nullReason = NULL == reason;
    if (nullReason)
        TA__unsetReason(self);
    else
        self->reason = reason;
}
/**
 * user public code
 */
const char *TA_getMessage(const TestAssertion *self)
{
    return self->message;
}
const char *TA_getReason(const TestAssertion *self)
{
    return self->reason;
}
const char *TA_getFile(const TestAssertion *self)
{
    return self->position.file;
}
const char *TA_getFunc(const TestAssertion *self)
{
    return self->position.func;
}
TAKind TA_getKind(const TestAssertion *self)
{
    return self->detailKind;
}
TAResult TA_getResult(const TestAssertion *self)
{
    return self->result;
}

const void * TA_mem_getPA(const TestAssertion *self)
{
    return self->detail.mem_kind.A;
}
const void * TA_mem_getPB(const TestAssertion *self)
{
    return self->detail.mem_kind.B;
}
size_t TA_mem_getLength(const TestAssertion *self)
{
    return self->detail.mem_kind.length;
}

int TA_getLine(const TestAssertion *self)
{
    return self->position.line;
}

TAOperator TA_bool_getOperator(const TestAssertion *self)
{
    return self->detail.bool_kind.operator;
}
int TA_bool_getA(const TestAssertion *self)
{
    return self->detail.bool_kind.A;
}
TAOperator TA_num_getOperator(const TestAssertion *self)
{
    return self->detail.num_kind.operator;
}
long long int TA_num_getA(const TestAssertion *self)
{
    return self->detail.num_kind.A;
}
long long int TA_num_getB(const TestAssertion *self)
{
    return self->detail.num_kind.B;
}

TAOperator TA_mem_getOperator(const TestAssertion *self)
{
    return self->detail.mem_kind.operator;
}
const void *TA_mem_getA(const TestAssertion *self)
{
    return self->detail.mem_kind.A;
}
const void *TA_mem_getB(const TestAssertion *self)
{
    return self->detail.mem_kind.B;
}
