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
#include <boost/timer/timer.hpp>
#include <prettyprint.hpp>
#include "lap.hpp"

namespace daestruct {
  namespace analysis {

using namespace boost::icl;

    AnalysisResult ChangedProblem::pryceAlgorithm() const {
      /* solve linear assignment problem */
      solution assignment = delta_lap(sigma, dual_rows, dual_columns, row_assignment, col_assignment);

      AnalysisResult result;
      result.c.resize(dimension);
      result.d.resize(dimension);
      result.row_assignment = std::move(assignment.rowsol);
      result.col_assignment = std::move(assignment.colsol);

      /* run fix-point algorithm */
      /*
      std::cout << "Delta-LAP solved: " << assignment.cost << std::endl;
      
      std::cout << result.row_assignment << std::endl;
      std::cout << result.col_assignment << std::endl;
      
      std::cout << assignment.v << std::endl;
      std::cout << assignment.u << std::endl;

      for (int i = 0; i < dimension; i++) {
	const int j = result.row_assignment[i];
	if (assignment.u[i] + assignment.v[j] > sigma(i,j))
	  std::cout << i << ", " << j << " violates dual " << std::endl;
      }

      std::cout << sigma << std::endl;
      */
      
      //std::cout << "Calculating smallest dual" << std::endl;
	  
      solveByFixedPoint(result.row_assignment, sigma, result.c, result.d);

      //std::cout << "Done fixed-point" << std::endl;
      //std::cout << "Canonical: c=" << result.c << " d=" << result.d << std::endl;
      return result;
    }

    ChangedProblem::ChangedProblem(const InputProblem& prob, const AnalysisResult& result,
				   const StructChange& delta) : 
      old_columns(prob.dimension - delta.deletedCols.size()),
      old_rows(prob.dimension - delta.deletedRows.size()),      
      dimension(prob.dimension - delta.deletedRows.size() + delta.newRows.size()),
      sigma(prob.dimension - delta.deletedRows.size() + delta.newRows.size()),
      row_changed(prob.dimension - delta.deletedRows.size() + delta.newRows.size()) {
     
      applyDiff(prob.sigma, result, delta);
    }

    ChangedProblem::ChangedProblem(const ChangedProblem& prob, const AnalysisResult& result,
				   const StructChange& delta) : 
      old_columns(prob.dimension - delta.deletedCols.size()),
      old_rows(prob.dimension - delta.deletedRows.size()),      
      dimension(prob.dimension - delta.deletedRows.size() + delta.newRows.size()),
      sigma(prob.dimension - delta.deletedRows.size() + delta.newRows.size()),
      row_changed(prob.dimension - delta.deletedRows.size() + delta.newRows.size()) {
     
      applyDiff(prob.sigma, result, delta);
    }

    void ChangedProblem::applyDiff(const sigma_matrix& oldSigma, const AnalysisResult& result, const StructChange& delta) {
      //std::cout << "ApplyDiff..." << std::endl;
      //boost::timer::auto_cpu_timer t;

      row_assignment.resize(dimension, BIG);
      col_assignment.resize(dimension, BIG);
      dual_columns.resize(dimension, BIG);
      dual_rows.resize(dimension, BIG);
      
      for (int removed : delta.deletedCols)
	colOffsets += make_pair(interval<int>::closed(removed, oldSigma.dimension()), -1);

      for (int removed : delta.deletedRows)
	rowOffsets += make_pair(interval<int>::closed(removed, oldSigma.dimension()), -1);

      for (auto row_iter = oldSigma.rows().cbegin(); row_iter != oldSigma.rows().cend(); row_iter++) {
	const int orig_row = row_iter - oldSigma.rows().cbegin();
	const int row = orig_row + rowOffsets(orig_row);

	if (delta.deletedRows.count(orig_row) == 0) {
	  dual_rows[row] = result.c[orig_row];

	  const int orig_assign_col = result.row_assignment[orig_row];

	  if (delta.deletedCols.count(orig_assign_col) == 0) {
	    const int new_assign_col = orig_assign_col + colOffsets(orig_assign_col);
	    row_assignment[row] = new_assign_col;
	    col_assignment[new_assign_col] = row;
	  } else 
	    row_assignment[row] = -1;

	  /* insert row from original matrix */
	  for (auto col_iter = (*row_iter).begin(); col_iter != (*row_iter).end(); col_iter++)
	    if (delta.deletedCols.count(col_iter.index()) == 0) {
	      const int orig_col = col_iter.index();
	      const int column = orig_col + colOffsets(orig_col);
	      /* insert column value from original matrix */
	      sigma.insert(row, column, *col_iter);
	    }
	}
      }
     
      for (size_t j = 0; j < result.d.size(); j++)
	dual_columns[j + colOffsets(j)] = -result.d.at(j);

      for (size_t i = 0; i < delta.newRows.size(); i++) {
	const NewRow& nrow = delta.newRows[i];
	//std::cout << "Adding new row " << i << " to " << (old_rows + i) << std::endl;
	for (const std::pair<int, int>& p : nrow.ex_vars) {
	  const int orig_col = get<0>(p) ;
	  const int col = orig_col + colOffsets(orig_col);
	  const int min = sigma.smallest_cost_row(col);
	  sigma.insert(old_rows+i, col, get<1>(p)); 
	  row_changed[col] = min != sigma.smallest_cost_row(col);
	}

	for (const std::pair<int, int>& p : nrow.new_vars)
	  sigma.insert(old_rows+i, old_columns + get<0>(p), get<1>(p)); 
      }
    }
  }
}
