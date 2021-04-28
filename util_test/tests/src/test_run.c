/*
 * test_run.c
 *
 *  Created on: 20 abr. 2021
 *      Author: Fernando
 */
#include <stdio.h> /*printf*/
#include <test_run.h>
int main(void)
{
	int someTestFailed = testRun_TestGroup_base();
	someTestFailed |= testRun_TestGroup_beforeAndAfter();
	someTestFailed |= testRun_TestGroup_nullStrings();
	someTestFailed |= testRun_assertions();
	someTestFailed |= testRun_assertionInterface();
	if (someTestFailed)
	    printf("\n-----------------\nTHERE WERE ERRORS\n-----------------\n");
	return !!someTestFailed;
}
