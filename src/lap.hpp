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

#ifndef DAESTRUCT_LAP_HPP
#define DAESTRUCT_LAP_HPP

#include <vector>
#include <boost/numeric/ublas/matrix.hpp>

struct solution {
  int cost;
  std::vector<int> rowsol;
  std::vector<int> colsol;
  std::vector<int> u;
  std::vector<int> v;  
};

/**
 * Solve the integer linear assignment problem defined by the cost matrix
 */
solution lap(const boost::numeric::ublas::compressed_matrix<int>& cost);

#endif
