/*
 * test_run.c
 *
 *  Created on: 20 abr. 2021
 *      Author: Fernando
 */
#include <stdio.h> /*printf*/
#include <test_run.h>
#include <test.h>
#include <subgrupo_reporter.h>

static TestDescriptor tests[]={
        {"# Pruebas base",testRun_TestGroup_base},
        {"# Pruebas de acciones antes y después",testRun_TestGroup_beforeAndAfter},
        {"# Pruebas de punteros nulos",testRun_TestGroup_nullStrings},
        {"# Pruebas de interfaz de aserción",testRun_assertionInterface},
        {"# Preubas de aserciónes",testRun_assertions},
};
static int numTests=sizeof(tests)/sizeof(*tests);

static SubgrupoReporter sgr;
static int initSgr = 0;
SubgrupoReporter *reporterGrupo(void)
{
    if (!initSgr)
        SubgrupoReporter_init(&sgr, "  | ");
    return &sgr;
}
int main(void)
{
    TestGroup base;
    TG_init(&base, "Pruebas de util_test");
    TG_setTests(&base, tests, numTests);

    TG_runTests(&base);

    return !TG_allTestsPassed(&base);
}
