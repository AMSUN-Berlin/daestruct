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
#ifndef DAE_C_CPP_INTERFACE_HPP
#define DAE_C_CPP_INTERFACE_HPP

#include <daestruct/analysis.hpp>
#include <daestruct/variable_analysis.hpp>

using namespace daestruct::analysis;

struct daestruct_input : public InputProblem {};

struct daestruct_result : public AnalysisResult {};

struct daestruct_diff : public StructChange {};

struct daestruct_changed : public ChangedProblem {};

struct daestruct_component_builder : public compressible_builder {};

struct daestruct_component : public compressible {}; 

struct daestruct_component_list : public compression {};

struct daestruct_component_instance : public compressible_instance {};

#endif
