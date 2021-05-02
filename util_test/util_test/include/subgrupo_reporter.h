/*
 * tgreporter_subgrupo.h
 *
 *  Created on: 2 may. 2021
 *      Author: Fernando
 */

#ifndef INCLUDE_SUBGRUPO_REPORTER_H_
#define INCLUDE_SUBGRUPO_REPORTER_H_

#include <tgreporter.h>

typedef struct SubgrupoReporter SubgrupoReporter;
struct SubgrupoReporter{
    TGReporter super;
    const char *prefijo;
    const SubgrupoReporter *base;
};

void SubgrupoReporter_init(SubgrupoReporter *self, const char *prefijo);
void SubgrupoReporter_init_conBase(SubgrupoReporter *self,const SubgrupoReporter *base,const char *prefijo);
TGReporter *SubgrupoReporter_comoTGReporter(SubgrupoReporter *self);


#endif /* INCLUDE_SUBGRUPO_REPORTER_H_ */
