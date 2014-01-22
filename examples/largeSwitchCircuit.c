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

#include <stdio.h>
#include <stdlib.h>
#include <daestruct.h>
#include <daestruct/variable_structure.h>

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
 *         /
 *        /             ||                          
 *   o---o   o----------||--------------------o
 *   |                  ||uC[j]               |
 *   |                      ____              |
 *   |            o--------|____|----o        |
 *   |            |         u2[j]    |        |
 *   |   ____     |                  |        |
 *   o--|____|----o--------WWWWWW----o--------o
 *       u1[j]              uL[j]
 * 
 * Switch open:
 * Equations:
 *   0)  u0=220*sin(time*omega);
 *   1)  u0= sum j = 0 .. n-1 u1[j]+uL[j];
 *  
 *   (j = 0..n-1)
 * 
 *   2 + j*8)  u1[j]=R[1,i]*i1[j];
 *   3 + j*8)  u2[j]=R[2,i]*i2[j];
 *   4 + j*8)  uL[j]=L[j]*der(iL[j]);
 *   5 + j*8)  uL[j]=u2[j];
 *   6 + j*8)  i0=i1[j];
 *   7 + j*8)  i1[j]=i2[j]+iL[j];
 * 
 * Variables:
 *   0   1   2+j*8  3+j*8  4+j*8  5+j*8  6+j*8  7+j*8
 *   u0, i0, u1[j], u2[j], uL[j], i1[j], i2[j], iL[j]
 *
 * Switch closed:
 * Equations:
 *   0)  u0=220*sin(time*omega);
 *   1)  u0= sum j = 0 .. n-1 u1[j]+uL[j];
 *  
 *   (j = 0..n-1)
 * 
 *   2 + j*8)  u1[j]=R[1,i]*i1[j];
 *   3 + j*8)  u2[j]=R[2,i]*i2[j];
 *   4 + j*8)  uL[j]=L[j]*der(iL[j]);
 *   5 + j*8)  iC[j]=C[j]*der(uC[j]);
 *   6 + j*8)  uC[j]=u1[j]+u2[j];
 *   7 + j*8)  uL[j]=u2[j];
 *   8 + j*8)  i0=i1[j]+iC[j];
 *   9 + j*8)  i1[j]=i2[j]+iL[j];
 * 
 * Variables:
 *   0   1   2+j*8  3+j*8  4+j*8  5+j*8  6+j*8  7+j*8  8+j*8  9+j*8
 *   u0, i0, u1[j], u2[j], uC[j], uL[j], i1[j], i2[j], iC[j], iL[j]
 */


const int u0 = 0;
const int i0 = 1;

struct sub_circuit {  
  int open;
  int un[8];
  int eq[8];
};

struct circuit {
  int count;
  struct sub_circuit* subs;
};

int u1(struct sub_circuit* s) { return s->un[0]; }
int u2(struct sub_circuit* s) { return s->un[1]; }
int uL(struct sub_circuit* s) { return s->un[2]; }
int i1(struct sub_circuit* s) { return s->un[3]; }
int i2(struct sub_circuit* s) { return s->un[4]; }
int iL(struct sub_circuit* s) { return s->un[5]; }

int uC(struct sub_circuit* s) { return s->open ? -1 : s->un[6]; }
int iC(struct sub_circuit* s) { return s->open ? -1 : s->un[7]; }

