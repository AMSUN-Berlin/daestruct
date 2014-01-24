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

#include <iostream>
#include <vector>
#include <climits>

#include <daestruct/sigma_matrix.hpp>

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
solution lap(const daestruct::sigma_matrix& cost);

/**
 * Solve the integer linear assignment problem using an older (partiall) assignment
 */
solution delta_lap(const daestruct::sigma_matrix& assigncost, const std::vector<int>& _u, const std::vector<int>& _v, 
		   const std::vector<int>& _rowsol, const std::vector<int>& _colsol);

std::ostream& operator<<(std::ostream& o, const solution& s);

#endif
