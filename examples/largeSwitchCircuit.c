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

int u1(int j) { return 2+j*8; }
int u2(int j) { return 3+j*8; }
int uC(int j) { return 4+j*8; }
int uL(int j) { return 5+j*8; }
int i1(int j) { return 6+j*8; }
int i2(int j) { return 7+j*8; }
int iC(int j) { return 8+j*8; }
int iL(int j) { return 9+j*8; }

void set_subcircuit_incidence(struct daestruct_input* p, int j) {
  //   2 + j*8)  u1[j]=R[1,i]*i1[j];
  daestruct_input_set(p, u1(j), 2+j*8, 0);
  daestruct_input_set(p, i1(j), 2+j*8, 0);

  // 3 + j*8)  u2[j]=R[2,i]*i2[j];
  daestruct_input_set(p, u2(j), 3+j*8, 0);
  daestruct_input_set(p, i2(j), 3+j*8, 0);

  // 4 + j*8)  uL[j]=L[j]*der(iL[j]);
  daestruct_input_set(p, uL(j), 4+j*8, 0);
  daestruct_input_set(p, iL(j), 4+j*8, 1);

  // 5 + j*8)  iC[j]=C[j]*der(uC[j]);
  daestruct_input_set(p, iC(j), 5+j*8, 0);
  daestruct_input_set(p, uC(j), 5+j*8, 1);

  // 6 + j*8)  uC[j]=u1[j]+u2[j];
  daestruct_input_set(p, uC(j), 6+j*8, 0);
  daestruct_input_set(p, u1(j), 6+j*8, 0);
  daestruct_input_set(p, u2(j), 6+j*8, 0);

  // 7 + j*8)  uL[j]=u2[j];
  daestruct_input_set(p, uL(j), 7+j*8, 0);
  daestruct_input_set(p, u2(j), 7+j*8, 0);

  // 8 + j*8)  i0=i1[j]+iC[j];
  daestruct_input_set(p, i0, 8+j*8, 0);
  daestruct_input_set(p, i1(j), 8+j*8, 0);
  daestruct_input_set(p, iC(j), 8+j*8, 0);
  
  // 9 + j*8)  i1[j]=i2[j]+iL[j];
  daestruct_input_set(p, i1(j), 9+j*8, 0);
  daestruct_input_set(p, i2(j), 9+j*8, 0);
  daestruct_input_set(p, iL(j), 9+j*8, 0);
}

void set_circuit_incidence(struct daestruct_input* p, int n) {
  
  daestruct_input_set(p, u0, 0, 0);
  daestruct_input_set(p, u0, 1, 0);
  
  for (int j = 0; j < n; j++) {
    daestruct_input_set(p, u1(j), 1, 0);
    daestruct_input_set(p, uL(j), 1, 0);

    set_subcircuit_incidence(p, j);
  }
}

int main(int argc, char** argv) {

  if (argc > 1) {
    const int n = atoi(argv[1]);
    const int dimension = 2 + n*8;

    struct daestruct_input* circuit = daestruct_input_create(dimension);    
    set_circuit_incidence(circuit, n);

    struct daestruct_result* result = daestruct_analyse(circuit);

    daestruct_input_delete(circuit);
    daestruct_result_delete(result);
  }
}
