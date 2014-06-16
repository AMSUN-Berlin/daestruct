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

namespace std {
  template<class M>
  inline
  void swap (boost::numeric::ublas::index_triple<M> i1, boost::numeric::ublas::index_triple<M> i2) {
    i1.swap (i2);
  }
}

namespace daestruct {
  using namespace boost::numeric::ublas;
    
  class sigma_matrix {

    compressed_matrix<int> m;
    std::vector<int> minimum_row;
    std::vector<compressed_matrix<int>::const_iterator1> rows;

  public:
    int dimension;

    sigma_matrix(const coordinate_matrix<int>& builder) : m(builder, builder.size1() * 5), dimension(builder.size1()) {
      minimum_row.reserve(dimension);
      rows.reserve(dimension);
      for (int r = 0; r < dimension; r++)
	rows.push_back(m.find1(0, r, 0));      

      for (auto row_iter = rowBegin(); row_iter != rowEnd(); row_iter++)
	for (auto col_iter = row_iter.begin(); col_iter != row_iter.end(); col_iter++) {
	  const int i = col_iter.index1();
	  const int j = col_iter.index2();
	  const int x = *col_iter;
	  const int* m_ptr = m.find_element(minimum_row[j],j);
      
	  if (!m_ptr || *m_ptr > x) {
	    minimum_row.at(j) = i;
	  }	  
	}

    }

    sigma_matrix(int d) : m(d, d, 3*d), minimum_row(d), dimension(d) {
      rows.reserve(d);
      for (int r = 0; r < d; r++)
	rows.push_back(m.find1(0, r, 0));
    }

    compressed_matrix<int>::const_iterator1 rowBegin() const {
      return m.begin1();
    }

    compressed_matrix<int>::const_iterator1 rowEnd() const {
      return m.end1();
    }

    compressed_matrix<int>::const_iterator1 findRow(int i) const {
      return rows[i];
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
      const int* m_ptr = m.find_element(minimum_row[j],j);
      
      if (!m_ptr || *m_ptr > x) {
	minimum_row.at(j) = i;
      }
      
      int* ptr = m.find_element(i, j);
      if (!ptr) {
	m.insert_element(i,j,x);
      } else {
	*ptr = x;
      }
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
      s << '[' << d << ',' << d << "]" << std::endl << "(";
      if (d > 0) {
	s << '(' ;
	nicePrint(s, sigma(0, 0));
	for (long j = 1; j < d; ++ j) {
	  s << ','; nicePrint(s, sigma(0, j));
	}
	s << ')' << std::endl;    
	for (long i = 1; i < d; ++ i) {
	  s << ",(" ;
	  nicePrint(s, sigma(i, 0));
	  for (long j = 1; j < d; ++ j) {
	    s << ',';
	    nicePrint(s, sigma(i, j));
	  }
	  s << ')' << std::endl;
	}
      }
      s << ')';

      return os << "sigma_matrix " << s.str ().c_str ();
    }
  };
}

#endif 
