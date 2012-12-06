// Copyright (c)   2012 David-Matthias Sichau
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

#ifndef UTILIB_TIMER_HPP_
#define UTILIB_TIMER_HPP_

#pragma once
#include <sys/time.h>

namespace UtilLib{

class Timer {
 public:

  /**
   * @brief starts the timer
   */
  void start()
  {
      gettimeofday(&t_start,  &t_zone);
  }

  /**
   * @brief stops the timer and returns the time
   *
   * @return the time since the timer has started
   */
  double stop()
  {
      gettimeofday(&t_end,  &t_zone);
      return (t_end.tv_usec  - t_start.tv_usec)*1e-6  + (t_end.tv_sec  - t_start.tv_sec);
  }
 private:

  /**
   * @brief storage for the start and end time
   */
  struct timeval t_start, t_end;
  /**
   * @brief stores the timezone
   */
  struct timezone t_zone;
};


} /* end namespace  */
#endif /* UTILIB_TIMER_HPP_ */
