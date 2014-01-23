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
#ifndef DAE_VARIABLE_ANALYSIS_HPP
#define DAE_VARIABLE_ANALYSIS_HPP

#include <vector>
#include <set>
#include <map>

#include <boost/icl/interval.hpp>
#include <boost/icl/interval_map.hpp>
#include <daestruct/sigma_matrix.hpp>
#include <daestruct/analysis.hpp>

namespace daestruct {
  namespace analysis {

    struct NewRow {
      map<int, int> ex_vars;
      map<int, int> new_vars;      
    };

    struct StructChange {
      int newVars;
      std::set<int> deletedRows;
      std::set<int> deletedCols;      
      std::vector<NewRow> newRows;
    };

    struct ChangedProblem {
    private:
      void applyDiff(const sigma_matrix& oldSigma, const AnalysisResult& result, const StructChange& delta);
    public:
      long rest_dimension;
      long dimension;
      sigma_matrix sigma;
      std::vector<int> row_assignment;
      std::vector<int> col_assignment;
      boost::icl::interval_map<int, int> colOffsets;
      boost::icl::interval_map<int, int> rowOffsets;

      ChangedProblem(const InputProblem& prob, const AnalysisResult& result,
		     const StructChange& delta);

      ChangedProblem(const ChangedProblem& prob, const AnalysisResult& result,
		     const StructChange& delta);

      AnalysisResult pryceAlgorithm() const;
    };

    
  }
}

#endif