void inst_subcircuit(struct daestruct_input* p, struct sub_circuit* s, int open, int* eq, int* un) {  
  s->open = open;
  for (int i = 0; i < 8; i++) {
    s->eq[i] = *eq + i;
    s->un[i] = *un + i;
  }

  //u1[j]=R[1,i]*i1[j];
  daestruct_input_set(p, u1(s), *eq, 0);
  daestruct_input_set(p, i1(s), *eq, 0);

  //u2[j]=R[2,i]*i2[j];
  daestruct_input_set(p, u2(s), *eq + 1, 0);
  daestruct_input_set(p, i2(s), *eq + 1, 0);

  //uL[j]=L[j]*der(iL[j]);
  daestruct_input_set(p, uL(s), *eq + 2, 0);
  daestruct_input_set(p, iL(s), *eq + 2, 1);

  //i1[j]=i2[j]+iL[j];
  daestruct_input_set(p, i1(s), *eq + 3, 0);
  daestruct_input_set(p, i2(s), *eq + 3, 0);
  daestruct_input_set(p, iL(s), *eq + 3, 0);

  //uL[j]=u2[j];
  daestruct_input_set(p, uL(s), *eq + 4, 0);
  daestruct_input_set(p, u2(s), *eq + 4, 0);

  if (!s->open) {
    //iC[j]=C[j]*der(uC[j]);
    daestruct_input_set(p, iC(s), *eq + 5, 0);
    daestruct_input_set(p, uC(s), *eq + 5, 1);

    //uC[j]=u1[j]+u2[j];
    daestruct_input_set(p, uC(s), *eq + 6, 0);
    daestruct_input_set(p, u1(s), *eq + 6, 0);
    daestruct_input_set(p, u2(s), *eq + 6, 0);

    //i0=i1[j]+iC[j];
    daestruct_input_set(p, i0, *eq + 7, 0);
    daestruct_input_set(p, i1(s), *eq + 7, 0);
    daestruct_input_set(p, iC(s), *eq + 7, 0);

    *eq += 8;
    *un += 8;
  } else {
    //i0=i1[j];
    daestruct_input_set(p, i0, *eq + 5, 0);
    daestruct_input_set(p, i1(s), *eq + 5, 0);
    
    *eq += 6;
    *un += 6;
  }
 
}

void inst_circuit(struct daestruct_input* p, struct circuit* circ, int n) {
  
  daestruct_input_set(p, u0, 0, 0);
  daestruct_input_set(p, u0, 1, 0);
  
  circ->count = n;
  circ->subs = calloc(n, sizeof(struct sub_circuit));

  int eq = 2;
  int un = 2;

  for (int j = 0; j < n; j++) {
    inst_subcircuit(p, &(circ->subs[j]), 0, &eq, &un);
    daestruct_input_set(p, u1(&(circ->subs[j])), 1, 0);
    daestruct_input_set(p, uL(&(circ->subs[j])), 1, 0);
  }
}

struct switch_event {
  int n;
  int uC, iC;
  int eq1,eq2,eq3;

  struct daestruct_diff* diff;
};

struct switch_event switch_sub_circuit(struct circuit* circuit, int n) {
  struct sub_circuit* s = &(circuit->subs[n]);

  struct switch_event sw;
  sw.diff = daestruct_diff_new();

  if (circuit->subs[n].open) {
    //close the switch
    const int uC = daestruct_diff_add_unknown(sw.diff);
    const int iC = daestruct_diff_add_unknown(sw.diff);

    //iC[j]=C[j]*der(uC[j]);
    const int eq1 = daestruct_diff_add_equation(sw.diff);
    daestruct_diff_set_new(sw.diff, eq1, uC, 1);
    daestruct_diff_set_new(sw.diff, eq1, iC, 0);

    //uC[j]=u1[j]+u2[j];
    const int eq2 = daestruct_diff_add_equation(sw.diff);
    daestruct_diff_set_new(sw.diff, eq2, uC, 0);
    daestruct_diff_set_existing(sw.diff, eq2, u1(s), 0);
    daestruct_diff_set_existing(sw.diff, eq2, u2(s), 0);

    daestruct_diff_remove_equation(sw.diff, s->eq[5]);

    //i0=i1[j]+iC[j];
    const int eq3 = daestruct_diff_add_equation(sw.diff);
    daestruct_diff_set_new(sw.diff, eq3, iC, 0);
    daestruct_diff_set_existing(sw.diff, eq2, i1(s), 0);
    daestruct_diff_set_existing(sw.diff, eq2, i0, 0);

    sw.n = n;
    sw.uC = uC;
    sw.iC = iC;
    sw.eq1 = eq1;
    sw.eq2 = eq2;
    sw.eq3 = eq3;
  } else {
    //open the switch
    daestruct_diff_remove_unknown(sw.diff, uC(s));
    daestruct_diff_remove_unknown(sw.diff, iC(s));

    daestruct_diff_remove_equation(sw.diff, s->eq[5]);
    daestruct_diff_remove_equation(sw.diff, s->eq[6]);
    daestruct_diff_remove_equation(sw.diff, s->eq[7]);
    
    int new = daestruct_diff_add_equation(sw.diff);
    daestruct_diff_set_existing(sw.diff, new, i0, 0);
    daestruct_diff_set_new(sw.diff, new, i1(s), 0);

    sw.n = n;
    sw.uC = -1;
    sw.iC = -1;
    sw.eq1 = new;
    sw.eq2 = -1;
    sw.eq3 = -1;
  }

