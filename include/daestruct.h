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
#ifndef DAESTRUCT_H
#define DAESTRUCT_H

#ifdef __cplusplus
extern "C" {
#endif
  
  /* daestruct input description */
  struct daestruct_input;    

  /**
   * set the maximum derivative of @variable in @equation in the given input problem
   */
  void daestruct_input_set(struct daestruct_input* problem, int variable, int equation, int derivative);
 
  /**
   * create an input problem description for the structural analysis
   * the returned pointer must be deleted with daestruct_input_delete
   */
  struct daestruct_input* daestruct_input_create(int dimension);

  /**
   * delete a daestruct input problem
   */
  void daestruct_input_delete(struct daestruct_input* problem);

  /* daestruct invocation */
  struct daestruct_result;

  /**
   * actually run the structural analysis
   * the returned pointer must be deleted with daestruct_result_delete
   */
  struct daestruct_result* daestruct_analyse(struct daestruct_input* problem);

  /**
   * get the derivation index
   */
  int daestruct_result_equation_index(struct daestruct_result* result, int equation);

  /**
   * get the derivation index
   */
  int daestruct_result_variable_index(struct daestruct_result* result, int variable);

  /**
   * delete the given result description
   */
  void daestruct_result_delete(struct daestruct_result* result);  

#ifdef __cplusplus
}
#endif

#endif
