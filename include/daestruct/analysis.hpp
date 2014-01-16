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
#ifndef DAE_ANALYSIS_HPP
#define DAE_ANALYSIS_HPP

#include <vector>
#include <functional>
#include <boost/variant.hpp>

#include <daestruct/sigma_matrix.hpp>

namespace daestruct {
  namespace analysis {

    using namespace std;

    struct AnalysisResult {
      std::vector<int> c;
      std::vector<int> d;
    };

    struct InputProblem {
      long dimension;
      sigma_matrix sigma;
  
      InputProblem(long d) : dimension(d), sigma(d) {}

      AnalysisResult pryceAlgorithm() const;
    };
  }
}

#endif
