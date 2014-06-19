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
  /* faster data structure for building up an input problem */
  struct daestruct_input_builder;

  /**
   * set the maximum derivative of @variable in @equation in the given input problem
   */
  void daestruct_input_builder_append(struct daestruct_input_builder* problem, int variable, int equation, int derivative);
 
  /**
   * create an input problem description builder for the structural analysis
   * the returned pointer must be deleted with daestruct_input_builder_delete
   */
  struct daestruct_input_builder* daestruct_input_builder_create(int dimension);

  /**
   * delete a daestruct input problem builder
   */
  void daestruct_input_builder_delete(struct daestruct_input_builder* problem);
  
  /* daestruct input description */
  struct daestruct_input;    
 
 /**
   * create an input problem description from the builder
   * the returned pointer must be deleted with daestruct_input_delete
   */
  struct daestruct_input* daestruct_input_builder_build(struct daestruct_input_builder* problem);

  /**
   * set the maximum derivative of @variable in @equation in the given input problem
   */
  void daestruct_input_set(struct daestruct_input* problem, int variable, int equation, int derivative);

  /**
   * set the maximum derivative of @variable in @equation in the given input problem
   * This method needs to be called in order, i.e. the <variable, equation> indices must
   * be strictly larger on each usage.
   */
  void daestruct_input_push_back(struct daestruct_input* problem, int variable, int equation, int derivative);
 
  /**
   * create an input problem description for the structural analysis
   * the returned pointer must be deleted with daestruct_input_delete
   */
  struct daestruct_input* daestruct_input_create(int dimension);

  /**
   * create an input problem description for the structural analysis
   * This variant allocates enough space for the given number of 
   * elements that will be set 
   * the returned pointer must be deleted with daestruct_input_delete
   */
  struct daestruct_input* daestruct_input_allocate(int dimension, int nonzeros);

  /**
   * delete a daestruct input problem
   */
  void daestruct_input_delete(struct daestruct_input* problem);

  /* daestruct invocation */
  struct daestruct_result;

  /**
   * actually run the structural analysis
   * the returned pointer must be deleted with daestruct_result_delete()
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

  struct daestruct_component_builder;

  /**
   * allocate a new compressed-component builder
   * the returned pointer must be deleted with daestruct_component_builder_delete()
   */
  struct daestruct_component_builder* daestruct_component_builder_create(int publics, int privates);

  /**
   * delete the given component builder
   */
  void daestruct_component_builder_delete(struct daestruct_component_builder* bldr);
  
  /**
   * Set the degree of derivation of the n-th public variable
   */
  void daestruct_component_builder_public_set(struct daestruct_component_builder* bldr, int equation, int public_var, int degree);
  
  /**
   * Set the degree of derivation of the n-th private variable
   */
  void daestruct_component_builder_private_set(struct daestruct_component_builder* bldr, int equation, int private_var, int degree);

  struct daestruct_component;  

  /**
   * build a compression component
   * the resulting pointer must be deleted with daestruct_component_delete()
   */
  struct daestruct_component* daestruct_component_build(struct daestruct_component_builder* builder);

  /**
   * delete the given component
   */
  void daestruct_component_delete(struct daestruct_component* cmp);
  
  struct daestruct_component_list;
  
  /**
   * Allocates a new list of component instances
   */ 
  struct daestruct_component_list* daestruct_component_list_empty();

  struct daestruct_component_instance;

  /**
   * Instantiate a component at the given offset using the given surrogat equation
   * returns a handle to the list-item, the returned pointer will be deleted with the list automatically
   */
  struct daestruct_component_instance* daestruct_component_instantiate(struct daestruct_component_list* list, 
								       struct daestruct_component* cmp, 
								       int public_var_offset, 
								       int surrogat_equation);
  /**
   * insert the public part (i.e. public variables and surrogat row) of an instantiated component
   * into an input problem
   */
  void daestruct_set_public_parts(struct daestruct_input* problem, struct daestruct_component_instance* instance);

  /**
   * Delete a list of instances
   */
  void daestruct_component_list_delete(struct daestruct_component_list* list);

  /**
   * run the structural analysis using a list of compressed components
   * the returned pointer must be deleted with daestruct_result_delete()
   */
  struct daestruct_result* daestruct_analyse_compressed(struct daestruct_input* problem, struct daestruct_component_list* list);

#ifdef __cplusplus
}
#endif

#endif
