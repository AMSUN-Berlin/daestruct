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
#include <boost/numeric/ublas/vector_sparse.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/timer/timer.hpp>

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
  using namespace std;

  typedef int der_t;

  class sigma_matrix;
  
  template<class E, class T> inline static void nicePrint(std::basic_ostringstream<E, T>& s, der_t val);

  class sigma_matrix {
  public:

    typedef mapped_vector<der_t, map_array<size_t, der_t>> row_t;
    std::vector<size_t> minimum_row;
    std::vector<row_t> _rows;

    der_t* find_element(size_t i, size_t j) {
      row_t& row = _rows.at(i);
      return row.find_element(j);
    }

    const der_t* find_element(size_t i, size_t j) const {
      const row_t& row = _rows.at(i);
      return row.find_element(j);
    }

    sigma_matrix(const coordinate_matrix<der_t>& builder) : minimum_row(builder.size1()) {
      _rows.reserve(builder.size1());

      for (auto row_iter = builder.begin1(); row_iter != builder.end1(); row_iter++) {
	row_t row(_rows.size());
	for (auto col_iter = row_iter.begin(); col_iter != row_iter.end(); col_iter++) {
	  const size_t i = col_iter.index1();
	  const size_t j = col_iter.index2();
	  const der_t x = *col_iter;

	  const der_t* m_ptr = find_element(minimum_row[j],j);
      
	  if (!m_ptr || *m_ptr > x) {
	    minimum_row.at(j) = i;
	  }
	  row.insert_element(j, x);
	}
	_rows.push_back(row);
      }
    }

    sigma_matrix(size_t d) : minimum_row(d) {
      _rows.reserve(d);
      for (size_t i = 0; i < d; i++)
	_rows.push_back(row_t(d));
    }

    sigma_matrix(size_t d, size_t nnzs) : minimum_row(d) {
      for (size_t i = 0; i < d; i++)
	_rows.push_back(row_t(d));
    }

    const std::vector<row_t>& rows() const {
      return _rows;
    }

    int smallest_cost_row(size_t column) const {
      return minimum_row[column];
    }    

    const int operator()(const size_t i, const size_t j) const {
      const der_t* ptr = find_element(i,j);
      if (ptr)
	return *ptr;
      else
	return BIG;
    }

    void insert(size_t i, size_t j, der_t x) {
      der_t* m_ptr = find_element(minimum_row[j],j);
      
      if (!m_ptr || *m_ptr > x) {
	minimum_row.at(j) = i;
      }
      
      row_t& row = _rows.at(i);

      der_t* ptr = row.find_element(j);
      if (!ptr) {
	row.insert_element(j,x);
      } else {
	*ptr = x;
      }

      //std::cout << "after setting <" << i << "," << j << "> to " << x << std::endl;
      //std::cout << *this << std::endl;
    }

    void push_back(size_t i, size_t j, der_t x) {
      const der_t* m_ptr = find_element(minimum_row[j],j);
      
      if (!m_ptr || *m_ptr > x) {
	minimum_row.at(j) = i;
      }
      
      _rows.at(i).insert_element(j,x);
    }
  
    size_t dimension() const { return _rows.size(); }

    template<class E, class T> inline static void nicePrint(std::basic_ostringstream<E, T>& s, der_t val) {
      if (val == BIG)
	s << "∞";
      else
	s << val;
    }

    template<class E, class T> friend std::basic_ostream<E, T> &operator << (std::basic_ostream<E, T> &os,
									     const sigma_matrix& sigma) {
      const size_t d = sigma.dimension();
    
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
