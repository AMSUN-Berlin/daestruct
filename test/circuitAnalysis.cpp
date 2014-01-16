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
#include <daestruct/analysis.hpp>
#include <boost/test/test_tools.hpp>

#include <prettyprint.hpp>

#include "circuitAnalysis.hpp"

namespace daestruct {
  namespace test {

    using namespace std;
    using namespace daestruct::analysis;

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
     * Equations:
     *   0)  u0=220*sin(time*omega);
     *   1)  u1=R[1]*i1;
     *   2)  u2=R[2]*i2;
     *   3)  uL=L*der(iL);
     *   4)  iC=C*der(uC);
     *   5)  u0=u1+uL;
     *   6)  uC=u1+u2;
     *   7)  uL=u2;
     *   8)  i0=i1+iC;
     *   9)  i1=i2+iL;
     * 
     * Variables:
     *   0  1  2  3  4  5  6  7  8  9
     *   u0,u1,u2,uC,uL,i0,i1,i2,iC,iL
     */
    void setCircuitIncidence(InputProblem& p) {
      //u0=220*sin(time*omega);
      p.sigma.insert(0, 0, 0);
      //u1=R[1]*i1;
      p.sigma.insert(1, 1, 0);
      p.sigma.insert(1, 6, 0);
      //u2=R[2]*i2;
      p.sigma.insert(2, 2, 0);
      p.sigma.insert(2, 7, 0);
      //uL=L*der(iL);
      p.sigma.insert(3, 4, 0);
      p.sigma.insert(3, 9, -1);
      //iC=C*der(uC);
      p.sigma.insert(4, 3, -1);
      p.sigma.insert(4, 8, 0);
      //u0=u1+uL;
      p.sigma.insert(5, 0, 0);
      p.sigma.insert(5, 1, 0);
      p.sigma.insert(5, 4, 0);
      //uC=u1+u2;
      p.sigma.insert(6, 3, 0);
      p.sigma.insert(6, 1, 0);
      p.sigma.insert(6, 2, 0);
      //uL=u2;
      p.sigma.insert(7, 4, 0);
      p.sigma.insert(7, 2, 0);
      //i0=i1+iC;
      p.sigma.insert(8, 5, 0);
      p.sigma.insert(8, 6, 0);
      p.sigma.insert(8, 8, 0);

      //i1=i2+iL;
      p.sigma.insert(9, 6, 0);
      p.sigma.insert(9, 7, 0);
      p.sigma.insert(9, 9, 0);	
    }

    void analyzeCircuit1() {
      InputProblem circuit(10);
      setCircuitIncidence(circuit);

      const AnalysisResult res = circuit.pryceAlgorithm();      

      /* x and y appear two-times differentiated */
      BOOST_CHECK_EQUAL( res.c, std::vector<int>({1, 1, 1, 1, 1, 0, 1, 1, 0, 1}) );
      
      /* x² + y² = 1 needs to be two-times differentiated */
      BOOST_CHECK_EQUAL( res.d, std::vector<int>({1, 1, 1, 0, 0, 1, 1, 1, 0, 1}) );

    };

  }

}
