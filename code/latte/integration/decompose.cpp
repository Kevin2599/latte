#define COEFF_MAX 10000

#include "PolyRep.h"

#include <iostream>
#include <fstream>
#include <NTL/ZZ.h>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 3) { cout << "Usage: ./integrate fileIn fileOut" << endl; return 1; }
	string line;
	polynomial myPoly;
	linearPoly lForm;
	ifstream myStream (argv[1]);
	ofstream outStream(argv[2]);
	if (!myStream.is_open()) { cout << "Error opening file " << argv[1] << ", please make sure it is spelled correctly." << endl; return 1; }
	//while (!myStream.eof())
	//{
		myStream >> line;
		cout << "Line is `" << line << "'" << endl;
		loadPolynomial(myPoly, line);
		cout << "The following polynomial has " << myPoly.termCount << " terms containing " << myPoly.varCount << " variables: " << endl;
		cout << printPolynomial(myPoly) << endl;
		
		lForm.termCount = 0;
		lForm.varCount = myPoly.varCount;
		
		cout << "Decomposing";
		for (int i = 0; i < myPoly.termCount; i++)
		{
			cout << ".";
			decompose(myPoly, lForm, i);
		}
		cout << endl;
		cout << "About to print linear form to file" << endl;
		//outStream << printForm(lForm) << endl; //print to output file
		
		//cout << "Maple expression is: " << endl;
		outStream << printMapleForm(lForm) << endl; //let's print this instead
		destroyForm(lForm);
		destroyPolynomial(myPoly);
	//}

	myStream.close();
	outStream.close();
	return 0; 
}
