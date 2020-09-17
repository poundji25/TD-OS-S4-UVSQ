#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexc.h"

#define PRINT_ON_FAILURE(a)  { fprintf(stderr, "FAIL -- %s\n", a); return 1; }

#define CMD_BUFFER	64

/**
 * Unit test -- function lexc_add()
 *
 * @return     0 on SUCCES, 1 on FAILURE
 */
int test_unit_add() {
	struct lexc *L = lexc_init("test", 1);
	struct lexc *M = lexc_init("test-im", 0);

	assert(L);
	assert(M);

	// test on bad lexicons
	if (!lexc_add(NULL, "test"))
		PRINT_ON_FAILURE("Add to 'NULL' lexicon must fail");
	if (!lexc_add(M, "test"))
		PRINT_ON_FAILURE("Add to immutable lexicon must fail");

	// test with a bad word
	if (!lexc_add(L, NULL))
		PRINT_ON_FAILURE("Add a 'NULL' word must fail");

	// test with a good word
	if (lexc_add(L, "test"))
		PRINT_ON_FAILURE("Add to mutable lexicon must succeed");

	// test with an existent word
	if (!lexc_add(L, "test"))
		PRINT_ON_FAILURE("Duplicated add must fail");

	lexc_fini(L);
	lexc_fini(M);

	return 0;
}

/**
 * Unit test -- function lexc_remove()
 *
 * @return     0 on SUCCES, 1 on FAILURE
 */
int test_unit_remove() {
	struct lexc *L = lexc_init("test", 1);
	struct lexc *M = lexc_init("test-im", 0);

	assert(L);
	assert(M);
	assert(!lexc_add(L, "test"));

	// test on bad lexicons
	if (!lexc_remove(NULL, "test"))
		PRINT_ON_FAILURE("Remove from 'NULL' lexicon must fail");
	if (!lexc_remove(M, "test"))
		PRINT_ON_FAILURE("Remove from immutable lexicon must fail");

	// test with a bad word
	if (!lexc_remove(L, NULL))
		PRINT_ON_FAILURE("Remove a 'NULL' word must fail");

	// test with a good word
	if (lexc_remove(L, "test"))
		PRINT_ON_FAILURE("Remove from mutable lexicon must succeed");

	// test with a non-existent word
	if (!lexc_remove(L, "test"))
		PRINT_ON_FAILURE("Remove a non-existent word must fail");

	lexc_fini(L);
	lexc_fini(M);

	return 0;
}

/**
 * Unit test -- function lexc_check()
 *
 * @return     0 on SUCCES, 1 on FAILURE
 */
int test_unit_check() {
	struct lexc *L = lexc_init("test", 1);

	assert(L);
	assert(!lexc_add(L,"test"));

	// test on bad lexicon
	if (!lexc_remove(NULL, "test"))
		PRINT_ON_FAILURE("Check from 'NULL' lexicon must fail");

	// test with a bad word
	if (!lexc_remove(L, NULL))
		PRINT_ON_FAILURE("Check a 'NULL' word must fail");

	// test with a good word
	if (lexc_remove(L, "test"))
		PRINT_ON_FAILURE("Check an existent word must succeed");

	// test with a non-existent word
	if (!lexc_check(L, "no-test"))
		PRINT_ON_FAILURE("Check a non-existent word must fail");

	lexc_fini(L);

	return 0;
}

/**
 * Integration test -- add the alphabet letters in the lexicon,
 * then remove them.
 *
 * @return     0 on SUCCES, 1 on FAILURE
 */
int test_intg_scenario_alphabet() {
	struct lexc *L;
	char c[2] = "a\0";

	L = lexc_init("alpha", 1);
	if (!L)
		return 1;
		
	for (;c[0] <= 'z'; ++c[0])
		{
	
			if (lexc_add(L, c))
			{
				return 1;}
		}

	--c[0];
	for (;c[0] >= 'a'; --c[0])
		if (lexc_remove(L, c))
			{return 1;}

	lexc_fini(L);

	return 0;
}

/**
 * Integration test -- add to and remove from the lexicon
 * the same word.
 *
 * @return     0 on SUCCES, 1 on FAILURE
 */
int test_intg_scenario_do_and_redo() {
	struct lexc *L;
	int i;

	L = lexc_init("test", 1);
	if (!L)
		return 1;

	for (i = 0; i < 10; ++i) {
		if (lexc_add(L, "do"))
			return 1;
		if (lexc_remove(L, "do"))
			return 1;
	}

	lexc_fini(L);

	return 0;
}

/**
 * Executes the unit and integration tests.
 * 
 * Uncomment them once their implementation are done.
 */
void execute_test() {
	printf("[U-TEST] add: %s\n", test_unit_add() ? "FAILURE" : "SUCCESS");
	printf("[U-TEST] remove: %s\n", test_unit_remove() ? "FAILURE" : "SUCCESS");
	printf("[U-TEST] check: %s\n", test_unit_check() ? "FAILURE" : "SUCCESS");

	printf("[I-TEST] scenario-alphabet: %s\n",
		   test_intg_scenario_alphabet() ? "FAILURE" : "SUCCESS");
	printf("[I-TEST] scenario-do-and-redo: %s\n",
		   test_intg_scenario_do_and_redo() ? "FAILURE" : "SUCCESS");
}

/**
 * Executes the command interpreter until the command 'quit' is entered.
 */
void execute() {
	char buffer[CMD_BUFFER];
	struct lexc *L;

	L = lexc_init("test", 1);

	do {
		fgets(buffer, CMD_BUFFER, stdin);
	} while (cmd_interpret(L, buffer));

	lexc_fini(L);
}

/**
 * Main function.
 * 
 * You can define the TEST_LEXC variable by compiling your program
 * with the following command, using the -D gcc option:
 * $ gcc -DTEST_LEXC test_lexc.c lexc.c
 */
void main()
{
#ifdef TEST_LEXC
	execute_test();
#else
	execute();
#endif
}
