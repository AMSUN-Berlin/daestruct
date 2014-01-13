/*
 * Copyright (C) 2014 uebb.tu-berlin.de.
 *
 * This file is part of daestruct
 *
 * daestruct is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * daestruct is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with daestruct. If not, see <http://www.gnu.org/licenses/>.
 */

#include <daestruct/analysis.hpp>

#include <vector>
#include <boost/numeric/ublas/matrix_sparse.hpp>

#include "lap.hpp"

using namespace boost::numeric::ublas;

namespace daestruct {
  namespace analysis {

    using namespace boost::numeric::ublas;
  
    void solveByFixedPoint(const std::vector<int>& assignment,  
			   const compressed_matrix<int>& sigma,
			   std::vector<int>& c, std::vector<int>& d) {
      bool converged = false;

      while (!converged) {
	converged = true;
	for (unsigned int j = 0; j < sigma.size1(); j++) {
	  int max = 0;
	  for (unsigned int i = 0; i < sigma.size2(); i++) {
	    const int a = sigma(i, j) + d[i];
	    max = max >= a ? max : a;
	  }
	  c[j] = max;
	}

	for (unsigned int i = 0; i < sigma.size1(); i++) {
	  const int j = assignment[i];
	  const int c2 = d[j] - sigma(i, j);
	  
	  if (d[i] != c2)
	    converged = false;
	  
	  d[i] = c2;
	}
      }
    }

    AnalysisResult InputProblem::pryceAlgorithm() const {
      compressed_matrix<int> cost(dimension, dimension, 4*dimension);

      incidence_setter setter =  [&cost] (int i, int j, int s) { cost(i,j) = s; };

      /* create cost matrix via callback */
      for(unsigned int i = 0; i < dimension; i++)
	mkSigma(i, setter);

      /* solve linear assignment problem */
      solution assignment = lap(cost);

      AnalysisResult result;
      result.c.resize(dimension);
      result.d.resize(dimension);

      /* run fix-point algorithm */
      solveByFixedPoint(assignment.rowsol, cost, result.c, result.d);

      return result;
    }
  }
}
