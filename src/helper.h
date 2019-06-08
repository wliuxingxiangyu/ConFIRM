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

#pragma once

// Benchmarks in Windows and Linux require different header files
#ifdef _WIN32
#include <iostream>
#include <windows.h>
#include <time.h>
#include <random>

#elif __linux__
#include <iostream>
#include <time.h>
#include <stdio.h> 
#include <limits.h>
#include <stdint.h>
#include <cstdlib>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdarg.h>
#define FALSE false
#define BYTE unsigned char

#else
#endif

// Loop iteration counts for different benchmarks.
// With the current counts, each benchmark runs around 2 min.
#define MAX_LOOP 1024L //base loop count
#define CPEHTS 5       //cppeh
#define SEHTS 5       //cppeh
#define DYNLTS 0.3     //run_time_dynlnk
#define FPTRTS 500     //fptr
//#define IMPDTS 6500    //data_symbl
#define INDCTS 360     //tail_call
#define JITCTS 2500
#define CALTS 0.4      //callback
#define LHWXTS 20      //signal
#define LTTX 1.2       //load_time_dynlnk
#define RETTS 5700     //ret
#define SWTCTS 590     //switch
#define VTABTS 460     //vtbl_call
#define DLTS 500

// Macros for different calling conventions in Windows and Linux
#ifdef _WIN32
#define CDECL __cdecl
#define STDCALL __stdcall
#define FASTCALL __fastcall
#define THISCALL __thiscall

#elif __linux__
#define CDECL __attribute__((cdecl))
#define STDCALL
#define FASTCALL __attribute__((fastcall))
#define THISCALL __attribute__((thiscall))

#else
#endif

// A helper function that can be used to calculate elapsed time in nanosecond.
#define NANOSECOND uint64_t
#define BILLION 1000000000L
extern NANOSECOND get_wall_time();

