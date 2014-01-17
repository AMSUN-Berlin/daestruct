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


#ifndef DAESTRUCT_SIGMA_MATRIX_HPP
#define DAESTRUCT_SIGMA_MATRIX_HPP

#include <iostream>
#include <climits>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>

#define BIG (INT_MAX / 2)

namespace daestruct {
  using namespace boost::numeric::ublas;

  class sigma_matrix {

    compressed_matrix<int> m;
    std::vector<int> minimum_row;

  public:
    long dimension;

    sigma_matrix(long d) : m(d, d, 3*d), minimum_row(d), dimension(d) {}

    compressed_matrix<int>::const_iterator1 rowBegin() const {
      return m.begin1();
    }

    compressed_matrix<int>::const_iterator1 rowEnd() const {
      return m.end1();
    }

    compressed_matrix<int>::const_iterator1 findRow(int i) const {
      return m.find1(0, i, 0);
    }

    int smallest_cost_row(int column) const {
      return minimum_row[column];
    }    

    const int operator()(const int i, const int j) const {
      const int* ptr = m.find_element(i,j);
      if (ptr)
	return *ptr;
      else
	return BIG;
    }

    void insert(int i, int j, int x) {
      const int* ptr = m.find_element(minimum_row[j],j);
      
      if (!ptr || *ptr > x)
	minimum_row[j] = i;

      m.insert_element(i,j,x);
    }
  
    template<class E, class T> inline static void nicePrint(std::basic_ostringstream<E, T>& s, int val) {
      if (val == BIG)
	s << "∞";
      else
	s << val;
    }

    template<class E, class T> friend std::basic_ostream<E, T> &operator << (std::basic_ostream<E, T> &os,
									     const sigma_matrix& sigma) {
      const long d = sigma.dimension;
    
      std::basic_ostringstream<E, T, std::allocator<E> > s;
      s.flags (os.flags ());
      s.imbue (os.getloc ());
      s.precision (os.precision ());
      s << '[' << d << ',' << d << "](";
      if (d > 0) {
	s << '(' ;
	nicePrint(s, sigma(0, 0));
	for (long j = 1; j < d; ++ j) {
	  s << ','; nicePrint(s, sigma(0, j));
	}
	s << ')';    
	for (long i = 1; i < d; ++ i) {
	  s << ",(" ;
	  nicePrint(s, sigma(i, 0));
	  for (long j = 1; j < d; ++ j) {
	    s << ',';
	    nicePrint(s, sigma(i, j));
	  }
	  s << ')';
	}
      }
      s << ')';

      return os << "sigma_matrix " << s.str ().c_str ();
    }
  };
}

#endif 
