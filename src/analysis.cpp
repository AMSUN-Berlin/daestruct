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

    sigma_matrix copy_defrag_noninflated(const sigma_matrix& compressed, const solution& comp_assignment,
					 AnalysisResult& result, const compression& c) {

      sigma_matrix inflated ( result.row_assignment.size() );
      int si = 0;
      
      for (auto row_iter = compressed.rowBegin(); row_iter != compressed.rowEnd(); row_iter++) {
	const int i = row_iter.index1();
	if (i != c.instances[si].s) {
	  /* copy row */
	  for (auto col_iter = row_iter.begin(); col_iter != row_iter.end(); col_iter++)
	    inflated.insert(si, col_iter.index2(), *col_iter);
	  
	  /* copy assignment */
	  result.row_assignment[i - si] = comp_assignment.rowsol[i];
	  result.col_assignment[comp_assignment.rowsol[i]] = i - si;
	} else {
	  si++;
	}
      }

      return inflated;
    }

    void inflate(sigma_matrix& inflated, const solution& comp_assignment, AnalysisResult& result, const compression& c) {
      int row_offset = comp_assignment.rowsol.size() - c.instances.size();
      int col_offset = comp_assignment.colsol.size();

      /* add private variables and equations */
      for (const compressible_instance& inst : c.instances) {
	/* which public variable does this component solve ? */
	const int k = comp_assignment.rowsol[inst.s] - inst.q;
	for (auto row_iter = inst.c->sigma.rowBegin(); row_iter != inst.c->sigma.rowEnd(); row_iter++)
	  for (auto col_iter = row_iter.begin(); col_iter != row_iter.end(); col_iter++) {
	    const int i = row_iter.index1() + row_offset;
	    const int j = 	    
	      (col_iter.index2() > inst.c->q) ?  // private variable
	      col_iter.index2() + col_offset
	      : 
	      col_iter.index2() + inst.q; //public var
	    ;

	    inflated.insert(i, j, *col_iter);

	    /* get the required matching */
	    const std::vector<int>& M = inst.c->M[k];

	    /* inflate matching as well */
	    const int i_m = M[row_iter.index1()];
	    if (i_m > inst.c->q) {
	      result.row_assignment[i] = i_m + col_offset;
	      result.col_assignment[i_m + col_offset] = i;
	    } else {
	      result.row_assignment[i] = i_m + inst.q;
	      result.col_assignment[i_m + inst.q] = i;
	    }
	  }

	/* book keeping */
	result.inflated.component_cols.push_back(col_offset);
	result.inflated.component_rows.push_back(row_offset);

	row_offset += inst.c->p + 1; //advance equations
	col_offset += inst.c->p; // advance variables
      }
    }

    AnalysisResult InputProblem::pryceCompressed(const compression& c) const {
      /* solve linear assignment problem */
      solution comp_assignment = lap(sigma);
      AnalysisResult result;
      result.row_assignment.resize(dimension + c.variables());
      result.col_assignment.resize(dimension + c.variables());
      
      sigma_matrix inflated = copy_defrag_noninflated(sigma, comp_assignment, result, c);

      inflate(inflated, comp_assignment, result, c);

      result.c.resize(dimension + c.variables());
      result.d.resize(dimension + c.variables());
      
      solveByFixedPoint(result.row_assignment, inflated, result.c, result.d);
      
      return result;
    }
          
    int AnalysisResult::extracted_equation(int eq, int k) const {
      return inflated.component_rows[k] + eq;
    }

    int AnalysisResult::extracted_variable(int var, int k) const {
      return inflated.component_cols[k] + var;
    }
    
    /**
     * Seal this sub-component (i.e. afterwards set_incidence is a no-op)
     */
    void compressible::seal() {
      if (M.size() == 0) {
	for (int j = p; j < p+q; j++) {
	  /* prepare identity matrix for the remaining rows and public vars */
	  for (int i = p+1; i < p+q; i++)
	    for (int j2 = p; j2 < p+q; j2++)
	      if (j2 != j)
		sigma.insert(i,j2,0);

	  solution sol = lap(sigma);
	  std::vector<int> M_i;
	  M_i.reserve(p+1);
	  for (int i = 0; i < p+1; i++)
	    M_i[i] = sol.rowsol[i];
	  M.push_back(M_i);
	  cost.push_back(sol.cost);

	  /* reset the identity matrix for the next run */
	  for (int i = p+1; i < p+q; i++)
	    for (int j2 = 0; j2 < q; j2++)
	      if (j2 != j)
		sigma.insert(i,j2,BIG);
	}	
      }
    }

    /**
     * Set the incidence in this sub component
     * Note: i < p + 1 !
     */
    void compressible::set_incidence(int i, int j, int val) {
      if (M.size() == 0) {
	sigma.insert(i, j, val);
      }
    }

    void compressible_instance::insert_incidence(sigma_matrix& sigma) const {
      c->seal();
      for (int j = 0; j < c->q; j++)
	sigma.insert(s, j + q, c->cost[j]);
    }


    int compression::variables() const {
      int vs = 0;
      for (const auto& inst : instances) 
	vs += inst.c->p;
      return vs;
    }

  }
}
