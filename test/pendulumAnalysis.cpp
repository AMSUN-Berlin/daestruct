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

#include <string>
#include <prettyprint.hpp>

#include "pendulumAnalysis.hpp"

namespace daestruct {
  namespace test {

    using namespace std;
    using namespace daestruct::analysis;

    string varToStr(const unsigned int var) {
      switch(var) {
	case 0 : return "x";
	case 1 : return "y";
	case 2 : return "F";
	default:
	  return "OOPS";
	}
    }

    string equationToStr(const unsigned int eq) {
      switch(eq) {
	case 0 : return "x² + y² = 1";
	case 1 : return "Fx = der(der(x))";
	case 2 : return "Fy - g = der(der(y))";
	default:
	  return "OOPS";
	}
    }

    /**
     * Set the incidence according to the cartesian pendulum model above
     * variables: x,y,F
     */
    void setIncidence(const unsigned int eq, const incidence_setter& setter) {
      switch(eq) {
	case 0 : {
	  setter(eq, 0, 0);
	  setter(eq, 1, 0);
	  return;
	}
	case 1 : {
	  setter(eq, 0, 2);
	  setter(eq, 2, 0);
	  return;
	}
	case 2 : {
	  setter(eq, 1, 2);
	  setter(eq, 2, 0);
	  return;
	}
	default:
	  return;
	}
    }

    void analyzePendulum() {
      InputProblem pendulum;
      pendulum.dimension = 3;
      pendulum.mkSigma = &setIncidence;

      const AnalysisResult res = pendulum.pryceAlgorithm();      

      /* x and y appear two-times differentiated */
      BOOST_CHECK_EQUAL( res.c, std::vector<int>({2,2,0}) );
      
      /* x² + y² = 1 needs to be two-times differentiated */
      BOOST_CHECK_EQUAL( res.d, std::vector<int>({2,0,0}) );
    }

   
    /**
     * Set the incidence according to the cartesian pendulum as described by Modelica
     * variables: x,y,vx,vy,F
     */
    void setIncidenceModelica(const unsigned int eq, const incidence_setter& setter) {
      switch(eq) {
	//x² + y² ...
	case 0 : {
	  setter(eq, 0, 0);
	  setter(eq, 1, 0);
	  return;
	}
	  //der(x) = vx 
	case 1 : {
	  setter(eq, 0, 1);
	  setter(eq, 2, 0);
	  return;
	}
	  //der(y) = vy 
	case 2 : {
	  setter(eq, 1, 1);
	  setter(eq, 3, 0);
	  return;
	}
	  //Fx = der(vx);
	case 3 : {
	  setter(eq, 0, 0);
	  setter(eq, 2, 1);
	  setter(eq, 4, 0);
	  return;
	}
	  //Fy = der(vy) - g;
	case 4 : {
	  setter(eq, 1, 0);
	  setter(eq, 3, 1);
	  setter(eq, 4, 0);
	  return;
	}
	default:
	  return;
	}
    }

    void analyzeModelicaPendulum() {
      InputProblem pendulum;
      pendulum.dimension = 5;
      pendulum.mkSigma = &setIncidenceModelica;

      const AnalysisResult res = pendulum.pryceAlgorithm();      

      BOOST_CHECK_EQUAL( res.c, std::vector<int>({2,2,1,1,0}) );
      BOOST_CHECK_EQUAL( res.d, std::vector<int>({2,1,1,0,0}) );
    }
    
  }
}
