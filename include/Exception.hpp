/* Copyright (c) 2005 - 2012 Marc de Kamps
 *               2010 Marc Kirchner
 *               2013 David Sichau <mail"at"sichau"dot"eu>
 *               2013 Simon Tanaka <tanakas"at"gmx"dot"ch>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef UTILIB_EXCEPTION_HPP_
#define UTILIB_EXCEPTION_HPP_


#include <exception>
#include <sstream>
#include <stdexcept>
#include <string>

namespace UtilLib {
/**
 * miind exception base class.
 */
class Exception : public std::exception {

 public:
    /** Constructor for C-style string error messages.
     *  @param message C-style string error message.
     *                 The string contents are copied upon construction
     *                 Responsibility for deleting the \c char* lies
     *                 with the caller.
     */
    explicit Exception(const char* message);

    /** Constructor for STL string class error messages.
     *  @param message The error message.
     */
    explicit Exception(const std::string& message);

    /** Constructur for STL string class error messages with file and line number.
     *  @param message an error message.
     *  @param file a file name.
     *  @param line a line number.
     */
    explicit Exception(const std::string& message,const char *file, int line);

    /** Destructor. Nothrow guarantee.
     * Virtual to allow for subclassing.
     */
    virtual ~Exception() throw();

    /** Returns a pointer to the constant error message.
     *  @return A pointer to a \c const \c char*. The underlying memory
     *          is in posession of the \c Exception object. Callers \a must
     *          not attempt to free the memory.
     */
    virtual const char* what() const throw();

 protected:
    /** Error message.
     */
    std::string msg_;
};




}  // end namespace






/** Convenience macros.
 */
#define lbm_fail(ERROR_MESSAGE) throw(UtilLib::Exception(ERROR_MESSAGE, __FILE__, __LINE__))

#endif  // UTILIB_EXCEPTION_HPP_
