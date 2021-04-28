/*
 * test_report.h
 *
 *  Created on: 22 abr. 2021
 *      Author: Fernando
 */

#ifndef INCLUDE_TGREPORT_H_
#define INCLUDE_TGREPORT_H_
#include <test.h>
#include <iassertion.h>
struct TGReporter;

typedef void (*TGReportBegin)(struct TGReporter *self,const char *groupName,int numTests);
typedef void (*TGReportTest)(struct TGReporter *self,const char *testName);
typedef void (*TGReportPass)(struct TGReporter *self);
typedef void (*TGReportFail)(struct TGReporter *self,const TestAssertion *ta);
typedef void (*TGReportError)(struct TGReporter *self,const TestAssertion *ta);
typedef void (*TGReportSummary)(struct TGReporter *self,const TestGroupOutcome *summary);

typedef struct TGReporter{
	TGReportBegin begin;
	TGReportTest test;
	TGReportPass pass;
	TGReportFail fail;
	TGReportError error;
	TGReportSummary summary;
}TGReporter;

static inline void TGR_reportBegin(TGReporter *self,const char *groupName,int numTests)
{
	const int pointerIsValid = (TGReportBegin)0 != (self->begin);

	if (pointerIsValid)
		self->begin(self,groupName,numTests);
}

static inline void TGR_reportTest(TGReporter *self,const char *testName)
{
	const int pointerIsValid = (TGReportTest)0 != self->test;

	if (pointerIsValid)
		self->test(self,testName);
}

static inline void TGR_reportPass(TGReporter *self)
{
	const int pointerIsValid = (TGReportPass) 0 != self->pass;

	if (pointerIsValid)
		self->pass(self);
}

static inline void TGR_reportFail(TGReporter *self,const TestAssertion *ta)
{
	const int pointerIsValid = (TGReportFail)0 != self->fail;

	if (pointerIsValid)
		self->fail(self,ta);
}

static inline void TGR_reportError(TGReporter *self,const TestAssertion *ta)
{
	const int pointerIsValid = (TGReportError)0 != self->error;

	if (pointerIsValid)
		self->error(self,ta);
}

static inline void TGR_reportSummary(TGReporter *self, const TestGroupOutcome *summary)
{
	const int pointerIsValid = (TGReportSummary)0 != self->summary;

	if (pointerIsValid)
		self->summary(self,summary);
}

#undef TGREPORTER_ISNULL_FNPTR

#endif /* INCLUDE_TGREPORT_H_ */
