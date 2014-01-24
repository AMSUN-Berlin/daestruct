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

#include "lap.hpp"
#include "prettyprint.hpp"
#include <iostream>

namespace daestruct {
  namespace analysis {
  
    void solveByFixedPoint(const std::vector<int>& assignment,  
			   const sigma_matrix& sigma,
			   std::vector<int>& c, std::vector<int>& d) {
      bool converged = false;

      while (!converged) {
	converged = true;
	
	for (auto row_iter = sigma.rowBegin(); row_iter != sigma.rowEnd(); row_iter++) {	      
	  const int i = row_iter.index1();

	  for (auto col_iter = row_iter.begin(); col_iter != row_iter.end(); col_iter++) {
	    const int j = col_iter.index2();
	    const int a = -1 * *col_iter + c[i];
	    if (a > d[j]) {
	      //std::cout << "d[" << j << "] = " << a << "(max row " << i << " = " << a << " > " << d[j] << ")" << std::endl; 
	      d[j] = a;
	    }
	  }
	}

	for (unsigned int i = 0; i < sigma.dimension; i++) {
	  const int j = assignment[i];
	  const int c2 = d[j] + sigma(i, j);
	  
	  if (c[i] != c2) {
	    converged = false;
	  }
	  
	  //std::cout << "c[" << i << "] = " << c2 << "(because d[" << j << "] = " << d[j] << ")" << std::endl;

	  c[i] = c2;
	  /*
	  for (unsigned int j = 0; j < sigma.dimension; j++)
	    if (d[j] < c[i] - sigma(i,j))
	      std::cout << "Violated dual in fix-point: " << "c[" << i << "] = " << c[i] << ", d[" << j << "] = " << d[j] << std::endl;
	  */
	}
      }
    }

    AnalysisResult InputProblem::pryceAlgorithm() const {
      //std::cout << sigma << std::endl;

      /* solve linear assignment problem */
      solution assignment = lap(sigma);

      std::cout << "lap solved: " << assignment.cost << std::endl;
      AnalysisResult result;
      result.row_assignment = std::move(assignment.rowsol);
      result.col_assignment = std::move(assignment.colsol);
      result.c.resize(dimension);
      result.d.resize(dimension);

      /*
      std::cout << assignment.v << std::endl;
      std::cout << assignment.u << std::endl;
      */

      /*
      for (int i = 0; i < dimension; i++) {
	const int j = result.row_assignment[i];
	if (assignment.u[i] - assignment.v[j] < sigma(i,j))
	  std::cout << i << ", " << j << " violates dual " << std::endl;
      }
      */

      /* run fix-point algorithm */
      std::cout << "Calculating smallest dual" << std::endl;
      solveByFixedPoint(result.row_assignment, sigma, result.c, result.d);
      //std::cout << "Canonical: c=" << result.c << " d=" << result.d << std::endl;

      return result;
    }
  }
}
