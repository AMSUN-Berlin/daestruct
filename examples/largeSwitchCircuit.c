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

#include "circuit.h"

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

    if (times > 0) {
    struct switch_event se = switch_sub_circuit(&circuit, sw);
    struct daestruct_changed* ch = daestruct_change_orig(sigma, result, se.diff);

    for (int k = 0; k < times; k++) {
      printf("Switch %d out of %d\n", k+1, times);
      daestruct_result_delete(result);
      printf("Running analysis...\n");
      result = daestruct_changed_analyse(ch);
      printf("Reconciling...\n");
      reconcile(&circuit, &se, ch);
      daestruct_diff_delete(se.diff);

      if (k < times - 1) { 
	printf("Next event...\n");
	se = switch_sub_circuit(&circuit, sw);

	struct daestruct_changed* old = ch;
	ch = daestruct_change(old, result, se.diff);
	daestruct_changed_delete(old);
      }
    }
    
    daestruct_changed_delete(ch);
    }
    daestruct_result_delete(result);
    daestruct_input_delete(sigma);

    free(circuit.subs);
  }
}
