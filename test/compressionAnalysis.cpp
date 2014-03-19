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

#include "compressionAnalysis.hpp"

namespace daestruct {
  namespace test {

    using namespace std;
    using namespace daestruct::analysis;

    compressible subCircuit() {
      /* compressible sub-component with 2 public and 8 private vars */
      compressible sc(2, 8);
      
      /*
                 ____________________________P_______________________________    ____Q_____
                |                                                            |  |          | */
      const int u1 = 0, i1 = 1, u2 = 2, i2 = 3, uL = 4, iL = 5, uC = 6, iC = 7, u = 8, i = 9;

      //u1=R[1]*i1;
      sc.set_incidence(0, u1, 0);
      sc.set_incidence(0, i1, 0);

      //u2=R[2]*i2;
      sc.set_incidence(1, u2, 0);
      sc.set_incidence(1, i2, 0);

      //uL=L*der(iL);
      sc.set_incidence(2, uL, 0);
      sc.set_incidence(2, iL, -1);

      //iC=C*der(uC);
      sc.set_incidence(3, uC, -1);
      sc.set_incidence(3, iC, 0);

      //u=u1+uL;
      sc.set_incidence(4, u, 0);
      sc.set_incidence(4, u1, 0);
      sc.set_incidence(4, uL, 0);

      //uC=u1+u2;
      sc.set_incidence(5, uC, 0);
      sc.set_incidence(5, u1, 0);
      sc.set_incidence(5, u2, 0);

      //uL=u2;
      sc.set_incidence(6, uL, 0);
      sc.set_incidence(6, u2, 0);

      //i0=i1+iC;
      sc.set_incidence(7, i, 0);
      sc.set_incidence(7, i1, 0);
      sc.set_incidence(7, iC, 0);

      //i1=i2+iL;
      sc.set_incidence(8, i1, 0);
      sc.set_incidence(8, i2, 0);
      sc.set_incidence(8, iL, 0);	

      sc.seal();
      return sc;
    }

    void analyzeCompressedCircuit1() {
      compressible *sc = new compressible(subCircuit());
      InputProblem compressed(2);
      const int u = 0, i = 1;

      /* u = sin(time * omega) */
      compressed.sigma.insert(0, u, 0);
      
      compression comp;
      comp.instances.push_back(compressible_instance(0,1,sc));     
      comp.instances[0].insert_incidence(compressed.sigma);
      
      AnalysisResult result = compressed.pryceCompressed(comp);

      BOOST_CHECK_EQUAL( result.d, std::vector<int>({1, 1, 1, 1, 1, 0, 1, 1, 0, 1}) );
      
      BOOST_CHECK_EQUAL( result.c, std::vector<int>({1, 1, 1, 0, 0, 1, 1, 1, 0, 1}) );

      delete sc;
    }

  }
}
