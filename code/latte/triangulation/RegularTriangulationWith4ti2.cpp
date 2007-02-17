/* RegularTriangulationWith4ti2.cpp -- Triangulate with 4ti2
	       
   Copyright 2007 Matthias Koeppe

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

#include <iostream>

// From 4ti2:
#include "BitSet.h"
#include "VectorArrayStream.h"
#include "LatticeBasis.h"
#include "RayAlgorithm.h"

// From LattE:
#include "latte_4ti2.h"
#include "dual.h"
#include "print.h"
#include "triangulation/RegularTriangulationWith4ti2.h"

using namespace _4ti2_;

listCone *
cone_from_ray_BitSet(vector<listVector *> &rays,
		     const BitSet &ray_set,
		     Vertex *vertex)
{
  listCone *c = createListCone();
  c->vertex = new Vertex(*vertex);
  vector<listVector *>::iterator i;
  int j;
  for (i = rays.begin(), j = 0; i!=rays.end(); ++i, j++) {
    if (ray_set[j]) {
      c->rays = new listVector((*i)->first, c->rays);
    }
  }
  return c;
}

listCone *
triangulate_cone_with_4ti2(listCone *cone,
			   BarvinokParameters *Parameters,
			   height_function_type height_function,
			   void *height_function_data,
			   int cone_dimension)
{
  listCone *triangulation = NULL;
  // Copy rays into an array, so we can index them.
  int num_rays = lengthListVector(cone->rays);
  vector<listVector *> rays_array = ray_array(cone);
  /* Create a matrix from the rays, with 1 extra coordinates 
     at the front for the lifting, and also slack variables.
     (4ti2 does not use a homogenization coordinate.) */
  int lifted_dim = Parameters->Number_of_Variables + 1 + 1 + num_rays;
  BitSet *rs = new BitSet(lifted_dim);
  VectorArray *matrix
    = rays_to_4ti2_VectorArray(cone->rays, Parameters->Number_of_Variables,
			       /* num_homogenization_vars: */ 1 + 1 + num_rays,
			       /* num_extra_rows: */ 1);
  /* Extra row: `vertical' ray -- This kills all upper facets.
     See Verdoolaege, Woods, Bruynooghe, Cools (2005). */
  (*matrix)[num_rays][0] = 1;
  /* Add identity matrix for the slack variables (including a slack
     variable for the extra ray). */
  {
    int i;
    for (i = 0; i<num_rays + 1; i++) {
      (*matrix)[i][i + 1] = 1;
      rs->set(i + 1);
    }
  }
  do {
    /* Compute random lifting. */
    int i;
    for (i = 0; i<num_rays; i++) {
      mpq_class height;
      height_function(height.get_mpq_t(), rays_array[i]->first, height_function_data);
      (*matrix)[i][0] = height.get_num();
    }
    /* Output of the file -- for debugging. */
    {
      std::ofstream file("lifted_cone_for_4ti2_triangulation");
      file << matrix->get_number() << " " << lifted_dim << endl;
      print(file, *matrix, 0, lifted_dim);
    }

    VectorArray *facets = new VectorArray(0, matrix->get_size());
    lattice_basis(*matrix, *facets);
    VectorArray* subspace = new VectorArray(0, matrix->get_size());
    RayAlgorithm algorithm;
    algorithm.compute(*matrix, *facets, *subspace, *rs);

    {
      std::ofstream file("4ti2_triangulation_output");
      file << facets->get_number() << " " << lifted_dim << "\n";
      print(file, *facets, 0, lifted_dim);
    }

    /* Walk through all facets.  (Ignore all equalities in *subspace.)
       */
    int num_facets = facets->get_number();
    BitSet incidence(num_rays);
    for (i = 0; i<num_facets; i++) {
      /* We ignore facets that are incident with the extra vertical
	 ray.  */
      if ((*facets)[i][0] != 0) {
	/* All other facets give a face of the triangulation. */
	/* Find incident rays.
	   They are the rays whose corresponding slack variables are
	   zero. */
	incidence.zero();
	int j;
	for (j = 0; j<num_rays; j++) {
	  if ((*facets)[i][j + 1] == 0) {
	    /* Incident! */
	    incidence.set(j);
	  }
	}
	/* Create the cone from the incidence information. */
	listCone *c = cone_from_ray_BitSet(rays_array, incidence, cone->vertex);
	/* Is a cone of the triangulation -- check it is simplicial */
	int c_num_rays = incidence.count();
	if (c_num_rays > cone_dimension) {
	  cerr << "Found non-simplicial cone (" << c_num_rays << "rays) "
	       << "in purported triangulation, triangulating it recursively." << endl;
	  /* In the refinement step, always fall back to using a
	     random height vector. */
	  listCone *ct = triangulate_cone_with_4ti2(c, Parameters,
						    random_height, NULL,
						    cone_dimension);
	  freeCone(c);
	  triangulation = appendListCones(ct, triangulation);
	}
	else if (c_num_rays < cone_dimension) {
	  cerr << "Lower-dimensional cone in purported triangulation, should not happen."
	       << endl;
	  abort();
	}
	else {

	  //computeDetAndFacetsOfSimplicialCone(c, Parameters->Number_of_Variables);
	  //printCone(c, Parameters->Number_of_Variables);

	  c->rest = triangulation;
	  triangulation = c;
	}
      }
    }
    delete facets;
    delete subspace;
    delete matrix;
    delete rs;
    return triangulation;
  NEW_LIFTING:
    delete facets;
    delete subspace;
  } while (1);
}

listCone *
random_regular_triangulation_with_4ti2(listCone *cone,
					 BarvinokParameters *Parameters)
{
  return triangulate_cone_with_4ti2(cone, Parameters, random_height, NULL,
				    Parameters->Number_of_Variables);
}

listCone *
refined_delone_triangulation_with_4ti2(listCone *cone,
					 BarvinokParameters *Parameters)
{
  return triangulate_cone_with_4ti2(cone, Parameters, delone_height, NULL,
				    Parameters->Number_of_Variables);
}
