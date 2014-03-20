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
      compressible_builder sc(2, 8);
      
      /*
                 ____________________________P_______________________________    ____Q_____
                |                                                            |  |          | */
      const int u1 = 0, i1 = 1, u2 = 2, i2 = 3, uC = 4, iC = 5, uL = 6, iL = 7, u = 0, i = 1;

      //u1=R[1]*i1;
      sc.set_private_incidence(0, u1, 0);
      sc.set_private_incidence(0, i1, 0);

      //u2=R[2]*i2;
      sc.set_private_incidence(1, u2, 0);
      sc.set_private_incidence(1, i2, 0);

      //uL=L*der(iL);
      sc.set_private_incidence(2, uL, 0);
      sc.set_private_incidence(2, iL, -1);

      //iC=C*der(uC);
      sc.set_private_incidence(3, uC, -1);
      sc.set_private_incidence(3, iC, 0);

      //u=u1+uL;
      sc.set_public_incidence(4, u, 0);
      sc.set_private_incidence(4, u1, 0);
      sc.set_private_incidence(4, uL, 0);

      //uC=u1+u2;
      sc.set_private_incidence(5, uC, 0);
      sc.set_private_incidence(5, u1, 0);
      sc.set_private_incidence(5, u2, 0);

      //uL=u2;
      sc.set_private_incidence(6, uL, 0);
      sc.set_private_incidence(6, u2, 0);

      //i0=i1+iC;
      sc.set_public_incidence(7, i, 0);
      sc.set_private_incidence(7, i1, 0);
      sc.set_private_incidence(7, iC, 0);

      //i1=i2+iL;
      sc.set_private_incidence(8, i1, 0);
      sc.set_private_incidence(8, i2, 0);
      sc.set_private_incidence(8, iL, 0);	

      return sc.build();
    }

    InputProblem inflated1() {
      InputProblem infl(10);
            /*
                 ____________________________P_______________________________    ____Q_____
                |                                                            |  |          | */
      const int u1 = 2, i1 = 3, u2 = 4, i2 = 5, uC = 6, iC = 7, uL = 8, iL = 9, u = 0, i = 1;

      infl.sigma.insert(0, u, 0);

      //u1=R[1]*i1;
      infl.sigma.insert(1, u1, 0);
      infl.sigma.insert(1, i1, 0);

      //u2=R[2]*i2;
      infl.sigma.insert(2, u2, 0);
      infl.sigma.insert(2, i2, 0);

      //uL=L*der(iL);
      infl.sigma.insert(3, uL, 0);
      infl.sigma.insert(3, iL, -1);

      //iC=C*der(uC);
      infl.sigma.insert(4, uC, -1);
      infl.sigma.insert(4, iC, 0);

      //u=u1+uL;
      infl.sigma.insert(5, u, 0);
      infl.sigma.insert(5, u1, 0);
      infl.sigma.insert(5, uL, 0);

      //uC=u1+u2;
      infl.sigma.insert(6, uC, 0);
      infl.sigma.insert(6, u1, 0);
      infl.sigma.insert(6, u2, 0);

      //uL=u2;
      infl.sigma.insert(7, uL, 0);
      infl.sigma.insert(7, u2, 0);

      //i0=i1+iC;
      infl.sigma.insert(8, i, 0);
      infl.sigma.insert(8, i1, 0);
      infl.sigma.insert(8, iC, 0);

      //i1=i2+iL;
      infl.sigma.insert(9, i1, 0);
      infl.sigma.insert(9, i2, 0);
      infl.sigma.insert(9, iL, 0);
      
      return infl;
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

      AnalysisResult expected = inflated1().pryceAlgorithm();

      BOOST_CHECK_EQUAL( result.d, expected.d );
      
      BOOST_CHECK_EQUAL( result.c, expected.c );

      delete sc;
    }

  }
}
