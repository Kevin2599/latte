/* ----------------------------------------------------------------- */
/*                                                                   */
/* LattE (Lattice Point Enumeration)                                 */
/*                                                                   */
/* Dualizing simplicial cones                                        */
/*                                                                   */
/* Author     : Raymond Hemmecke, Ruriko Yoshida                     */
/*                                                                   */
/* Created    : 14-OCT-02                                            */
/* Last Update: 05-DEC-02                                            */
/*                                                                   */
/* ----------------------------------------------------------------- */

/*  Rudy Yoshid edited a dualiz back function                        */
#include "config.h"
#include "myheader.h"
#include "cone.h"
#include "ConeDeterminant.h"
#include "print.h"
#include "ramon.h"
#include "rational.h"
#include "vertices/cdd.h"
#include "barvinok/Cone.h"
#include "barvinok/ConeDecom.h"
#include "barvinok/dec.h"
#include <list>
using namespace std;

/* ----------------------------------------------------------------- */
//  vector computeFacet(vector *mat, int numOfVars) {
//    /* I assume that mat has numOfVars+1 rows and numOfVars columns. 
//       It corresponds to the matrix formed by the extreme rays collected as
//       columns. 
//       Note that I permute the entries in mat! */
//    int sign,i;
//    vector facet,tmp;
//    vector *localMat;

//  /*  printf("Computing normal vector...\n"); */

//    localMat=createArrayVector(numOfVars+1);
//    for (i=0; i<numOfVars+1; i++) localMat[i]=copyVector(mat[i],numOfVars);

//    facet=createVector(numOfVars+1);
//    sign=1;
//    /* Note that there the sign changes twice. (computing the overall
//       facet, switching rows) 
//       Thus, the overall sign is always +.*/

//    for (i=0; i<numOfVars+1; i++) {
//  /*      printVector(localMat[i],numOfVars); */
//      tmp=localMat[i];
//      localMat[i]=localMat[numOfVars];
//      localMat[numOfVars]=tmp;
//      facet[i]=computeDeterminant(localMat,numOfVars);
//    }

//    printf("Facet: ");
//    printVector(facet,numOfVars+1);

//    for (i=0; i<numOfVars+1; i++) free(localMat[i]);
//    free(localMat);

//    return (facet);
//  }
/* ----------------------------------------------------------------- */
//  listVector* dualizeSimplicialCone(listVector* rays, int numOfVars) {
//    /* I assume that the cone is simplicial! */
//    int i,j;
//    vector tmp, facet;
//    vector *mat, *mat2;
//    listVector *dual, *endDual;

//  printf("Starting dualizing step...\n");
//  printListVector(rays,numOfVars);
//  printf("\n");
//    mat=createArrayVector(numOfVars);
//    for (i=0;i<numOfVars;i++)
//      mat[i]=createVector(numOfVars);
//    mat2=createArrayVector(numOfVars);
//    mat2=transformListVectorToArrayVector(rays, mat2);

//    for (i=0;i<numOfVars;i++)
//      for (j=0;j<numOfVars;j++)
//        mat[i][j]=mat2[j][i];
//    free(mat2);

//  printf("mat\n");
//  for (j=0; j<numOfVars; j++)
//    printVector(mat[j],numOfVars);

//    /* Now the rays are given in the matrix mat. */

//    dual=createListVector(0);
//    endDual=dual;

//    tmp=createVector(numOfVars);
//    for (i=0; i<numOfVars; i++) {
//  printf("tmp\n");
//  printVector(tmp,numOfVars);
//      for (j=0; j<numOfVars; j++) {
//        tmp[j]=mat[j][i];
//        mat[j][i]=mat[j][numOfVars-1];
//        mat[j][numOfVars-1]=tmp[j];
//      }
//  printf("i = %d\n",i);
//  for (j=0; j<numOfVars; j++)
//    printVector(mat[j],numOfVars);

//      facet=computeFacet(mat,numOfVars-1);

//      /* We still have to test whether to use facet or
//         -facet as normal vector. */

