/*
 * tgreporter_stub.c
 *
 *  Created on: 22 abr. 2021
 *      Author: Fernando
 */

#include <tgreporter_stub.h>

static void begin(struct TGReporter *self,const char *groupName,int numTests)
{
	(void)self;
	(void)groupName;
	(void)numTests;
}
static void test(struct TGReporter *self,const char *testName)
{
	(void)self;
	(void)testName;
}
static void pass(struct TGReporter *self)
{
	(void)self;
}
static void fail(struct TGReporter *self,const char *message)
{
	(void)self;
	(void)message;
}
static void error(struct TGReporter *self,const char *message)
{
	(void)self;
	(void)message;
}
static void summary(struct TGReporter *self,const TestGroupOutcome *summary)
{
	(void)self;
	(void)summary;
}

static TGReporter reporter;

TGReporter * TGReporter_stub_getInstance(void)
{
	reporter.begin=begin;
	reporter.test=test;
	reporter.pass=pass;
	reporter.fail=fail;
	reporter.error=error;
	reporter.summary=summary;
	return &reporter;
}
