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

    void solveByFixedPoint(const std::vector<size_t>& assignment,  
			   const sigma_matrix& sigma,
			   std::vector<int>& c, std::vector<int>& d);

    struct InflatedMap {
      /* public variables and non-component equations */
      std::vector<int> cols;
      std::vector<int> rows;
      
      /* component-private variables and component-equations */
      std::vector<int> component_cols;
      std::vector<int> component_rows;
    };

    struct AnalysisResult {
      std::vector<size_t> row_assignment;
      std::vector<size_t> col_assignment;
      
      std::vector<int> c;
      std::vector<int> d;

      InflatedMap inflated;

      int extracted_equation(int eq, int k) const ;

      int extracted_variable(int var, int k) const;
    };

    /**
     * A sealed compressible sub-component S = ((P U Q) U X, D, d, {M_q})
     * Q = {0 .. q - 1}
     * P = {0 .. p - 1}
     * X = {0 .. p}
     */
    struct compressible {
      size_t p;
      size_t q;

      sigma_matrix sigma; //represents D and d as well

      typedef std::vector<std::vector<size_t>> assignment_permutations;

      /* M_q in the form of row-assignments */
      assignment_permutations M;

      /* cost of the M_q assignments */
      std::vector<int> cost;

      compressible(int pub_v, int pri_v, const sigma_matrix& s);

    };

    /**
     * A compressible sub-component builder
     */
    struct compressible_builder {
      size_t p;
      size_t q;

      //TODO: replace this with a better suited matrix, 
      //every row from p+1 .. p+q will be full in the end
      sigma_matrix sigma; //represents D and d as well

      compressible_builder(int pub_v, int pri_v) : p(pri_v), q(pub_v), sigma(p+q) {}

      /**
       * Build this sub-component (i.e. afterwards set_*_incidence is a no-op)
       */
      compressible build();

      /**
       * Set the incidence in this sub component
       * j is interpreted as a private variable
       * Note: i < p + 1 !
       */
      void set_private_incidence(int i, int j, int val);

      /**
       * Set the incidence in this sub component
       * j is interpreted as a private variable
       * Note: i < p + 1 !
       */
      void set_public_incidence(int i, int j, int val);
    };

    struct compressible_instance {
      size_t q; /* offset of the first public variable in the compressed matrix */
      size_t s; /* the surrogat equation */    
      compressible* c;

      compressible_instance(size_t qv, size_t se, compressible* comp) : q(qv), s(se), c(comp) {}

      void insert_incidence(sigma_matrix& sigma) const;
    };

    struct compression {
      int variables() const;

      std::vector<compressible_instance> instances;
    };

    struct InputProblem {
      int dimension;
      sigma_matrix sigma;
  
      InputProblem(const coordinate_matrix<der_t>& builder) : dimension(builder.size1()), sigma(builder) {}

      InputProblem(int d) : dimension(d), sigma(d) {}

      InputProblem(int d, int nnzs) : dimension(d), sigma(d, nnzs) {}

      AnalysisResult pryceAlgorithm() const;
    
      AnalysisResult pryceCompressed(const compression& c) const;
    };
  }
}

#endif
