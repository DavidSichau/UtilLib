// Copyright (c) 2005 - 2012 Marc de Kamps
//						2012 David-Matthias Sichau
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation
//      and/or other materials provided with the distribution.
//    * Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software
//      without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
// USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//


#include <UtilLib/config.hpp>
#ifdef ENABLE_MPI
#include <boost/mpi/communicator.hpp>
#endif
#include <UtilLib/include/MPIProxy.hpp>
#define private public
#define protected public
#include <UtilLib/include/ProgressBar.hpp>
#undef protected
#undef private
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <boost/test/minimal.hpp>
using namespace boost::unit_test;
using namespace UtilLib;


void test_Constructor() {

    std::stringstream os;

    ProgressBar pb(100, 0.5, os);

    if (MPIProxy().getRank() == 0) {
        BOOST_CHECK(pb.expectedCount_==100);
    }
    BOOST_CHECK(pb.updateInterval_==0.5);

    if (MPIProxy().getRank() == 0) {

        std::stringstream tempStream;
        tempStream <<"progress: "<<std::setw(4)<<std::setprecision(3)<< static_cast<float>(0)<<"%   estimated time remaining:"<< std::setw(6) << std::setprecision(3) <<"inf sec\n"
                << "0%   10   20   30   40   50   60   70   80   90   100%\n"
                << "|----|----|----|----|----|----|----|----|----|----|"
                << std::endl;
        BOOST_CHECK(os.str()==tempStream.str());
        pb++;
        pb++;
        pb++;
        pb++;
        std::string s = os.str();
        BOOST_CHECK(std::count(s.begin(), s.end(),'*')==3);
        ++pb;
        ++pb;
        ++pb;
        s = os.str();
        BOOST_CHECK(std::count(s.begin(), s.end(), '*')==4);
        pb+=50;
        s = os.str();
        BOOST_CHECK(std::count(s.begin(), s.end(), '*')==28);

    }
}

int test_main(int argc, char* argv[]) // note the name!
        {

#ifdef ENABLE_MPI
    boost::mpi::environment env(argc, argv);

    // we use only two processors for this testing
    if (MPIProxy().getSize() != 2) {
        BOOST_FAIL( "Run the test with two processes!");
    }
#endif
    // we use only two processors for this testing
    test_Constructor();

    return 0;

}

