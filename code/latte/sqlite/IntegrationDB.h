#ifndef INTEGRATIONDB_H
#define INTEGRATIONDB_H

/**
 * This database class assumes there is a database with the schema defined
 * as in the "createScript.sqlite" file in the current directory.
 */


#include "SqliteDB.h"
#include "../rational.h"

using namespace std;
//A B C D E F G H I J K L M N O P Q R S T U V W X Y Z

class IntegrationDB: public SqliteDB
{
private:
	void makeMoreIntegrationTests(int dim, int degree, int vertexCount, bool useDual, int requestedCount, int existingCount);
	vector<vector<string> > getUnusedPolynomials(int dim, int degree, int vertexCount, bool useDual);
	vector<vector<string> > getUnusedPolytopes(int dim, int degree, int vertexCount, bool useDual);

public:
	enum AlgorithemUsed {Lawrence, Triangulate};
	IntegrationDB();

	void deletePolynomial(int id);
	int doesPolynomialExist(const char * p);
	int doesPolytopeExist(const char *polymake);

	int getNumberPolynomials(int dim, int degree);
	int getNumberPolytopes(int dim, int vertexCount, bool useDuals);
	int getNumberIntegrationTest(int dim, int vertexCount, int degree, bool useDuals);

	vector<vector<string> > getRowsToIntegrate(int dim, int vertex, int degree, bool useDual, int limit);

	void insertEmptyIntegrationTest(const char* polynomialPath, int dim, int degree,
											const char* polytopePath, const char* polymakePath, int vertexCount, int simple,
											const char* dualPolytopePath, const char* dualPolymakePath, int dualVertexCount, int dualSimple);

	int insertIntegrationTest(int polynomialID, int polytopeID);
	void insertIntegrationTest(int dim, int degree, int vertexCount, int count);
	int insertPolynomial(int dim, int degree, const char*filePath) throw(SqliteDBexception);

	int insertPolytope(int dim, int vertexCount, int simple, int dualRowID, const char* latteFilePath, const char* polymakeFilePath);

	//Here, we pick the polynomial to test on from what is in the database.
	int insertPolytopeAndPickIntegrationTest(int dim, int vertexCount,     int simple    , const char * latteFile    , const char * polymakeFile
													, int dualVertexCount, int dualSimple, const char * dualLatteFile, const char * dualPolymakeFile);

	void updateIntegrationTimeAndValue(AlgorithemUsed alg, double time, RationalNTL currentValue, string previousValueStr,  string rowid);

};










#endif


