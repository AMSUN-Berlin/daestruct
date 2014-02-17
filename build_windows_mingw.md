Requirements:

	* MinGW
		* Download from http://www.mingw.org/
		* Install MinGW, leave everything on default		
			* The MinGW Installation Manager will open. Under Basic Setup select
				* mingw32-base
				* mingw32-gcc-g++
			* Click Installation -> Apply Changes. Click Apply
			
		* Add MinGW bin folder to PATH, for example: C:\MinGW\bin
		  See also http://www.mingw.org/wiki/Getting_Started under Environment Settings


	* Boost 1.55
	    * Download it from http://sourceforge.net/projects/boost/files/boost/1.55.0/
		* Unpack it to: 
		  C:\boost
		  So the path looks like this: 
		  C:\boost\boost_1_55_0
		* Open a command prompt
			* cd C:\boost\boost_1_55_0\
			* bootstrap mingw
			* b2 toolset=gcc
			  optional you can append -jN where N is the numbers of jobs to run, i.e. your cores
		* Wait
		
		  
	* CMake
		Download the Windows installer from
		http://www.cmake.org/cmake/resources/software.html
		When asked, selected add to PATH
		
		
Building daestruct:

	* Clone the git rep from: https://github.com/choeger/daestruct
		
	* Start a command prompt
		* cd <path to daestruct>
		* mkdir build
		* cd build
		* cmake .. -G "MinGW Makefiles"
		* mingw32-make
	