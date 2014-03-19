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

#ifndef DAESTRUCT_TEST_COMPRESSION_ANALYSIS_HPP
#define DAESTRUCT_TEST_COMPRESSION_ANALYSIS_HPP

namespace daestruct {
  namespace test {

    /**
     * Run structural analysis of the following circuit-model:
     *                      ||                          
     *   o------------------||--------------------o
     *   |                  ||uC                  |
     *   |                      ____              |
     *   |            o--------|____|----o        |
     *   |   ____     |          u2      |        |
     *   o--|____|----o--------WWWWWW----o--------o
     *   |    u1    _____        uL               |
     *   |         /     \                        |
     *   o---------|  ~  |------------------------o
     *             \_____/u0
     *
     *   Results from: 
     *   http://trac.jmodelica.org/browser/trunk/Compiler/ModelicaFrontEnd/src/test/IndexReduction.mo
     * 
     *   0)  u0=220*sin(time*omega);
     *   1)  u1=R[1]*i1;
     *   2)  u2=R[2]*i2;
     *   3)  uL=L*der(iL);
     *   4)  iC=C*der(uC);
     *   5)  u0= u1+uL;
     *   6)  uC=u1+u2;
     *   7)  uL=u2;
     *   8)  i0=i1+iC;
     *   9)  i1=i2+iL;
     * public variables : i0, u0
     */
    void analyzeCompressedCircuit1();

  }

}


#endif
