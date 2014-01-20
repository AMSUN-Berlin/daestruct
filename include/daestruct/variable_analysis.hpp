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

#include <daestruct/sigma_matrix.hpp>
#include <daestruct/analysis.hpp>

namespace daestruct {
  namespace analysis {

    using namespace std;

    enum action { REMOVE, INSERT };

    /**
     * remove some rows or columns 
     */
    struct ColDiff {
      action action;
      int location;
      int count;
    };

    /**
     * insert a row or column 
     */
    struct RowDiff {
      action action;
      int location;
      compressed_vector<int> sigma;
    };

    struct StructChange {
      std::vector<RowDiff> rowDiffs;          
      std::vector<ColDiff> colDiffs;          
    };

    struct ChangedProblem {
      long dimension;
      sigma_matrix sigma;
      AnalysisResult pResult;

      ChangedProblem(const InputProblem& prob, const AnalysisResult& result,
		     const int dimDelta, const StructChange& delta);

      AnalysisResult pryceAlgorithm() const;
    };

    
  }
}

#endif
