// Copyright (c) 	2012 David-Matthias Sichau
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

#ifndef UTILIB_PROGRESSBAR_HPP_
#define UTILIB_PROGRESSBAR_HPP_

#include <iostream>
#include <UtilLib/include/Timer.hpp>

namespace UtilLib{

class ProgressBar {
 public:
  /**
   * Constructor
   * @param expected_count The expected count
   * @param updateInterval how often the information should be updated default this is 30 seconds
   * @param os The output stream
   */
  explicit ProgressBar(unsigned long expected_count, double updateInterval = 30, std::ostream& os = std::cout);
  /**
   * Display appropriate progress tic if needed.
   * @param increment
   * @post count()== original count() + increment
   */
  unsigned long operator+=(unsigned long increment);

  /**
   * Prefix operator
   */
  unsigned long operator++();

  /**
   * Postfix operator
   */
  unsigned long operator++(int);

 private:
  unsigned long count_ = 0;///The current count
  unsigned long expectedCount_;///The number of counts
  unsigned long nextTicCount_ = 0;///When the next tic should be generated
  unsigned long updateCount_ = 0;///When the percentage should be updated
  unsigned short tic_ = 0;///The current tic
  /**
   * @brief the interval in sec at which the percentage and runtime estimation is updated
   */
  const float updateInterval_;
  /**
   * The stream where the progress Bar is printed to.
   */
  std::ostream& outputStream_;

  /**
   * use of floating point ensures that both large and small counts
   * work correctly.  static_cast<>() is also used several places
   * to suppress spurious compiler warnings.
   */
  void displayTic();

  /**
   * @brief updates the percentage.
   */
  void displayPercentage();

  /**
   * @brief The timers where wholeTime_ is used for the overall time
   */
  Timer wholeTime_;
};

} /* end namespace  */
#endif /* UTILIB_PROGRESSBAR_HPP_ */
