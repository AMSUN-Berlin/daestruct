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

    void test_LAP_neg_delta() {
      sigma_matrix sigma ( 3 );

      /*
	    1  2  3
	   ---------
	A |-1 -1  X |
	  |---------|
	B |-2 -1 -1 |
	  |---------|
	C | X -3 -1 |
	  -----------
       */

      sigma.insert(0, 0, -1);
      sigma.insert(0, 1, -1);

      sigma.insert(1, 0, -2);
      sigma.insert(1, 1, -1);
      sigma.insert(1, 2, -1);

      sigma.insert(2, 1, -3);
      sigma.insert(2, 2, -1);

      std::vector<size_t> rowsol({BIG, 0, 1});
      std::vector<size_t> colsol({ 1, 2, BIG});

      std::vector<int> u({ 0, 0, 0});
      std::vector<int> v({ -2, -3, 0});

      solution assignment = delta_lap(sigma, u, v, rowsol, colsol);
      
      BOOST_CHECK_EQUAL( assignment.cost, -5 );
      BOOST_CHECK_EQUAL( assignment.rowsol, std::vector<size_t>({0,2,1}) );      
      BOOST_CHECK_EQUAL( assignment.colsol, std::vector<size_t>({0,2,1}) );      
    }

    void test_LAP_better_delta() {
      sigma_matrix sigma ( 2 );

      /*
	    1  2  
	   ------
	A | 0  3 |
	  |------|
	B | 1  X |
	   ------
       */
      sigma.insert(0, 0, 0);
      sigma.insert(0, 1, 3);

      sigma.insert(1, 0, 1);

      solution assignment = lap(sigma);
      
      BOOST_CHECK_EQUAL( assignment.rowsol, std::vector<size_t>({1,0}) );
      BOOST_CHECK_EQUAL( assignment.colsol, std::vector<size_t>({1,0}) );      
      BOOST_CHECK_EQUAL( assignment.cost, 4);

      sigma_matrix sigma2 ( 2 );

      /*
	    1  2  
	   ------
	A | 0  3 |
	  |------|
	B | 1  3 |
	   ------
       */
      sigma2.insert(0, 0, 0);
      sigma2.insert(0, 1, 3);

      sigma2.insert(1, 0, 1);
      sigma2.insert(1, 1, 3);

      solution delta_assignment = delta_lap(sigma2, assignment.u, assignment.v, std::vector<size_t>({1, BIG}), std::vector<size_t>({BIG,0}));

      BOOST_CHECK_EQUAL( delta_assignment.rowsol, std::vector<size_t>({0,1}) );
      BOOST_CHECK_EQUAL( delta_assignment.colsol, std::vector<size_t>({0,1}) );      
      BOOST_CHECK_EQUAL( delta_assignment.cost, 3);
    }

    void test_LAP_delta() {
      sigma_matrix sigma ( 3 );

      /*
	    1  2  3
	   ---------
	A | 1  1  X |
	  |---------|
	B | 2  1  1 |
	  |---------|
	C | X  3  1 |
	  -----------
       */

      sigma.insert(0, 0, 1);
      sigma.insert(0, 1, 1);

      sigma.insert(1, 0, 2);
      sigma.insert(1, 1, 1);
      sigma.insert(1, 2, 1);

      sigma.insert(2, 1, 3);
      sigma.insert(2, 2, 1);

      std::vector<size_t> rowsol({BIG, 0, 1});
      std::vector<size_t> colsol({ 1, 2, BIG});

      std::vector<int> u({ 0, 0, 0});
      std::vector<int> v({ 2, 3, 0});

      solution assignment = delta_lap(sigma, u, v, rowsol, colsol);
      
      BOOST_CHECK_EQUAL( assignment.cost, 4 );
      BOOST_CHECK_EQUAL( assignment.rowsol, std::vector<size_t>({1,0,2}) );      
      BOOST_CHECK_EQUAL( assignment.colsol, std::vector<size_t>({1,0,2}) );      
    }
    
    void test_LAP_taxi_example() {
      sigma_matrix sigma ( 5 );

      //from: https://bisor.wiwi.uni-kl.de/orwiki/Assignment_problem:_Hungarian_method_3#Example_2_.E2.80.93_Minimazation_problem
      sigma.insert(0, 0, 12);
      sigma.insert(0, 1, 8);
      sigma.insert(0, 2, 11);
      sigma.insert(0, 3, 18);
      sigma.insert(0, 4, 11);

      sigma.insert(1, 0, 14);
      sigma.insert(1, 1, 22);
      sigma.insert(1, 2, 8);
      sigma.insert(1, 3, 12);
      sigma.insert(1, 4, 14);

      sigma.insert(2, 0, 14);
      sigma.insert(2, 1, 14);
      sigma.insert(2, 2, 16);
      sigma.insert(2, 3, 14);
      sigma.insert(2, 4, 15);

      sigma.insert(3, 0, 19);
      sigma.insert(3, 1, 11);
      sigma.insert(3, 2, 14);
      sigma.insert(3, 3, 17);
      sigma.insert(3, 4, 15);

      sigma.insert(4, 0, 13);
      sigma.insert(4, 1, 9);
      sigma.insert(4, 2, 17);
      sigma.insert(4, 3, 20);
      sigma.insert(4, 4, 11);

      solution assignment = lap(sigma);

      BOOST_CHECK_EQUAL( assignment.rowsol, std::vector<size_t>({0,2,3,1,4}) );      
      BOOST_CHECK_EQUAL( assignment.colsol, std::vector<size_t>({0,3,1,2,4}) );

      std::vector<size_t> partial_r(assignment.rowsol);
      std::vector<size_t> partial_c(assignment.colsol);
      std::vector<int> u(assignment.u);
      std::vector<int> v(assignment.v);

      partial_r[0] = BIG;
      partial_c[0] = BIG;
      solution assignment2 = delta_lap(sigma, u, v, partial_r, partial_c);

      BOOST_CHECK_EQUAL( assignment2.rowsol, std::vector<size_t>({0,2,3,1,4}) );      
      BOOST_CHECK_EQUAL( assignment2.colsol, std::vector<size_t>({0,3,1,2,4}) );

    }

    void test_LAP_on_lifted_identity() {
      sigma_matrix sigma ( 5 );
    
      sigma.insert(0, 0, 4);
      sigma.insert(1, 1, 4);
      sigma.insert(2, 2, 4);
      sigma.insert(3, 3, 4);
      sigma.insert(4, 4, 4);

      sigma.insert(1, 0, 2);
      sigma.insert(2, 0, 2);
      sigma.insert(3, 0, 2);
      sigma.insert(4, 0, 2);

      sigma.insert(2, 1, 1);
      sigma.insert(3, 1, 1);
      sigma.insert(4, 1, 1);

      sigma.insert(3, 2, 0);
      sigma.insert(4, 2, 0);

      sigma.insert(4, 3, 0);

      solution assignment = lap(sigma);
      
      BOOST_CHECK_EQUAL( assignment.rowsol, std::vector<size_t>({0,1,2,3,4}) );
      BOOST_CHECK_EQUAL( assignment.colsol, std::vector<size_t>({0,1,2,3,4}) );      
    }
    
    void test_LAP_on_identity() {
      sigma_matrix sigma ( 5 );

      sigma.insert(0, 0, 1);
      sigma.insert(1, 1, 1);
      sigma.insert(2, 2, 1);
      sigma.insert(3, 3, 1);
      sigma.insert(4, 4, 1);

      solution assignment = lap(sigma);
      
      BOOST_CHECK_EQUAL( assignment.rowsol, std::vector<size_t>({0,1,2,3,4}) );
      BOOST_CHECK_EQUAL( assignment.colsol, std::vector<size_t>({0,1,2,3,4}) );
      
      std::vector<size_t> partial_r(assignment.rowsol);
      std::vector<size_t> partial_c(assignment.colsol);
      std::vector<int> u(assignment.u);
      std::vector<int> v(assignment.v);

      partial_r[2] = BIG;
      partial_c[2] = BIG;
      solution assignment2 = delta_lap(sigma, u, v, partial_r, partial_c);

      BOOST_CHECK_EQUAL( assignment2.rowsol, std::vector<size_t>({0,1,2,3,4}) );
      BOOST_CHECK_EQUAL( assignment2.colsol, std::vector<size_t>({0,1,2,3,4}) );

      u = assignment2.u;
      v = assignment2.v;
      partial_r[1] = BIG;
      partial_c[1] = BIG;
      solution assignment3 = delta_lap(sigma, u, v, partial_r, partial_c);

      BOOST_CHECK_EQUAL( assignment3.rowsol, std::vector<size_t>({0,1,2,3,4}) );
      BOOST_CHECK_EQUAL( assignment3.colsol, std::vector<size_t>({0,1,2,3,4}) );
    }
  }
}
