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

  if (argc > 1) {
    const int n = atoi(argv[1]);
    const int dimension = 2 + n*8;

    struct circuit circuit;
    struct daestruct_input* sigma = daestruct_input_create(dimension);    

    inst_circuit(sigma, &circuit, n);

    printf("Circuit created\n");
    struct daestruct_result* result = daestruct_analyse(sigma);

    daestruct_input_delete(sigma);
    daestruct_result_delete(result);
    free(circuit.subs);
  }
}
