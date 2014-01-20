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
#ifndef DAESTRUCT_VARIABLE_STRUCTURE_H
#define DAESTRUCT_VARIABLE_STRUCTURE_H

#ifdef __cplusplus
extern "C" {
#endif
  
  /**
   * A change of one unknown
   */
  struct daestruct_unkn_diff;

  struct daestruct_unkn_diff* daestruct_remove_unknown(int idx);

  struct daestruct_unkn_diff* daestruct_insert_unknown_before(int idx);

  /**
   * A change of one equation
   */
  struct daestruct_eqn_diff;



#ifdef __cplusplus
}
#endif

#endif
