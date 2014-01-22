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
#ifndef DAESTRUCT_VARIABLE_STRUCTURE_H
#define DAESTRUCT_VARIABLE_STRUCTURE_H

#include <daestruct.h>

#ifdef __cplusplus
extern "C" {
#endif
  
  /**
   * A change in a structural description of a DAE
   */
  struct daestruct_diff;

  struct daestruct_diff* daestruct_diff_new();

  void daestruct_diff_delete(struct daestruct_diff* diff);

  void daestruct_diff_remove_unknown(struct daestruct_diff* diff, int unknown);

  void daestruct_diff_remove_equation(struct daestruct_diff* diff, int equation);

  int daestruct_diff_add_unknown(struct daestruct_diff* diff);

  int daestruct_diff_add_equation(struct daestruct_diff* diff);

  void daestruct_diff_set_existing(struct daestruct_diff* diff, int newEquation, int unknown, int der);

  void daestruct_diff_set_new(struct daestruct_diff* diff, int newEquation, int unknown, int der);

  /**
   * A problem that has been derived from an older problem
   */
  struct daestruct_changed;

  struct daestruct_changed* daestruct_change_orig(struct daestruct_input*  original, 
						  struct daestruct_result* result, 
						  struct daestruct_diff* diff);

  struct daestruct_changed* daestruct_change(struct daestruct_changed* original, 
					     struct daestruct_result* result, 
					     struct daestruct_diff* diff);

  int daestruct_changed_new_un_index(struct daestruct_changed* changed, int new_unknown);

  int daestruct_changed_new_eq_index(struct daestruct_changed* changed, int new_equation);

  int daestruct_changed_ex_un_index(struct daestruct_changed* changed, int new_unknown);

  int daestruct_changed_ex_eq_index(struct daestruct_changed* changed, int new_equation);

  void daestruct_changed_delete(struct daestruct_changed* changed);

  struct daestruct_result* daestruct_changed_analyse(struct daestruct_changed* problem);

#ifdef __cplusplus
}
#endif

#endif
