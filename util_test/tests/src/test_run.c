/*
 * test_run.c
 *
 *  Created on: 20 abr. 2021
 *      Author: Fernando
 */

int testRun_TestGroup_base(void);
int testRun_TestGroup_aditional(void);

int main(void)
{
	const int base = testRun_TestGroup_base();
	const int aditional = testRun_TestGroup_aditional();
	const int someTestFailed = !!(base || aditional);
	return someTestFailed;
}
