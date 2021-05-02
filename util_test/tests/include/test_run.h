/*
 * test_run.h
 *
 *  Created on: 28 abr. 2021
 *      Author: Fernando
 */

#ifndef INCLUDE_TEST_RUN_H_
#define INCLUDE_TEST_RUN_H_
#include <test.h>
#include <subgrupo_reporter.h>

SubgrupoReporter *reporterGrupo(void);

void testRun_TestGroup_base(TestGroup *base);
void testRun_TestGroup_beforeAndAfter(TestGroup *base);
void testRun_TestGroup_nullStrings(TestGroup *base);
void testRun_assertions(TestGroup *base);
void testRun_assertionInterface(TestGroup *base);


#endif /* INCLUDE_TEST_RUN_H_ */
