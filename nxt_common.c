/*
 * IDEA NXT encryption algorithm implementation
 * Issue date: 02/25/2006
 *
 * Copyright (C) 2006 Olivier Gay <olivier.gay@a3.epfl.ch>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <string.h>

#include "nxt_common.h"

const uint8 pad[32] = {
    0xb7, 0xe1, 0x51, 0x62, 0x8a, 0xed, 0x2a, 0x6a, 0xbf, 0x71, 0x58, 0x80,
    0x9c, 0xf4, 0xf3, 0xc7, 0x62, 0xe7, 0x16, 0x0f, 0x38, 0xb4, 0xda, 0x56,
    0xa7, 0x84, 0xd9, 0x04, 0x51, 0x90, 0xcf, 0xef
};

#if ((defined NXT64_INIT_TABLES) || (defined NXT128_INIT_TABLES))
const uint8 sbox[256] = {
    0x5d, 0xde, 0x00, 0xb7, 0xd3, 0xca, 0x3c, 0x0d, 0xc3, 0xf8, 0xcb, 0x8d,
    0x76, 0x89, 0xaa, 0x12, 0x88, 0x22, 0x4f, 0xdb, 0x6d, 0x47, 0xe4, 0x4c,
    0x78, 0x9a, 0x49, 0x93, 0xc4, 0xc0, 0x86, 0x13, 0xa9, 0x20, 0x53, 0x1c,
    0x4e, 0xcf, 0x35, 0x39, 0xb4, 0xa1, 0x54, 0x64, 0x03, 0xc7, 0x85, 0x5c,
    0x5b, 0xcd, 0xd8, 0x72, 0x96, 0x42, 0xb8, 0xe1, 0xa2, 0x60, 0xef, 0xbd,
    0x02, 0xaf, 0x8c, 0x73, 0x7c, 0x7f, 0x5e, 0xf9, 0x65, 0xe6, 0xeb, 0xad,
    0x5a, 0xa5, 0x79, 0x8e, 0x15, 0x30, 0xec, 0xa4, 0xc2, 0x3e, 0xe0, 0x74,
    0x51, 0xfb, 0x2d, 0x6e, 0x94, 0x4d, 0x55, 0x34, 0xae, 0x52, 0x7e, 0x9d,
    0x4a, 0xf7, 0x80, 0xf0, 0xd0, 0x90, 0xa7, 0xe8, 0x9f, 0x50, 0xd5, 0xd1,
    0x98, 0xcc, 0xa0, 0x17, 0xf4, 0xb6, 0xc1, 0x28, 0x5f, 0x26, 0x01, 0xab,
    0x25, 0x38, 0x82, 0x7d, 0x48, 0xfc, 0x1b, 0xce, 0x3f, 0x6b, 0xe2, 0x67,
    0x66, 0x43, 0x59, 0x19, 0x84, 0x3d, 0xf5, 0x2f, 0xc9, 0xbc, 0xd9, 0x95,
    0x29, 0x41, 0xda, 0x1a, 0xb0, 0xe9, 0x69, 0xd2, 0x7b, 0xd7, 0x11, 0x9b,
    0x33, 0x8a, 0x23, 0x09, 0xd4, 0x71, 0x44, 0x68, 0x6f, 0xf2, 0x0e, 0xdf,
    0x87, 0xdc, 0x83, 0x18, 0x6a, 0xee, 0x99, 0x81, 0x62, 0x36, 0x2e, 0x7a,
    0xfe, 0x45, 0x9c, 0x75, 0x91, 0x0c, 0x0f, 0xe7, 0xf6, 0x14, 0x63, 0x1d,
    0x0b, 0x8b, 0xb3, 0xf3, 0xb2, 0x3b, 0x08, 0x4b, 0x10, 0xa6, 0x32, 0xb9,
    0xa8, 0x92, 0xf1, 0x56, 0xdd, 0x21, 0xbf, 0x04, 0xbe, 0xd6, 0xfd, 0x77,
    0xea, 0x3a, 0xc8, 0x8f, 0x57, 0x1e, 0xfa, 0x2b, 0x58, 0xc5, 0x27, 0xac,
    0xe3, 0xed, 0x97, 0xbb, 0x46, 0x05, 0x40, 0x31, 0xe5, 0x37, 0x2c, 0x9e,
    0x0a, 0xb1, 0xb5, 0x06, 0x6c, 0x1f, 0xa3, 0x2a, 0x70, 0xff, 0xba, 0x07,
    0x24, 0x16, 0xc6, 0x61
};

uint8 nxt_alpha_mul(uint8 x)
{
    uint8 ret;

    if (x & 0x80)
        ret = (uint8) (((uint16) x << 1) ^ IRRED_POLY);
    else
        ret = x << 1;

    return ret;
}

uint8 nxt_alpha_div(uint8 x)
{
    uint8 ret;

    if (x & 0x01)
        ret = (uint8) (((uint16) x ^ IRRED_POLY) >> 1);
    else
        ret = x >> 1;

    return ret;
}
#endif

void nxt_p(const uint8 *key, uint8 l, uint8 *pkey, uint16 ek)
{
    memcpy(pkey, key, l);
    memcpy(pkey + l, pad, (ek >> 3) - l);
}

void nxt_m(const uint8 *pkey, uint8 *mkey, uint16 ek)
{
    const uint8 mkey_m2 = 0x6a;
    const uint8 mkey_m1 = 0x76;
    int bound;
    int i;

    bound = (ek >> 3) - 1;

    mkey[0] = pkey[0] ^ (mkey_m1 + mkey_m2);
    mkey[1] = pkey[1] ^ (mkey[0] + mkey_m1);

    for (i = 2; i <= bound; i++) {
        mkey[i] = pkey[i] ^ (mkey[i - 1] + mkey[i - 2]);
    }
}