//      endDual->rest=createListVector(facet);
//      endDual=endDual->rest;
//    }
//  /*  printf("done.\n"); */

//    return (dual->rest);
//  }
/* ----------------------------------------------------------------- */
listVector* transformArrayBigVectorToListVector2(mat_ZZ A, int numOfVectors,
						int numOfVars) {
  int i;
  vec_ZZ v;
  listVector *L, *endL;

  v=createVector(numOfVars);
  L=createListVector(v);
  endL=L;

  for (i=0; i<numOfVectors; i++) {
    v=A[i];
    endL->rest = createListVector(v);
    endL = endL->rest;
  }

  return (L->rest);
}
/* ----------------------------------------------------------------- */
mat_ZZ createConeDecMatrix2(listVector *cones, int numOfRays, int numOfVars) {
  int i;
  mat_ZZ mat;
  listVector *tmp;

  mat.SetDims(numOfRays, numOfVars);

  tmp=cones;
  for (i=0; i<numOfRays; i++) {
    mat[i]=copyVector(tmp->first,numOfVars);
    tmp=tmp->rest;
  }
  removeListVector(cones);
  return (mat);
}
/* ----------------------------------------------------------------- */
listCone* transformRudyListConeIntoRamonListCone2(list< Cone > RudyCones,
						 int numOfVars) {
  int s;
  listCone *cones, *endCones, *newCone;
  Cone tmp;

  cones=createListCone();
  endCones=cones;

  while (!RudyCones.empty()) {
    newCone=createListCone();

    tmp = RudyCones.back();
    RudyCones.pop_back();
    s = tmp.sign;
    if (s==0) s=-1;

    newCone->coefficient=s;
    newCone->rays = transformArrayBigVectorToListVector(tmp.generator,
							numOfVars,numOfVars);
    endCones->rest=newCone;
    endCones=endCones->rest;
  }
//    printListCone(cones->rest,numOfVars);
  return (cones->rest);
}

/* ----------------------------------------------------------------- */
/*list< Cone > transformRamonListConeIntoRudyListCone(listCone* cones,
						 int numOfVars) {
  int s;
  list< Cone > RudyCones;
  listCone *tmpcones, *endCones, *newCone;
  Cone tmp;
  listVector *rays;

  tmpcones = cones;
  //cones=createListCone();
  //endCones=cones;

  while (tmpcones) {
    //newCone=createListCone();
    if(tmpcones->coefficient == -1)
    tmp.sign = 0;
    else tmp.sign = 1;
    rays = tmpcones->rays;
    tmp.generator.SetDims(numOfVars, numOfVars);
    for(int i = 0; i < numOfVars; i++){
      tmp.generator[i] = rays->first;
      rays->rest;
      }
    tmpcones = tmpcones -> rest;
    RudyCones.push_back(tmp);
  }
//    printListCone(cones->rest,numOfVars);
  return (RudyCones);
}        */
listCone* dualizeCones(listCone *cones, int numOfVars) {
  int i,j,tmpInt,len,numOfVertices,numOfConesDualized,numOfAllCones;
  ZZ x,y;
  rationalVector *w;
  listVector *rays, *rays2, *facets, *endFacets;
  listCone *tmp;
  char cddInFileName[127];
  string tmpString;

  printf("Dualizing all cones.\n");

  numOfConesDualized=0;
  numOfAllCones=lengthListCone(cones);

  tmp=cones;
  while (tmp) {
    rays=tmp->rays;
    rays2=rays;
    len=lengthListVector(rays);
/*      if (len!=numOfVars) { */
/*      if (1>0) { */

      strcpy(cddInFileName,"latte_cdd.ine");

      ofstream out(cddInFileName);
      if(!out){
	cerr << "Cannot open output file in dualizeCones." << endl;
	exit(1);
      }

      out << "H-representation\n";
      out << "begin\n";
      out << lengthListVector(rays) << " " << numOfVars+1 << "integer" << endl;

      while (rays) {
	out << "0 ";
	for (i=0; i<(numOfVars); i++) out << -(rays->first)[i] << " ";
	out << endl;
	rays=rays->rest;
      }
      out << "end\n";
      out.close();

/*      printf("Computing facets with cdd..."); */
      system(CDD_PATH " latte_cdd.ine > latte_cdd.out");
/*      printf("done.\n"); */

      strcpy(cddInFileName,"latte_cdd.ext");

      ifstream in(cddInFileName);
      if(!in){
	cerr << "Cannot open input file in dualizeCones." << endl;
	exit(1);
      }

      while (tmpString!="begin") getline(in,tmpString);

      in >> numOfVertices >> tmpInt >> tmpString;

      facets=createListVector(createVector(numOfVars));
      endFacets=facets;
      
      for (i=0; i<numOfVertices; i++) {
	w=createRationalVector(numOfVars);
	for (j=0; j<numOfVars+1; j++) {
	  x=0;
	  y=0;
	  ReadCDD(in,x,y);
	  if (j>0) {
	    w->enumerator[j-1]=x;
	    w->denominator[j-1]=y;
	  }
	}
	w=normalizeRationalVector(w,numOfVars);
	endFacets->rest=createListVector(w->enumerator);
	endFacets=endFacets->rest;
      }
      in.close();
      system("rm latte_cdd.*");
      tmp->facets=tmp->rays;    
      tmp->rays=facets->rest;
/*      } else { */
/*        tmp->facets=tmp->rays;     */
/*        tmp->rays=dualizeSimplicialCone(rays,numOfVars); */

/*      if (len==numOfVars) { */
//      if (1<0) {
//        printf("cdd\n");
//        printListVector(tmp->rays,numOfVars);
//        printf("dualize\n");
//        printListVector(dualizeSimplicialCone(rays2,numOfVars),numOfVars);
//      }

    tmp=tmp->rest;
    numOfConesDualized++;
    if (numOfConesDualized==50*(numOfConesDualized/50)) {
/*        exit(0); */
      printf("%d / %d done.\n",numOfConesDualized,numOfAllCones);
    }
  }

  printf("All cones are now dualized.\n");
   removeListVector(cones->facets);
  return (cones);
}
/* ----------------------------------------------------------------- */