  return sw;
}

void reconcile(struct circuit* circ, struct switch_event* sw, struct daestruct_changed* ch) {
  struct sub_circuit * s = &(circ->subs[sw->n]);

  for (int j = 0; j < sw->n; j++) {
    for (int k = 0; k < 8; k++) {
      circ->subs[j].un[k] = daestruct_changed_ex_un_index(ch, circ->subs[j].un[k]);
      circ->subs[j].eq[k] = daestruct_changed_ex_eq_index(ch, circ->subs[j].eq[k]);
    }
  }

  if (s->open) {
    s->open = 0;
    s->un[6] = daestruct_changed_new_un_index(ch, sw->uC);
    s->un[7] = daestruct_changed_new_un_index(ch, sw->iC);
    s->eq[6] = daestruct_changed_new_eq_index(ch, sw->eq1);
    s->eq[7] = daestruct_changed_new_eq_index(ch, sw->eq2);
    s->eq[5] = daestruct_changed_new_eq_index(ch, sw->eq3);
  } else {
    s->open = 1;
    s->un[6] = -1;
    s->un[7] = -1;
    s->eq[6] = -1;
    s->eq[7] = -1;
    s->eq[5] = daestruct_changed_new_eq_index(ch, sw->eq3);
  }

  for (int j = sw->n+1; j < circ->count; j++) {
    for (int k = 0; k < 8; k++) {
      circ->subs[j].un[k] = daestruct_changed_ex_un_index(ch, circ->subs[j].un[k]);
      circ->subs[j].eq[k] = daestruct_changed_ex_eq_index(ch, circ->subs[j].eq[k]);
    }
  }
}

int main(int argc, char** argv) {

  if (argc > 3) {
    const int n = atoi(argv[1]);
    const int dimension = 2 + n*8;

    const int sw = atoi(argv[2]);
    const int times = atoi(argv[3]);

    struct circuit circuit;
    struct daestruct_input* sigma = daestruct_input_create(dimension);    
    inst_circuit(sigma, &circuit, n);

    printf("Circuit created\n");

    struct daestruct_result* result = daestruct_analyse(sigma);
    struct switch_event se = switch_sub_circuit(&circuit, sw);
    struct daestruct_changed* ch = daestruct_change_orig(sigma, result, se.diff);

    for (int k = 0; k < times; k++) {
      daestruct_result_delete(result);
      result = daestruct_changed_analyse(ch);
      reconcile(&circuit, &se, ch);
      daestruct_diff_delete(se.diff);
      se = switch_sub_circuit(&circuit, sw);
      daestruct_changed_delete(ch);
      struct daestruct_changed* old = ch;
      ch = daestruct_change(old, result, se.diff);
      daestruct_changed_delete(old);
    }

    daestruct_input_delete(sigma);
    daestruct_result_delete(result);

    free(circuit.subs);
  }
}
