/*
 * test.c
 *
 *  Created on: 20 abr. 2021
 *      Author: Fernando
 */


#include <test.h>
#include <string.h> /*memset*/

void TG_init(TestGroup *self)
{
	memset(self,0,sizeof(*self));
}

void TG_setTests(TestGroup *self, TestDescriptor *tests,int numTests)
{
	self->tests = tests;
	self->numTests=numTests;
}

void TG_doBeforeGroup(TestGroup *self, TG_Before beforeGroup)
{
	self->beforeGroup = beforeGroup;
}

void TG_doAfterGroup(TestGroup *self, TG_After afterGroup)
{
	self->afterGroup = afterGroup;
}
