/*
 * test.h
 *
 *  Created on: 20 abr. 2021
 *      Author: Fernando
 */

#ifndef INCLUDE_TEST_H_
#define INCLUDE_TEST_H_

/* Declara que existe una struct TestGroup definida luego */
struct TestGroup;

/* Punteros a función par prueba, incialización y finalización*/

typedef void (*TG_Test)(struct TestGroup*);
typedef TG_Test TG_Before;
typedef TG_Test TG_After;

/**
 * Descriptor de función de prueba.
 */
typedef struct TestDescriptor{
	const char* description;
	TG_Test testFunction;
}TestDescriptor;

typedef struct TestGroup
{
	TG_Before beforeGroup;
	TG_After afterGroup;
	TG_Before beforeTest;
	TG_After afterTest;
	TestDescriptor *tests;
	int numTests;
}TestGroup;

void TG_init(TestGroup *self);

void TG_doBeforeGroup(TestGroup *self, TG_Before beginGroup);
void TG_doAfterGroup(TestGroup *self, TG_After afterGroup);
void TG_doBeforeTest(TestGroup *self,TG_Before beforeTest);
void TG_doAfterTest(TestGroup *self,TG_After afterTest);
void TG_setTests(TestGroup *self, TestDescriptor *tests, int numTests);

#endif /* INCLUDE_TEST_H_ */
