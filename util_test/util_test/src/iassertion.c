/*
 * test_assertion.c
 *
 *  Created on: 26 abr. 2021
 *      Author: Fernando
 */

#include <iassertion_impl.h>

static void TA__unsetMessage(TestAssertion *self)
{
    self->message = "No description given";
}
static void TA__unsetReason(TestAssertion *self)
{
    self->reason = "No reason given";
}
static void TA__unsetFile(TestAssertion *self)
{
    self->position.file = "<No file given>";
}
static void TA__unsetFunc(TestAssertion *self)
{
    self->position.func = "<No function given>";
}
static void TA_mem__init(TestAssertion *self)
{
    self->detail.mem_kind.A=NULL;
    self->detail.mem_kind.B=NULL;
    self->detail.mem_kind.length=0;
}

static void TA__initDefaultValues(TestAssertion *self)
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
