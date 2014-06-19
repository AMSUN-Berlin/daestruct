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
#include <boost/timer/timer.hpp>

extern "C" {

  using namespace daestruct::analysis;

  void daestruct_input_builder_append(struct daestruct_input_builder* problem, int variable, int equation, int derivative) {
    problem->append_element(variable, equation, derivative);
  }
 
  struct daestruct_input_builder* daestruct_input_builder_create(int dimension) {
    return static_cast<daestruct_input_builder*>(new coordinate_matrix<daestruct::der_t> (dimension, dimension, 8*dimension));
  }

  void daestruct_input_builder_delete(struct daestruct_input_builder* problem) {
    delete problem;
  }
  
  struct daestruct_input* daestruct_input_builder_build(struct daestruct_input_builder* problem) {
    return static_cast<daestruct_input*>(new InputProblem(*problem));
  }

  void daestruct_input_push_back(struct daestruct_input* problem, int variable, int equation, int derivative) {
    problem->sigma.push_back(equation, variable, -derivative);
  }
  
  void daestruct_input_set(struct daestruct_input* problem, int variable, int equation, int derivative) {
    problem->sigma.insert(equation, variable, -derivative);
  }
 
  struct daestruct_input* daestruct_input_create(int dimension) {
    return static_cast<daestruct_input*>(new InputProblem(dimension));
  }

  struct daestruct_input* daestruct_input_allocate(int dimension, int nonzeros) {
    return static_cast<daestruct_input*>(new InputProblem(dimension, nonzeros));
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

  struct daestruct_component_builder* daestruct_component_builder_create(int publics, int privates) {
    return static_cast<struct daestruct_component_builder*>
      (new compressible_builder(publics, privates));
  }

  void daestruct_component_builder_delete(struct daestruct_component_builder* bldr) {
    delete bldr;
  }
  
  void daestruct_component_builder_public_set(struct daestruct_component_builder* bldr, int equation, int public_var, int degree) {
    bldr->set_public_incidence(equation, public_var, -degree);
  }
  
  void daestruct_component_builder_private_set(struct daestruct_component_builder* bldr, int equation, int private_var, int degree) {
    bldr->set_private_incidence(equation, private_var, -degree);    
  }

  struct daestruct_component* daestruct_component_build(struct daestruct_component_builder* builder) {
    return static_cast<struct daestruct_component*>(new compressible(builder->build()));
  }

  void daestruct_component_delete(struct daestruct_component* cmp) {
    delete cmp;
  }
  
  struct daestruct_component_list* daestruct_component_list_empty() {
    return static_cast< struct daestruct_component_list*>(new compression());
  }

  struct daestruct_component_instance* daestruct_component_instantiate(struct daestruct_component_list* list, 
								       struct daestruct_component* cmp, 
								       int public_var_offset, 
								       int surrogat_equation) {
    list->instances.push_back(compressible_instance(public_var_offset, surrogat_equation, cmp));
    return static_cast<struct daestruct_component_instance*>(&list->instances.back());
  }

  void daestruct_set_public_parts(struct daestruct_input* problem, 
				  struct daestruct_component_instance* instance) {
    instance->insert_incidence(problem->sigma);
  }

  void daestruct_component_list_delete(struct daestruct_component_list* list) {
    delete list;
  }

  struct daestruct_result* daestruct_analyse_compressed(struct daestruct_input* problem, struct daestruct_component_list* list) {
    return static_cast<struct daestruct_result*>(new AnalysisResult(problem->pryceCompressed(*list)));
  }

}
