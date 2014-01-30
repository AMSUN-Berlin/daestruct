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

#include <daestruct.h>

#include <daestruct/analysis.hpp>
#include <daestruct/sigma_matrix.hpp>
#include <daestruct/c_cpp_interface.hpp>

extern "C" {

  using namespace daestruct::analysis;

  void daestruct_input_set(struct daestruct_input* problem, int variable, int equation, int derivative) {
    problem->sigma.insert(equation, variable, -derivative);
  }
 
  struct daestruct_input* daestruct_input_create(int dimension) {
    return static_cast<daestruct_input*>(new InputProblem(dimension));
  }

  void daestruct_input_delete(struct daestruct_input* problem) {
    delete problem;
  }

  struct daestruct_result* daestruct_analyse(struct daestruct_input* problem) {
    return static_cast<daestruct_result*>(new AnalysisResult(problem->pryceAlgorithm()));
  }

  int daestruct_result_equation_index(struct daestruct_result* result, int equation) {
    return result->c[equation];
  }

  int daestruct_result_variable_index(struct daestruct_result* result, int variable) {
    return result->d[variable];
  }

  void daestruct_result_delete(struct daestruct_result* result) {
    delete result;
  }
}
