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
#include <boost/test/test_tools.hpp>
#include <prettyprint.hpp>

#include "lap.hpp"
#include "test_lap.hpp"

namespace daestruct {
  namespace test {
    void test_LAP_on_identity() {
      sigma_matrix sigma ( 5 );

      sigma.insert(0, 0, 1);
      sigma.insert(1, 1, 1);
      sigma.insert(2, 2, 1);
      sigma.insert(3, 3, 1);
      sigma.insert(4, 4, 1);

      solution assignment = lap(sigma);
      
      BOOST_CHECK_EQUAL( assignment.rowsol, std::vector<int>({0,1,2,3,4}) );
      BOOST_CHECK_EQUAL( assignment.colsol, std::vector<int>({0,1,2,3,4}) );
      
      std::vector<int> partial_r(assignment.rowsol);
      std::vector<int> partial_c(assignment.colsol);
      std::vector<int> u(assignment.u);
      std::vector<int> v(assignment.v);

      partial_r[2] = -1;
      partial_c[2] = -1;
      solution assignment2 = delta_lap(sigma, u, v, partial_r, partial_c);

      BOOST_CHECK_EQUAL( assignment2.rowsol, std::vector<int>({0,1,2,3,4}) );
      BOOST_CHECK_EQUAL( assignment2.colsol, std::vector<int>({0,1,2,3,4}) );

      u = assignment2.u;
      v = assignment2.v;
      partial_r[1] = -1;
      partial_c[1] = -1;
      solution assignment3 = delta_lap(sigma, u, v, partial_r, partial_c);

      BOOST_CHECK_EQUAL( assignment3.rowsol, std::vector<int>({0,1,2,3,4}) );
      BOOST_CHECK_EQUAL( assignment3.colsol, std::vector<int>({0,1,2,3,4}) );
    }
  }
}
