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

#include <UtilLib/include/ProgressBar.hpp>
#include <UtilLib/include/MPIProxy.hpp>
#include <iomanip>

namespace UtilLib {

ProgressBar::ProgressBar(unsigned long expectedCount,
                         const std::string & description, std::ostream& os) :
    description_(description), outputStream_(os) {
    if (MPIProxy().getRank() == 0) {
        restart(expectedCount);
    }
}

void ProgressBar::restart(unsigned long expected_count) {
    count_ = nextTicCount_ = tic_ = 0;
    expectedCount_ = expected_count;
    outputStream_ << description_ << "\n"
                  << "progress: "<<std::setw(4)<<std::setprecision(3)<< static_cast<float>(tic_)<<"%   estimated time remaining:"<< std::setw(6) << std::setprecision(3) <<"inf sec\n"
                  << "0%   10   20   30   40   50   60   70   80   90   100%\n"
                  << "|----|----|----|----|----|----|----|----|----|----|"
                  << std::endl;
    timer_.start();

}

unsigned long ProgressBar::operator+=(unsigned long increment) {
    if (MPIProxy().getRank() == 0) {
        if ((count_ += increment) >= nextTicCount_) {
            displayTic();
        }
    }
    return count_;
}

unsigned long ProgressBar::operator++() {
    return operator+=(1);
}

unsigned long ProgressBar::operator++(int) {
    return operator+=(1);
}

void ProgressBar::displayTic() {
    unsigned int tics_needed =
            static_cast<unsigned int>((static_cast<double>(count_)
                                       / expectedCount_) * 50.0);
    const double dt = timer_.stop();
    const double remainingTime= (50 - tics_needed)*dt;
    /*
      * I use the following terminal control sequences:
      * \x1b[s stores the current cursor position
      * \x1b[A goes one line up
      * \r goes to the beginning of the line
      * \x1b[K deletes form the cursor position to the end
      * \x1b[u restores the cursor position
      */
    outputStream_ << "\x1b[s\x1b[A\x1b[A\x1b[A\r"<<"progress: " << std::setw(4) << std::setprecision(3)
                  << static_cast<float>(count_)/(expectedCount_)*100 << "%   "
                  << "estimated time remaining:"<< std::setw(6) << std::setprecision(3) << remainingTime<<" sec"<<"\x1b[K\x1b[u";
    timer_.start();

    do {
        outputStream_ << '*' << std::flush;
    } while (++tic_ < tics_needed);

    nextTicCount_ = static_cast<unsigned long>((tic_ / 50.0) * expectedCount_);

    if (count_ == expectedCount_) {


        outputStream_ << "\x1b[s\x1b[A\x1b[A\x1b[A\r"<<"progress: " << std::setw(4) << std::setprecision(3)
                  << static_cast<float>(100) << "%   "
                  << "estimated time remaining:"<< std::setw(6) << std::setprecision(3) << 0.0<<" sec"<<"\x1b[K\x1b[u";
       if (tic_ < 51)
            outputStream_ << '*';
        outputStream_ << std::endl;
    }
}

} /* end namespace  */
