/**************************************************************************************************
 * Copyright (c) 2019 Xiaoyang Xu, Masoud Ghaffarinia, Wenhao Wang, Kevin W. Hamlen, Zhiqiang Lin *
 *                                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software  *
 * and associated documentation files (the "Software"), to deal in the Software without           * 
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,     *
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the  *
 * Software is furnished to do so, subject to the following conditions:                           *
 *                                                                                                *
 * The above copyright notice and this permission notice shall be included in all copies or       *
 * substantial portions of the Software.                                                          *
 *                                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING  *
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND     *
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.        *
 **************************************************************************************************/

#include "helper.h"

NANOSECOND start_time;
NANOSECOND end_time;
NANOSECOND total_time;

//A function does nothing but return
void retFunc();

int main()
{
    // Record starting time
    start_time = get_wall_time();

	// Call a function that does nothing but returns in an intensive loop.
    for (long long int i = 0; i < MAX_LOOP * RETTS; i++)
    {
        #ifdef _WIN32
        retFunc();
        #elif __linux__
        __asm__ ("call retFunc");
        #else
        #endif
    }

    // Record ending time
    end_time = get_wall_time();
    total_time = end_time - start_time;

    // Print time elapsed in the loop.
    printf("total time in nanoseconds is %llu\n", (long long unsigned int) total_time);

    return 0;
}

#ifdef _WIN32
__declspec(naked)
void retFunc()
{
    __asm { ret }
}
#elif __linux__
__asm__ (
".globl retFunc\n"
"    .type retFunc, @function\n"
"    retFunc:\n"
"    ret\n");
#else
#endif
