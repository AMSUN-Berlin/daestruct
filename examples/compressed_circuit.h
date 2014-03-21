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

#ifndef DAESTRUCT_EXAMPLE_COMPRESSED_CIRCUIT_H
#define DAESTRUCT_EXAMPLE_COMPRESSED_CIRCUIT_H

#include <daestruct.h>

/**
 * Run structural analysis of the following circuit-model:

 *       _________             ___________
 *      |         |           |           |
 *   o--| circ[0] |--- ... ---| circ[n-1] |---o
 *   |  |_________|           |___________|   |
 *   |                                        |
 *   |          _____                         |
 *   |         /     \                        |
 *   o---------|  ~  |------------------------o
 *             \_____/u0
 *
 *   Where circ[j] =
 *
 *          
 *                      ||                          
 *   o------------------||--------------------o
 *   |                  ||uC[j]               |
 *   |                      ____              |
 *   |            o--------|____|----o        |
 *   |            |         u2[j]    |        |
 *   |   ____     |                  |        |
 *   o--|____|----o--------WWWWWW----o--------o
 *       u1[j]              uL[j]
 *
 *  ________________________________private vars__________________   ____public vars____
 * |                                                              | |                   |
 *  u1 = 0, i1 = 1, u2 = 2, i2 = 3, uC = 4, iC = 5, uL = 6, iL = 7      u = 0, i = 1
 */

struct cmpr_subcircuit {  
  int u, i, s;
  struct daestruct_component_instance* instance;
};

struct cmpr_circuit {
  int n;
  struct cmpr_subcircuit* subs;
  struct daestruct_component_list* instances;
  struct daestruct_component *component;
};

struct daestruct_component* sub_component();

void inst_cmpr_subcircuit(struct cmpr_subcircuit*, 
			  struct daestruct_input* p, 
			  struct daestruct_component* cmp,
			  struct cmpr_circuit* circ,
			  int* eq, int* un);

struct cmpr_circuit* inst_cmpr_circuit(struct daestruct_input* p, struct daestruct_component* cmp, int n);

void delete_cmpr_circuit(struct cmpr_circuit* c);


#endif


