/*
 * subgrupo_reporter.c
 *
 *  Created on: 2 may. 2021
 *      Author: Fernando
 */
#include <subgrupo_reporter.h>
#include <iassertion.h>
#include <stdio.h> /*printf*/
#include <string.h>/*memset*/

static void prefijo(const SubgrupoReporter *self)
{
    const int esRaiz = NULL == self->base;
    if (!esRaiz)
        prefijo(self->base);
    printf(self->prefijo);
}
static void lineaNueva(SubgrupoReporter *self)
{
    printf("\n");
    prefijo(self);
}
static void lineaIndentada(SubgrupoReporter *self)
{
    printf("\n");
    prefijo(self);
    printf("    ");
}
static void begin(struct TGReporter *super,const char *groupName,int numTests)
{
    SubgrupoReporter *self = (SubgrupoReporter*)super;
    lineaNueva(self);
    printf("Inicio grupo %s, %d pruebas.",groupName,numTests);
    lineaNueva(self);
}
static void test(struct TGReporter *super,const char *testName)
{
    SubgrupoReporter *self = (SubgrupoReporter*)super;
    lineaNueva(self);
    printf(testName);
}
static void pass(struct TGReporter *super)
{
    (void)super;
    printf(" [OK]");
}
static void posicion(SubgrupoReporter *self, const TestAssertion *ta)
{
    lineaIndentada(self);
    printf("Función: %s",TA_getFunc(ta));
    lineaIndentada(self);
    printf("Archivo: %s",TA_getFile(ta));
    lineaIndentada(self);
    printf("Línea: %d",TA_getLine(ta));
}
static const char* strOperador(TAOperator op)
{
    switch(op){
    case TAO_EQ: return "==";
    case TAO_NEQ: return "!=";
    case TAO_IS_FALSE: return "is false";
    case TAO_IS_TRUE: return "is true";
    default: return "<[INCONSISTENCIA!]>";
    }
}
static const char* strResult(TAResult r)
{
    switch(r){
    case TA_PASS:return "Correcto:";
    case TA_FAIL:return "Falla:";
    case TA_ERROR:return "Error:";
    default: return "<[INCONSISTENCIA!]>";
    }
}
#define ASERCION_FALLIDA "Aserción fallida: "
static void assertion_mem(SubgrupoReporter *self,const TestAssertion *ta)
{
    lineaIndentada(self);
    printf(ASERCION_FALLIDA "[%u bytes @ %p] %s [%u bytes @ %p]",
            TA_mem_getLength(ta),TA_mem_getA(ta),
            strOperador(TA_mem_getOperator(ta)),
            TA_mem_getLength(ta),TA_mem_getB(ta));
}
static void assertion_num(SubgrupoReporter *self,const TestAssertion *ta)
{
    lineaIndentada(self);
    printf(ASERCION_FALLIDA "%lld %s %lld",
            TA_num_getA(ta),
            strOperador(TA_num_getOperator(ta)),
            TA_num_getB(ta));
}
static void assertion_bool(SubgrupoReporter *self,const TestAssertion *ta)
{
    lineaIndentada(self);
    printf(ASERCION_FALLIDA "%s %s",
            TA_bool_getA(ta)? "[predicado verdadero]":"[predicado falso]",
            strOperador(TA_bool_getOperator(ta)));
}
static void kindDetail(SubgrupoReporter *self,const TestAssertion *ta)
{
    switch(TA_getKind(ta)){
    case TAK_MEM:assertion_mem(self, ta);return;
    case TAK_NUM:assertion_num(self,ta);return;
    case TAK_BOOL:assertion_bool(self,ta);return;
    default:return;
    }
}
static void assertion_detail(SubgrupoReporter *self,const TestAssertion *ta)
{
    lineaIndentada(self);
    printf("%s %s",strResult(TA_getResult(ta)), TA_getMessage(ta));
    lineaIndentada(self);
    printf("Razón: %s",TA_getReason(ta));
    kindDetail(self, ta);
    posicion(self, ta);
}
static void fail(struct TGReporter *super,const TestAssertion *ta)
{
    SubgrupoReporter *self = (SubgrupoReporter*)super;
    printf(" [FALLA]");
    assertion_detail(self, ta);
}
static void error(struct TGReporter *super,const TestAssertion *ta)
{
    SubgrupoReporter *self = (SubgrupoReporter*)super;
    printf(" [ERROR]");
    assertion_detail(self, ta);
}
static void summary(struct TGReporter *super,const TestGroupOutcome *summary)
{
    SubgrupoReporter *self = (SubgrupoReporter*)super;
    lineaNueva(self);
    lineaNueva(self);
    printf("Pruebas ejecutadas: %d",summary->run);
    lineaNueva(self);
    printf("Pruebas pasadas: %d",summary->passed);
    lineaNueva(self);
    printf("Fallas ocurridas: %d",summary->failed);
    lineaNueva(self);
    printf("Errores ocurridos: %d",summary->error);
    lineaNueva(self);
    printf("Fin de grupo\n");
}


void SubgrupoReporter_init(SubgrupoReporter *self, const char *prefijo)
{
    TGReporter *tgr = &self->super;
    memset(self, 0,sizeof(*self));

    if (NULL == prefijo)
        self->prefijo="";
    else
        self->prefijo = prefijo;
    self->base = NULL;

    tgr->begin=begin;
    tgr->test=test;
    tgr->pass=pass;
    tgr->fail=fail;
    tgr->error=error;
    tgr->summary=summary;
}

void SubgrupoReporter_init_conBase(SubgrupoReporter *self,const SubgrupoReporter *base,const char *prefijo)
{
    SubgrupoReporter_init(self,prefijo);
    self->base = base;
}

TGReporter *SubgrupoReporter_comoTGReporter(SubgrupoReporter *self)
{
    return &self->super;
}

