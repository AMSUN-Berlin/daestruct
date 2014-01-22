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

#include <boost/icl/interval.hpp>
#include <boost/icl/interval_map.hpp>
#include "lap.hpp"

namespace daestruct {
  namespace analysis {

using namespace boost::icl;

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
				   const StructChange& delta) : 
      rest_dimension(prob.dimension - delta.deletedRows.size()),
      dimension(prob.dimension - delta.deletedRows.size() + delta.newRows.size()),
      sigma(prob.dimension - delta.deletedRows.size() + delta.newRows.size()) {
     
      applyDiff(prob.sigma, result, delta);
    }

    ChangedProblem::ChangedProblem(const ChangedProblem& prob, const AnalysisResult& result,
				   const StructChange& delta) : 
      rest_dimension(prob.dimension - delta.deletedRows.size()),
      dimension(prob.dimension - delta.deletedRows.size() + delta.newRows.size()),
      sigma(prob.dimension - delta.deletedRows.size() + delta.newRows.size()) {
     
      applyDiff(prob.sigma, result, delta);
    }

    void ChangedProblem::applyDiff(const sigma_matrix& oldSigma, const AnalysisResult& result, const StructChange& delta) {
      pResult.row_assignment.reserve(dimension);
      pResult.col_assignment.reserve(dimension);

      for (int removed : delta.deletedCols)
	colOffsets += make_pair(interval<int>::closed(removed, dimension), -1);

      for (int removed : delta.deletedRows)
	rowOffsets += make_pair(interval<int>::closed(removed, dimension), -1);

      for (auto row_iter = oldSigma.rowBegin(); row_iter != oldSigma.rowEnd(); row_iter++) {
	const int orig_row = row_iter.index1();
	const int row = orig_row + rowOffsets(orig_row);
	
	const int orig_assign_col = result.row_assignment[orig_row];
	const int new_assign_col = orig_assign_col + colOffsets(orig_assign_col);
	pResult.row_assignment[row] = new_assign_col;
	pResult.col_assignment[new_assign_col] = row;

	if (delta.deletedRows.count(row_iter.index1()) == 0) {
	  /* insert row from original matrix */
	  for (auto col_iter = row_iter.begin(); col_iter != row_iter.end(); col_iter++)
	    if (delta.deletedCols.count(col_iter.index2()) == 0) {
	      const int orig_col = col_iter.index2();
	      const int column = orig_col + colOffsets(orig_col);
	      /* insert column value from original matrix */
	      sigma.insert(row, column, *col_iter);
	    }
	}
      }
      
      for (int i = 0; i < delta.newRows.size(); i++) {
	const NewRow& nrow = delta.newRows[i];
	for (auto it = nrow.ex_vars.begin(); it != nrow.ex_vars.end(); it++)
	  sigma.insert(rest_dimension+i, it.index(), *it); 

	for (auto it = nrow.new_vars.begin(); it != nrow.new_vars.end(); it++)
	  sigma.insert(rest_dimension+i, it.index() + rest_dimension, *it); 

	pResult.col_assignment[rest_dimension+i] = -1;
	pResult.row_assignment[rest_dimension+i] = -1;
      }
    }
  }
}
