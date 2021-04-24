/*
 * test_run.c
 *
 *  Created on: 20 abr. 2021
 *      Author: Fernando
 */

int testRun_TestGroup_base(void);
int testRun_TestGroup_beforeAndAfter(void);
int testRun_TestGroup_nullStrings(void);
int testRun_assertions(void);
int main(void)
{
	int someTestFailed = testRun_TestGroup_base();
	someTestFailed |= testRun_TestGroup_beforeAndAfter();
	someTestFailed |= testRun_TestGroup_nullStrings();
	someTestFailed |= testRun_assertions();

	return !!someTestFailed;
}
