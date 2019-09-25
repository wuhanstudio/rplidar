/*
 *  RPLIDAR SDK
 *
 *  Copyright (c) 2009 - 2014 RoboPeak Team
 *  http://www.robopeak.com
 *  Copyright (c) 2014 - 2019 Shanghai Slamtec Co., Ltd.
 *  http://www.slamtec.com
 *
 */
/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __RP_TYPES_H__
#define __RP_TYPES_H__

#include <stdint.h>

//based on stdint.h
typedef int8_t         _s8;
typedef uint8_t        _u8;

typedef int16_t        _s16;
typedef uint16_t       _u16;

typedef int32_t        _s32;
typedef uint32_t       _u32;

typedef int64_t        _s64;
typedef uint64_t       _u64;

#define __small_endian

typedef uint32_t u_result;

#define RESULT_OK              0
#define RESULT_FAIL_BIT        0x80000000
#define RESULT_ALREADY_DONE    0x20
#define RESULT_INVALID_DATA    (0x8000 | RESULT_FAIL_BIT)
#define RESULT_OPERATION_FAIL  (0x8001 | RESULT_FAIL_BIT)
#define RESULT_OPERATION_TIMEOUT  (0x8002 | RESULT_FAIL_BIT)
#define RESULT_OPERATION_STOP    (0x8003 | RESULT_FAIL_BIT)
#define RESULT_OPERATION_NOT_SUPPORT    (0x8004 | RESULT_FAIL_BIT)
#define RESULT_FORMAT_NOT_SUPPORT    (0x8005 | RESULT_FAIL_BIT)
#define RESULT_INSUFFICIENT_MEMORY   (0x8006 | RESULT_FAIL_BIT)

#define IS_OK(x)    ( ((x) & RESULT_FAIL_BIT) == 0 )
#define IS_FAIL(x)  ( ((x) & RESULT_FAIL_BIT) )

// typedef _word_size_t (THREAD_PROC * thread_proc_t ) ( void * );

#endif // __RP_TYPES_H__
