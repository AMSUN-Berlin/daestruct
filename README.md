libdaestruct
============

Structural analysis of Differential Algebraic Equations (DAEs)

This library provides an implementation of Pryce' index reduction method[1].
In fact this method does not completely implement index reduction but yields
the information about which equations and variables need to be derived up to
which order to maintain an ODE.

Example
-------

The following example demonstrates the usage of daestruct with the classic
cartesian pendulum using the C-interface (for full code, see example/pendulum.c).

```C
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
```


[1] Pryce, John D. "A simple structural analysis method for DAEs." BIT Numerical Mathematics 41.2 (2001): 364-394. http://link.springer.com/article/10.1023/A:1021998624799

