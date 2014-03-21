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

#include <stdlib.h>
#include "compressed_circuit.h"

struct daestruct_component* sub_component() {
  struct daestruct_component_builder* builder = daestruct_component_builder_create(2, 8);
      
  /*
         ____________________________P_______________________________    ____Q_____
        |                                                            |  |          | */
  const int u1 = 0, i1 = 1, u2 = 2, i2 = 3, uC = 4, iC = 5, uL = 6, iL = 7, u = 0, i = 1;

  //u1=R[1]*i1;
  daestruct_component_builder_private_set(builder, 0, u1, 0);
  daestruct_component_builder_private_set(builder, 0, i1, 0);

  //u2=R[2]*i2;
  daestruct_component_builder_private_set(builder, 1, u2, 0);
  daestruct_component_builder_private_set(builder, 1, i2, 0);

  //uL=L*der(iL);
  daestruct_component_builder_private_set(builder, 2, uL, 0);
  daestruct_component_builder_private_set(builder, 2, iL, 1);

  //iC=C*der(uC);
  daestruct_component_builder_private_set(builder, 3, uC, 1);
  daestruct_component_builder_private_set(builder, 3, iC, 0);

  //u=u1+uL;
  daestruct_component_builder_public_set(builder, 4, u, 0);
  daestruct_component_builder_private_set(builder, 4, u1, 0);
  daestruct_component_builder_private_set(builder, 4, uL, 0);

  //uC=u1+u2;
  daestruct_component_builder_private_set(builder, 5, uC, 0);
  daestruct_component_builder_private_set(builder, 5, u1, 0);
  daestruct_component_builder_private_set(builder, 5, u2, 0);

  //uL=u2;
  daestruct_component_builder_private_set(builder, 6, uL, 0);
  daestruct_component_builder_private_set(builder, 6, u2, 0);

  //i0=i1+iC;
  daestruct_component_builder_public_set(builder, 7, i, 0);
  daestruct_component_builder_private_set(builder, 7, i1, 0);
  daestruct_component_builder_private_set(builder, 7, iC, 0);

  //i1=i2+iL;
  daestruct_component_builder_private_set(builder, 8, i1, 0);
  daestruct_component_builder_private_set(builder, 8, i2, 0);
  daestruct_component_builder_private_set(builder, 8, iL, 0);	

  struct daestruct_component* cmp = daestruct_component_build(builder);
  daestruct_component_builder_delete(builder);
  return cmp;
}

void inst_cmpr_subcircuit(struct cmpr_subcircuit* sc,
			  struct daestruct_input* p, 
			  struct daestruct_component* cmp,
			  struct cmpr_circuit* circ,
			  int* eq, int* un) {
  sc->u = (*un)++;
  sc->i = (*un)++;
  sc->s = (*eq)++;
  sc->instance = daestruct_component_instantiate(circ->instances, cmp, sc->u, sc->s);

  daestruct_set_public_parts(p, sc->instance);
}

struct cmpr_circuit* inst_cmpr_circuit(struct daestruct_input* p, struct daestruct_component* cmp, int n) {
  struct cmpr_circuit* circ = malloc(sizeof(struct cmpr_circuit));
  
  circ->n = n;

  circ->instances = daestruct_component_list_empty();

  circ->subs = malloc(sizeof(struct cmpr_subcircuit) * n);

  int eq = 0, un = 0;

  for (int i = 0; i < n; i++)
    inst_cmpr_subcircuit(&(circ->subs[i]), p, cmp, circ, &eq, &un);

  const int sum_eq = eq++;
  for (int i = 0; i < (n-1); i++) {
    const int flow_eq = eq++;
   
    //i_k = i_{k+1}
    daestruct_input_set(p, circ->subs[i].i, flow_eq, 0);
    daestruct_input_set(p, circ->subs[i+1].i, flow_eq, 0);
    
    daestruct_input_set(p, circ->subs[i+1].u, sum_eq, 0);
  }
  daestruct_input_set(p, circ->subs[0].u, sum_eq, 0);
  

  return circ;
}

void delete_cmpr_circuit(struct cmpr_circuit* c) {
  daestruct_component_list_delete(c->instances);
  free(c->subs);
  free(c);  
}
