/*
 * test_report.h
 *
 *  Created on: 22 abr. 2021
 *      Author: Fernando
 */

#ifndef INCLUDE_TGREPORT_H_
#define INCLUDE_TGREPORT_H_
#include <test.h>

struct TGReporter;

typedef struct TGReporter{
	void (*begin)(struct TGReporter *self,const char *groupName,int numTests);
	void (*test)(struct TGReporter *self,const char *testName);
	void (*pass)(struct TGReporter *self);
	void (*fail)(struct TGReporter *self,const char *message);
	void (*error)(struct TGReporter *self,const char *message);
	void (*summary)(struct TGReporter *self,const TestGroupOutcome *summary);
}TGReporter;

#define TGREPORTER_ISNULL_FNPTR(ptr)  ((void (*)(void))(ptr) == (void (*)(void)) 0)

static inline void TGR_reportBegin(TGReporter *self,const char *groupName,int numTests)
{
	const int pointerIsValid = !TGREPORTER_ISNULL_FNPTR(self->begin);

	if (pointerIsValid)
		self->begin(self,groupName,numTests);
}

static inline void TGR_reportTest(TGReporter *self,const char *testName)
{
	const int pointerIsValid = !TGREPORTER_ISNULL_FNPTR(self->test);

	if (pointerIsValid)
		self->test(self,testName);
}

static inline void TGR_reportPass(TGReporter *self)
{
	const int pointerIsValid = !TGREPORTER_ISNULL_FNPTR(self->pass);

	if (pointerIsValid)
		self->pass(self);
}

static inline void TGR_reportFail(TGReporter *self,const char *message)
{
	const int pointerIsValid = !TGREPORTER_ISNULL_FNPTR(self->fail);

	if (pointerIsValid)
		self->fail(self,message);
}

static inline void TGR_reportError(TGReporter *self,const char *message)
{
	const int pointerIsValid = !TGREPORTER_ISNULL_FNPTR(self->error);

	if (pointerIsValid)
		self->error(self,message);
}

static inline void TGR_reportSummary(TGReporter *self, const TestGroupOutcome *summary)
{
	const int pointerIsValid = !TGREPORTER_ISNULL_FNPTR(self->summary);

	if (pointerIsValid)
		self->summary(self,summary);
}

#undef TGREPORTER_ISNULL_FNPTR

#endif /* INCLUDE_TGREPORT_H_ */
