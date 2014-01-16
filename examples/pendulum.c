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

#include <daestruct.h>

int main(int argc, char** argv) {

  const int dimension = 3;
  const int x = 0;
  const int y = 1;
  const int F = 2;

  struct daestruct_input* pendulum = daestruct_input_create(3);

  // x² + y² = 1
  daestruct_input_set(pendulum, x, 0, 0);
  daestruct_input_set(pendulum, y, 0, 0);

  // xF = der(der(x))
  daestruct_input_set(pendulum, x, 1, 2);
  daestruct_input_set(pendulum, F, 1, 0);

  // yF = der(der(y)) - g
  daestruct_input_set(pendulum, y, 2, 2);
  daestruct_input_set(pendulum, F, 2, 0);

  struct daestruct_result* result = daestruct_analyse(pendulum);

  const char* variables[] = {"x", "y", "F"};
  const char* equations[] = {"x² + y² = 1", "xF = der(der(x))", "yF = der(der(y)) - g" };

  for (int v = 0; v < dimension; v++)
    printf("'%s' needs to be derived %d times\n", variables[v], daestruct_result_variable_index(result, v));

  for (int eq = 0; eq < dimension; eq++)
    printf("'%s' needs to be derived %d times\n", equations[eq], daestruct_result_equation_index(result, eq));

  daestruct_input_delete(pendulum);
  daestruct_result_delete(result);
}
