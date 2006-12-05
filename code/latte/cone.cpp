/* cone.cpp -- Linked list of cones

   Copyright 2002-2004 Jesus A. De Loera, David Haws, Raymond
      Hemmecke, Peter Huggins, Jeremy Tauzer, Ruriko Yoshida
   Copyright 2006 Matthias Koeppe

   This file is part of LattE.
   
   LattE is free software; you can redistribute it and/or modify it
   under the terms of the version 2 of the GNU General Public License
   as published by the Free Software Foundation.

   LattE is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with LattE; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
*/

#include <stdlib.h>
#include "ramon.h"

/* ----------------------------------------------------------------- */
int lengthListVector(listVector* LIST) {
  int len=0;

  while (LIST) {len++; LIST = LIST->rest;}
  return (len);
}

/* ----------------------------------------------------------------- */
listVector* appendVectorToListVector(const vec_ZZ &v, listVector *REST) {
  // listVector *LIST;
  listVector * LIST = new listVector(v);  
  LIST->rest = REST;
  return (LIST);
}

/* ----------------------------------------------------------------- */
listVector *copyListVector(listVector *l)
{
  listVector *result = NULL;
  listVector **rest_p = &result;
  while (l != NULL) {
    *rest_p = new listVector(l->first);
    rest_p = &(*rest_p)->rest;
    l = l->rest;
  }
  return result;
}

/* ----------------------------------------------------------------- */
listCone* createListCone() {
  listCone* z;

  z = new listCone;
  if (z==0) {
    cerr << "Memory exhausted" << endl;
    exit(1);
  }

  z->coefficient=1;
  z->vertex=0;
  z->rays=0;
  z->facets=0;
  z->determinant = 0;
  z->latticePoints=0;
  z->rest=0;

  return (z);
}
/* ----------------------------------------------------------------- */
int lengthListCone(listCone* LIST) {
  int len=0;

  while (LIST) {len++; LIST = LIST->rest;}
  return (len);
}
/* ----------------------------------------------------------------- */
void freeCone(listCone *cone)
{
  delete cone->vertex;
  freeListVector(cone->rays);
  freeListVector(cone->facets);
  freeListVector(cone->latticePoints);
  delete cone;
}

void freeListCone(listCone *list)
{
  while (list != NULL) {
    listCone *rest = list->rest;
    freeCone(list);
    list = rest;
  }
}

listCone *appendListCones(listCone *A, listCone *B)
{
  if (A == NULL) return B;
  else {
    listCone *C;
    for (C = A; C->rest!=NULL; C = C->rest);
    C->rest = B;
    return A;
  }
}

listCone *copyCone(listCone *cone)
{
  listCone *copy = createListCone();
  copy->coefficient = cone->coefficient;
  copy->vertex = new Vertex(*cone->vertex);
  copy->determinant = cone->determinant;
  copy->rays = copyListVector(cone->rays);
  copy->dual_determinant = cone->determinant;
  copy->facets = copyListVector(cone->facets);
  copy->facet_divisors = cone->facet_divisors;
  copy->latticePoints = copyListVector(cone->latticePoints);
  copy->lattice_points_scalar_products = cone->lattice_points_scalar_products;
  copy->rest = NULL;
  return copy;
}