listCone* dualizeBackCones(listCone *cones, int numOfVars) 
{
  int i,len,numOfConesDualized,numOfAllCones;
  ZZ x,y;
 // rationalVector *w;
  listVector *rays, *rays2;
  listCone *tmp;
//  char cddInFileName[127];
 // string tmpString;

  mat_ZZ Inverse;
  Inverse.SetDims(numOfVars, numOfVars);
  numOfConesDualized=0;
  numOfAllCones=lengthListCone(cones);

  tmp=cones;
  while (tmp) {
    rays=tmp->rays;
    rays2=rays;
    len=lengthListVector(rays);
/*      if (len!=numOfVars) { */
/*      if (1>0) { */

      for(i = 0; i < numOfVars; i++)
      { Inverse[i] = rays->first;
        rays = rays -> rest;}
     //cout << Inverse << endl; exit(9);
      Inverse = -transpose(inv(Inverse));
       for(i = 0; i < numOfVars; i++)
      {  rays2->first = Inverse[i];
        rays2 = rays2 -> rest;}
   //   cout << Inverse << endl; exit(9);
     // tmp->facets=tmp->rays;
     // tmp->rays=facets->rest;
/*      } else { */
/*        tmp->facets=tmp->rays;     */
/*        tmp->rays=dualizeSimplicialCone(rays,numOfVars); */

/*      if (len==numOfVars) { */
//      if (1<0) {
//        printf("cdd\n");
//        printListVector(tmp->rays,numOfVars);
//        printf("dualize\n");
//        printListVector(dualizeSimplicialCone(rays2,numOfVars),numOfVars);
//      }

    tmp=tmp->rest;
    numOfConesDualized++;
    if (numOfConesDualized==50*(numOfConesDualized/50)) {
/*        exit(0); */
      printf("%d / %d done.\n",numOfConesDualized,numOfAllCones);
    }
  }

  // removeListVector(cones->facets);
  return (cones);
}
/* ----------------------------------------------------------------- */

