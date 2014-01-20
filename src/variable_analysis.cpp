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

#include <daestruct/variable_analysis.hpp>

#include "lap.hpp"

namespace daestruct {
  namespace analysis {

    AnalysisResult ChangedProblem::pryceAlgorithm() const {
      /* solve linear assignment problem */
      solution assignment = lap(sigma);

      AnalysisResult result;
      result.row_assignment = std::move(assignment.rowsol);
      result.col_assignment = std::move(assignment.colsol);
      result.c.resize(dimension);
      result.d.resize(dimension);

      /* run fix-point algorithm */
      solveByFixedPoint(result.row_assignment, sigma, result.c, result.d);

      return result;
    }

    ChangedProblem::ChangedProblem(const InputProblem& prob, const AnalysisResult& result,
				   const int dimDelta, const StructChange& delta) : 
      dimension(prob.dimension + dimDelta), sigma(prob.dimension) {
	
      pResult.row_assignment.reserve(dimension);
      pResult.col_assignment.reserve(dimension);
	
      auto rowDelta = delta.rowDiffs.begin();
      int rowOffset = 0;
      for (auto row_iter = prob.sigma.rowBegin(); row_iter != prob.sigma.rowEnd();) {
	if (rowDelta != delta.rowDiffs.end() && row_iter.index1() == rowDelta->location) {
	  switch (rowDelta->action) {
	  case REMOVE:
	    rowOffset -= 1;
	    row_iter++;
	    break;
	  case INSERT:
	    /* insert a new row */
	    pResult.row_assignment[row_iter.index1()+rowOffset] = -1; // new row is unassigned

	    /* get sigma values for new row */
	    for (auto v_i = rowDelta->sigma.begin(); v_i != rowDelta->sigma.end(); v_i++)
	      sigma.insert(row_iter.index1()+rowOffset, v_i.index(), *v_i);

	    rowOffset += 1;
	    break;
	  }
	  rowDelta++;
	} else {
	  /* insert row from original matrix */
	  auto colDelta = delta.colDiffs.begin();
	  int colOffset = 0;
	  for (auto col_iter = row_iter.begin(); col_iter != row_iter.end();)
	    if (colDelta != delta.colDiffs.end() && col_iter.index2() == colDelta->location) {
	      /* apply column diff */
	      switch (colDelta->action) {
	      case REMOVE:
		colOffset -= 1;
		col_iter++; //advance one column, this one has been removed
		break;		
	      case INSERT:
		pResult.col_assignment[colDelta->location+colOffset] = -1; // new col is unassigned
		colOffset += 1;
		break;
	      }
	      colDelta++;
	    }	else {
	      const int row = row_iter.index1() + rowOffset;
	      const int column = col_iter.index2() + colOffset;
	      /* insert column value from original matrix */
	      sigma.insert(row, column, *col_iter);
	      pResult.col_assignment[column] = row;
	      pResult.row_assignment[row] = column;
	    }	    
	  row_iter++;
	}
      }	
    }
  }
}
