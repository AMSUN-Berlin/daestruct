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

namespace daestruct {
  namespace analysis {

    using namespace std;

    typedef function<void(int,int,int)> incidence_setter;

    typedef function<void(int, incidence_setter)> incidence_callback;

    struct AnalysisResult {
      vector<int> c;
      vector<int> d;
    };

    struct InputProblem {
      unsigned int dimension;
      incidence_callback mkSigma;    
  
      AnalysisResult pryceAlgorithm() const;
    };    
  }
}

#endif
