/*
 * tgreport_default.c
 *
 *  Created on: 22 abr. 2021
 *      Author: Fernando
 */

#include <tgreporter_stdout.h>
#include <subgrupo_reporter.h>


static SubgrupoReporter report = {0};
static int inicializado = 0;

TGReporter * TGReporter_stdout_getInstance(void)
{
    if (!inicializado)
        SubgrupoReporter_init(&report, "");

    return SubgrupoReporter_comoTGReporter(&report);
}
