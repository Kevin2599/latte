/*
 * Driver.cpp
 *
 *  Created on: Jun 24, 2010
 *      Author: Brandon Dutra and Gregory Pinto
 *
 *
 *
 *  type --help to print the help menu.
 */

#include "valuation.h"


int main(int argc, char *argv[])
{
	Valuation::mainValuationDriver((const char **) argv, argc);
	//mainValuationDriverOLD((const char **) argv, argc); //remove the file printing later.
	//runHyperSimplexTests();
	//runBirkhoffTests();
	//runTests();
	//runOneTest(atoi(argv[1]), atoi(argv[2]));

	return 0;
}
//main()