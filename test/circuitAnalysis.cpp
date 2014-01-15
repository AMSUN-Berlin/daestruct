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
    void setCircuitIncidence(const unsigned int eq, const incidence_setter& setter) {
      switch(eq) {
      case 0 : { //u0=220*sin(time*omega);
	setter(eq, 0, 0);
	return;
      }
      case 1 : { //u1=R[1]*i1;
	setter(eq, 1, 0);
	setter(eq, 6, 0);
	return;
      }
      case 2 : { //u2=R[2]*i2;
	setter(eq, 2, 0);
	setter(eq, 7, 0);
	return;
      }
      case 3 : { //uL=L*der(iL);
	setter(eq, 4, 0);
	setter(eq, 9, 1);
	return;
      }
      case 4 : { //iC=C*der(uC);
	setter(eq, 3, 1);
	setter(eq, 8, 0);
	return;
      }
      case 5 : { //u0=u1+uL;
	setter(eq, 0, 0);
	setter(eq, 1, 0);
	setter(eq, 4, 0);
	return;
      } 
      case 6 : { //uC=u1+u2;
	setter(eq, 3, 0);
	setter(eq, 1, 0);
	setter(eq, 2, 0);
	return;
      }
      case 7 : { //uL=u2;
	setter(eq, 4, 0);
	setter(eq, 2, 0);
	return;
      } 
      case 8 : { //i0=i1+iC;
	setter(eq, 5, 0);
	setter(eq, 6, 0);
	setter(eq, 8, 0);
	return;
      }
      case 9 : { //i1=i2+iL;
	setter(eq, 6, 0);
	setter(eq, 7, 0);
	setter(eq, 9, 0);
	return;
      }
      default:
	return;
      }
    }

    void analyzeCircuit1() {

      InputProblem circuit;
      circuit.dimension = 10;
      circuit.mkSigma = &setCircuitIncidence;

      const AnalysisResult res = circuit.pryceAlgorithm();      

      /* x and y appear two-times differentiated */
      BOOST_CHECK_EQUAL( res.c, std::vector<int>({1, 1, 1, 1, 1, 0, 1, 1, 0, 1}) );
      
      /* x² + y² = 1 needs to be two-times differentiated */
      BOOST_CHECK_EQUAL( res.d, std::vector<int>({1, 1, 1, 0, 0, 1, 1, 1, 0, 1}) );

    };

  }

}
