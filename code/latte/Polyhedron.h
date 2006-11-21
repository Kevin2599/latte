// This is a -*- C++ -*- header file.
   
/* Polyhedron.h -- Representation of a polyhedron.

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

#ifndef POLYHEDRON__H
#define POLYHEDRON__H

#include "cone.h"

class Polyhedron {
public:
  int numOfVars;		/* Number of variables, including
				   homogenization */
  bool homogenized;  /* If true, CONES represent the homogenization of
			the polyhedron.  Otherwise, CONES represent
			the supporting cones of all vertices of the
			polyhedron. */
  bool dualized;	       /* Whether CONES have been dualized. */
  listCone *cones; 
private:
  Polyhedron(const Polyhedron &);
public:
  Polyhedron() : cones(0), numOfVars(0), dualized(false), homogenized(false) {}
  ~Polyhedron() { freeListCone(cones); }
};

#endif