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
#include <daestruct/variable_structure.h>
  
#include <daestruct/analysis.hpp>
#include <daestruct/variable_analysis.hpp>
#include <daestruct/sigma_matrix.hpp>
#include <daestruct/c_cpp_interface.hpp>

using namespace daestruct::analysis;

extern "C" {

  struct daestruct_diff* daestruct_diff_new() {
    return new daestruct_diff();
  }

  void daestruct_diff_delete(struct daestruct_diff* diff) {
    delete diff;
  }

  void daestruct_diff_remove_unknown(struct daestruct_diff* diff, int unknown) {
    diff->deletedCols.insert(unknown);
  }

  void daestruct_diff_remove_equation(struct daestruct_diff* diff, int equation) {
    diff->deletedRows.insert(equation);
  }

  int daestruct_diff_add_equation(struct daestruct_diff* diff) {
    NewRow nrow;
    diff->newRows.push_back(nrow);
    return diff->newRows.size() - 1;
  }

  int daestruct_diff_add_unknown(struct daestruct_diff* diff) {
    return diff->newVars++;
  }

  void daestruct_diff_set_existing(struct daestruct_diff* diff, int newEquation, int unknown, int der) {
    diff->newRows[newEquation].ex_vars[unknown] = -der;
  }

  void daestruct_diff_set_new(struct daestruct_diff* diff, int newEquation, int unknown, int der) {
    diff->newRows[newEquation].new_vars[unknown] = -der;
  }

  struct daestruct_changed* daestruct_change_orig(struct daestruct_input*  original, 
						  struct daestruct_result* result, 
						  struct daestruct_diff* diff) {
    return static_cast<daestruct_changed*>(new ChangedProblem(*original, *result, *diff));
  }

  struct daestruct_changed* daestruct_change(struct daestruct_changed* original, 
					     struct daestruct_result* result, 
					     struct daestruct_diff* diff) {
    return static_cast<daestruct_changed*>(new ChangedProblem(*original, *result, *diff));
  }

  int daestruct_changed_new_un_index(struct daestruct_changed* changed, int new_unknown) {
    return new_unknown + changed->old_columns;
  }

  int daestruct_changed_new_eq_index(struct daestruct_changed* changed, int new_equation) {
    return new_equation + changed->old_rows;
  }

  int daestruct_changed_ex_un_index(struct daestruct_changed* changed, int un) {
    return changed->colOffsets(un) + un;
  }

  int daestruct_changed_ex_eq_index(struct daestruct_changed* changed, int eq) {
    return changed->rowOffsets(eq) + eq;
  }

  void daestruct_changed_delete(struct daestruct_changed* changed) {
    delete changed;
  }

  struct daestruct_result* daestruct_changed_analyse(struct daestruct_changed* problem) {
    return static_cast<daestruct_result*>(new AnalysisResult(problem->pryceAlgorithm()));
  }
}
