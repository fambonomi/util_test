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
#include <test_run.h>

#define DEBE_SER_CADENA_VALIDA(Campo) "Luego de inicializar, TA_get"Campo" debe ser una cadena válida"
static inline void empty_assertion_consistency(
        TestGroup *tg,const TestAssertion *assertion,TAKind initKind)
{
    const intptr_t nullPtr = (intptr_t)NULL;
    ASSERT_INT_NOT_EQUAL(tg,DEBE_SER_CADENA_VALIDA("Message"),
            nullPtr,(intptr_t) TA_getMessage(assertion));
    ASSERT_INT_NOT_EQUAL(tg,DEBE_SER_CADENA_VALIDA("Reason"),
            nullPtr,(intptr_t) TA_getReason(assertion));
    ASSERT_INT_NOT_EQUAL(tg,DEBE_SER_CADENA_VALIDA("File"),
            nullPtr,(intptr_t)TA_getFile(assertion));
    ASSERT_INT_NOT_EQUAL(tg,DEBE_SER_CADENA_VALIDA("Func"),
            nullPtr,(intptr_t)TA_getFunc(assertion));
    ASSERT_INT_EQUAL(tg,"After initialization kind should be as specified in init",
            initKind,TA_getKind(assertion));
    ASSERT_INT_EQUAL(tg,"After initialization result should be Pass",
            TA_PASS,TA_getResult(assertion));
}
#undef DEBE_SER_CADENA_VALIDA

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
#define PROPIEDAD_DEBE_SER(Propiedad,valor) "Luego de inicializar, TA_mem_get"Propiedad" debe ser "valor
static void test_empty_mem_assertion(TestGroup *tg)
{
    TestAssertion assertion={.detail={.mem_kind={.A=(void *)0x5a5a5a5a,.B=(void *)0x96969696}}};
    test_empty_assertion_consistency(tg,&assertion, TAK_MEM);
    const intptr_t nullPtr = (intptr_t) NULL;

    ASSERT_INT_EQUAL(tg,PROPIEDAD_DEBE_SER("PA","puntero nulo"),
            nullPtr,(intptr_t)TA_mem_getPA(&assertion));
    ASSERT_INT_EQUAL(tg,PROPIEDAD_DEBE_SER("PB","puntero nulo"),
            nullPtr,(intptr_t)TA_mem_getPB(&assertion));
    ASSERT_INT_EQUAL(tg,PROPIEDAD_DEBE_SER("Length","0"),
            0,TA_mem_getLength(&assertion));
}
#undef PROPIEDAD_DEBE_SER
static inline void test__result(TestGroup *tg,TAResult result)
{
    TestAssertion assertion;
    const int validResult = result >= 0 && result <= TA_MAX;

    ASSERT_TRUE(tg,"[<El argumento 'result' debe ser TAResult válido>]",validResult);

    TA_init(&assertion, TAK_BOOL);
    TA_setResult(&assertion, result);

    ASSERT_INT_EQUAL(tg,"TA_getResult (primer entero) debe coincidir con el dado a TA_setResult (segundo entero)",
            TA_getResult(&assertion),result);
}
static inline void test__invalidResult(TestGroup *tg,TAResult result)
{
    TestAssertion assertion;
    const int validResult = result >= 0 && result <= TA_MAX;

    ASSERT_FALSE(tg,"[<El argumento 'result' debe ser TAResult INVÁLIDO>]",validResult);

    TA_init(&assertion, TAK_BOOL);
    TA_setResult(&assertion, result);

    ASSERT_INT_NOT_EQUAL(tg,"TA_getResult (primer entero) no debe ser el resultado inválido dado a TA_setResult (segundo entero)",
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

    ASSERT_INT_EQUAL(tg,"El puntero devuelto por TA_getFile (primer entero) debe ser el puntero no nulo dado a TA_setFile (segundo entero)",
            (intptr_t)TA_getFile(&assertion),(intptr_t)file);
    ASSERT_INT_EQUAL(tg,"El puntero devuelto por TA_getFunc (primer entero) debe ser el puntero no nulo dado a TA_setFunc (segundo entero)",
            (intptr_t)TA_getFunc(&assertion),(intptr_t)func);
    ASSERT_INT_EQUAL(tg,"El número devuelto por TA_getLine (primer entero) debe ser el número no negativo dado a TA_setLine (segundo entero)",
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

    ASSERT_INT_NOT_EQUAL(tg,"El puntero devuelto por TA_getFile (primer entero) debe ser un puntero no nulo cuando se da uno nulo a TA_setFile (segundo entero)",
            (intptr_t)TA_getFile(&assertion),(intptr_t)file);
    ASSERT_INT_NOT_EQUAL(tg,"El puntero devuelto por TA_getFunc (primer entero) debe ser un puntero no nulo cuando se da uno nulo a TA_setFunc (segundo entero)",
            (intptr_t)TA_getFunc(&assertion),(intptr_t)func);
    ASSERT_INT_NOT_EQUAL(tg,"El número devuelto por TA_getLine (primer entero) debe ser 0 cuando se dió un número negativo a TA_setLine (segundo entero)",
            TA_getLine(&assertion),line);

}

#define OPERADOR_DEBE_COINCIDIR(tipoAsercion) "El operador devuelto por TA_"tipoAsercion"_getOperator (primer entero) debe coincidir con el operador dado a TA_"tipoAsercion"_setOperator (segundo entero)."
static inline void boolAssertion_testOperator(TestGroup *tg, TestAssertion *ta, TAOperator op)
{
    TA_bool_setOperator(ta,op);
    ASSERT_INT_EQUAL(tg,OPERADOR_DEBE_COINCIDIR("bool"),
            TA_bool_getOperator(ta),op);
}
#define OPERANDO_DEBE_COINCIDIR(tipoAsercion,operando) "El operando devuelto por TA_"tipoAsercion"_get"operando" (primer entero) debe coincidir con el operador dado a TA_"tipoAsercion"_set"operando" (segundo entero)."
static inline void boolAssertion_testOperand(TestGroup *tg, TestAssertion *ta, int A)
{
    TA_bool_setA(ta,A);
    ASSERT_INT_EQUAL(tg,OPERANDO_DEBE_COINCIDIR("bool","A"),
            TA_bool_getA(ta),A);

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
    ASSERT_INT_EQUAL(tg,OPERADOR_DEBE_COINCIDIR("num"),
            TA_num_getOperator(ta),op);
}

static inline void numAssertion_testOperand(TestGroup *tg, TestAssertion *ta, long long int A, long long int B)
{
    TA_num_setA(ta,A);
    TA_num_setB(ta,B);
    ASSERT_INT_EQUAL(tg,OPERANDO_DEBE_COINCIDIR("num","A"),
            TA_num_getA(ta),A);
    ASSERT_INT_EQUAL(tg,OPERANDO_DEBE_COINCIDIR("num","B"),
            TA_num_getB(ta),B);

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

    numAssertion_testOperand(tg, &assertion, 0x6996699669966996LL,0x3cc33cc33cc33cc3LL);
    numAssertion_testOperand(tg, &assertion, 0x0123456789ABCDEFLL,0x7EDCBA98F6543210LL);
}

static inline void memAssertion_testOperator(TestGroup *tg, TestAssertion *ta, TAOperator op)
{
    TA_mem_setOperator(ta,op);
    ASSERT_INT_EQUAL(tg,OPERADOR_DEBE_COINCIDIR("mem"),
            TA_mem_getOperator(ta),op);
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
    ASSERT_INT_EQUAL(tg,OPERANDO_DEBE_COINCIDIR("mem","A"),
            (intptr_t)TA_mem_getA(ta),(intptr_t)A);
    TA_mem_setB(ta,B);
    ASSERT_INT_EQUAL(tg,OPERANDO_DEBE_COINCIDIR("mem","B"),
            (intptr_t)TA_mem_getB(ta),(intptr_t)B);
    TA_mem_setLength(ta,length);
    ASSERT_INT_EQUAL(tg,"La longitud devuelta por TA_mem_getLength (primer entero) debe coincidir con la dada a TA_mem_setLength (segundo entero).",
            TA_mem_getLength(ta),length);

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
    ASSERT_INT_EQUAL(tg,"TA_getMessage (primer entero) debe devolver el puntero no nulo dado a TA_setMessage (segundo entero).",
            (intptr_t)TA_getMessage(ta),(intptr_t)message);
}
static inline void assertion_testNullMessage(TestGroup *tg, TestAssertion *ta)
{
    TA_setMessage(ta, NULL);
    ASSERT_INT_NOT_EQUAL(tg,"TA_getMessage (primer entero) debe devolver un puntero por defecto si se dió uno nulo a TA_setMessage (segundo entero)",
            (intptr_t)TA_getMessage(ta),(intptr_t)NULL);
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
    ASSERT_INT_EQUAL(tg,"TA_getReason (primer entero) debe devolver el puntero no nulo dado a TA_setReason (segundo entero).",
            (intptr_t)TA_getReason(ta),(intptr_t)reason);
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
    ASSERT_INT_NOT_EQUAL(tg,"TA_getReason (primer entero) debe devolver un puntero por defecto si se da uno nulo a TA_setReason (segundo entero).",
            (intptr_t)TA_getReason(ta),(intptr_t)NULL);

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

    ASSERT_INT_EQUAL(tg,"TA_getKind (primer entero) debe devolver el valor dado a la última llamada a TA_setKind (segundo entero).",
            TA_getKind(&assertion),TAK_MEM);
}

#define NO_ESTABLECER_TIPO_INVALIDO(setter) "TA_getKind (primer entero) debe devolver el valor por defecto si se suministró un dato inválido a "setter" (segundo entero)"
static void test_assertionInvalidKind(TestGroup *tg)
{
    TestAssertion assertion = {0};
    const int invalidKind = TAK_MAX+1;
    TA_init(&assertion,invalidKind);

    ASSERT_INT_NOT_EQUAL(tg,NO_ESTABLECER_TIPO_INVALIDO("TA_init"),TA_getKind(&assertion),invalidKind);

    TA_setKind(&assertion, invalidKind);

    ASSERT_INT_NOT_EQUAL(tg,NO_ESTABLECER_TIPO_INVALIDO("TA_setKind"),TA_getKind(&assertion),invalidKind);

}
#undef NO_ESTABLECER_TIPO_INVALIDO


static void test_assertionDirectKind(TestGroup *tg)
{
    const char *const msg="TA_getKind (primer entero) debe devolver correctamente el tipo establecido por TA_init a TAK_DIRECT (segundo entero)";
    TestAssertion assertion = {0};
    TA_init(&assertion,TAK_DIRECT);

    ASSERT_INT_EQUAL(tg,msg,TA_getKind(&assertion),TAK_DIRECT);

}
static TestDescriptor tests[]=
{
        {"Datos de aserción bool después de inicializar",test_empty_bool_assertion},
        {"Datos de aserción num después de inicializar",test_empty_num_assertion},
        {"Datos de aserción mem después de inicializar",test_empty_mem_assertion},
        {"TA_getResult retorna el resultado válido dado a TA_setResult",test_assertionResult},
        {"TA_getResult retorna resultado por defecto si se dió uno inválido a TA_setResult",test_assertionInvalidResult},
        {"Se respetan los parámetros de posición correctos dados",test_positionParameters},
        {"Si se dan parámetros de posición inválidos son reemplazados por valores por defecto",test_invalidPositionParameters},
        {"Datos de operador de aserción bool",test_boolAssertionOperator},
        {"Datos de operando de aserción bool",test_boolAssertionOperand},
        {"Datos de operador de aserción num",test_numAssertionOperator},
        {"Datos de operandos de aserción num",test_numAssertionOperand},
        {"Datos de operador de aserción mem",test_memAssertionOperator},
        {"Datos de operandos de aserción mem",test_memAssertionOperands},
        {"Establece el mensaje de aserción correctamente",test_assertionMessage},
        {"No establece un puntero nulo como mensaje de aserción, usar valor por defecto en su lugar",test_assertionNullMessage},
        {"Establece la razón correctamente",test_assertionReason},
        {"No establece un puntero nulo como razón, usar valor por defecto en su lugar",test_assertionNullReason},
        {"Debe ser posible cambiar el tipo de aserción reportado con TA_setKind",test_assertionSetKind},
        {"No debe establecer un tipo de aserción no válido, usar valor por defecto en su lugar",
        test_assertionInvalidKind},
        {"El tipo de aserción TA_DIRECT (para reportar resultados sin detalle) debe ser un tipo válido",
                test_assertionDirectKind},
};
static const int numTests = sizeof(tests)/sizeof(*tests);
/**
 * public interface
 */

void testRun_assertionInterface(TestGroup *base)
{
    static TestGroup tg;
    TG_init(&tg, "Pruebas de interfaz de reporte de resultados de aserción");
    TG_setTests(&tg, tests, numTests);

    TG_setReportPlugin(&tg, SubgrupoReporter_comoTGReporter(reporterGrupo()));

    TG_runTests(&tg);

    ASSERT_TRUE(base,"Todas las pruebas de la interfaz de reporte de resultados de aserción fueron exitosas",TG_allTestsPassed(&tg));
}
