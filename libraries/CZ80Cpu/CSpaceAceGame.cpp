//
// Copyright (c) 2021, Warren Ondras
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#include "CSpaceAceGame.h"

//
// See the base game cpp comments for details about this platform.
//


//
// ROMs for version A (spaceacea in MAME)
//
static const ROM_DATA2N s_romData2nSetA[] PROGMEM  = {
    // 01   02   04   08   10   20   40   80  100  200  400  800 1000
    {0x3f,0xe0,0xcc,0xff,0xff,0xff,0xff,0xff,0x21,0x01,0x3e,0xff,0xff}, // sa_a_u1.bin - 0x8eb1889e
    {0xc1,0xe6,0xcb,0x01,0xcd,0x0f,0x4b,0xe6,0x8d,0xe0,0x01,0x13,0x1d}, // sa_a_u2.bin - 0x18d0262d
    {0x00,0x13,0x01,0x84,0x00,0x00,0x1a,0x01,0x1a,0x1c,0x45,0xff,0x01}, // sa_a_u3.bin - 0x4646832d
    {0x1b,0x2f,0x12,0x24,0x01,0x15,0x99,0x1c,0x16,0xff,0x13,0x82,0xff}, // sa_a_u4.bin - 0x57db2a79
    {0x63,0x10,0x24,0x63,0x20,0x80,0x84,0x01,0x12,0x49,0xff,0x32,0x1d}, // sa_a_u5.bin - 0x85cbcdc4
    {0}  // end of list
};
static const ROM_REGION s_romRegion_SetA[] PROGMEM = {
    {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nSetA[0].data2n, 0x8eb1889e, "rU1"}, // sa_a_u1.bin
    {NO_BANK_SWITCH, 0x2000, 0x2000, s_romData2nSetA[1].data2n, 0x18d0262d, "rU2"}, // sa_a_u2.bin
    {NO_BANK_SWITCH, 0x4000, 0x2000, s_romData2nSetA[2].data2n, 0x4646832d, "rU3"}, // sa_a_u3.bin
    {NO_BANK_SWITCH, 0x6000, 0x2000, s_romData2nSetA[3].data2n, 0x57db2a79, "rU4"}, // sa_a_u4.bin
    {NO_BANK_SWITCH, 0x8000, 0x2000, s_romData2nSetA[4].data2n, 0x85cbcdc4, "rU5"}, // sa_a_u5.bin
    {0}  // end of list
};

//
// ROMs for version A2 (spaceacea2 in MAME)
//
static const ROM_DATA2N s_romData2nSetA2[] PROGMEM  = {
    // 01   02   04   08   10   20   40   80  100  200  400  800 1000
    {0x3f,0xe0,0xcc,0xff,0xff,0xff,0xff,0xff,0x21,0x01,0x3e,0xff,0xff}, // sa_a2_u1.bin - 0x71b39e27
    {0xc1,0xe6,0xcb,0x01,0xcd,0x0f,0x4b,0xe6,0x8d,0xe0,0x01,0x13,0x1d}, // sa_a2_u2.bin - 0x18d0262d
    {0x00,0x13,0x01,0x84,0x00,0x00,0x1a,0x01,0x1a,0x1c,0x45,0xff,0x01}, // sa_a2_u3.bin - 0x4646832d
    {0x1b,0x2f,0x12,0x24,0x01,0x15,0x99,0x1c,0x16,0xff,0x13,0x82,0xff}, // sa_a2_u4.bin - 0x57db2a79
    {0x63,0x10,0x24,0x63,0x20,0x80,0x84,0x01,0x12,0x49,0xff,0x32,0x1d}, // sa_a2_u5.bin - 0x85cbcdc4
    {0}  // end of list
};
static const ROM_REGION s_romRegion_SetA2[] PROGMEM = {
    {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nSetA2[0].data2n, 0x71b39e27, "rU1"}, // sa_a2_u1.bin
    {NO_BANK_SWITCH, 0x2000, 0x2000, s_romData2nSetA2[1].data2n, 0x18d0262d, "rU2"}, // sa_a2_u2.bin
    {NO_BANK_SWITCH, 0x4000, 0x2000, s_romData2nSetA2[2].data2n, 0x4646832d, "rU3"}, // sa_a2_u3.bin
    {NO_BANK_SWITCH, 0x6000, 0x2000, s_romData2nSetA2[3].data2n, 0x57db2a79, "rU4"}, // sa_a2_u4.bin
    {NO_BANK_SWITCH, 0x8000, 0x2000, s_romData2nSetA2[4].data2n, 0x85cbcdc4, "rU5"}, // sa_a2_u5.bin
    {0}  // end of list
};

//
// ROMs for version A3 (spaceace in MAME)
//
static const ROM_DATA2N s_romData2nSetA3[] PROGMEM  = {
    // program ROMs
    // 01   02   04   08   10   20   40   80  100  200  400  800 1000
    {0x3f,0xe0,0xcc,0xff,0xff,0xff,0xff,0xff,0x21,0x01,0x3e,0xff,0xff}, // sa_a3_u1.bin - 0x427522d0
    {0xc1,0xe6,0xcb,0x01,0xcd,0x0f,0x4b,0xe6,0x8d,0xe0,0x01,0x13,0x1d}, // sa_a3_u2.bin - 0x18d0262d
    {0x00,0x13,0x01,0x84,0x00,0x00,0x1a,0x01,0x1a,0x1c,0x45,0xff,0x01}, // sa_a3_u3.bin - 0x4646832d
    {0x1b,0x2f,0x12,0x24,0x01,0x15,0x99,0x1c,0x16,0xff,0x13,0x82,0xff}, // sa_a3_u4.bin - 0x57db2a79
    {0x63,0x10,0x24,0x63,0x20,0x80,0x84,0x01,0x12,0x49,0xff,0x32,0x1d}, // sa_a3_u5.bin - 0x85cbcdc4
    {0}  // end of list
};
static const ROM_REGION s_romRegion_SetA3[] PROGMEM = {
    {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nSetA3[0].data2n, 0x427522d0, "rU1"}, // sa_a3_u1.bin
    {NO_BANK_SWITCH, 0x2000, 0x2000, s_romData2nSetA3[1].data2n, 0x18d0262d, "rU2"}, // sa_a3_u2.bin
    {NO_BANK_SWITCH, 0x4000, 0x2000, s_romData2nSetA3[2].data2n, 0x4646832d, "rU3"}, // sa_a3_u3.bin
    {NO_BANK_SWITCH, 0x6000, 0x2000, s_romData2nSetA3[3].data2n, 0x57db2a79, "rU4"}, // sa_a3_u4.bin
    {NO_BANK_SWITCH, 0x8000, 0x2000, s_romData2nSetA3[4].data2n, 0x85cbcdc4, "rU5"}, // sa_a3_u5.bin
    {0} // end of list
};

//
// ROMs for Space Ace Enahancement version 1.0 (unofficial mod, not in MAME)
// http://www.dragons-lair-project.com/tech/enhancements/sae.asp
//
static const ROM_DATA2N s_romData2nSetSAE10[] PROGMEM  = {
    // program ROMs
    // 01   02   04   08   10   20   40   80  100  200  400  800 1000
    {0x3f,0xe0,0xcc,0xff,0xff,0xff,0x20,0x41,0x21,0x01,0x3e,0x21,0xff}, // SAE10_U1.bin - 0xcbc5e425
    {0x70,0x61,0x65,0x65,0x6e,0x2d,0x62,0x74,0x20,0x2e,0x01,0x01,0x00}, // SAE10_U2.bin - 0x71a26f47
    {0x70,0x61,0x65,0x65,0x6e,0x2d,0x62,0x74,0x20,0x2e,0x17,0x13,0x23}, // SAE10_U3.bin - 0xbac5cdd8
    {0x70,0x61,0x65,0x65,0x6e,0x2d,0x62,0x74,0x20,0x2e,0x5e,0x0a,0x00}, // SAE10_U4.bin - 0xe18380f9
    {0x70,0x61,0x65,0x65,0x6e,0x2d,0x62,0x74,0x20,0x2e,0x11,0x17,0xff}, // SAE10_U5.bin - 0x8a536cb0
    {0}  // end of list
};
static const ROM_REGION s_romRegion_SetSAE10[] PROGMEM = {
    {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nSetSAE10[0].data2n, 0xcbc5e425, "rU1"}, // SAE10_U1.bin
    {NO_BANK_SWITCH, 0x2000, 0x2000, s_romData2nSetSAE10[1].data2n, 0x71a26f47, "rU2"}, // SAE10_U2.bin
    {NO_BANK_SWITCH, 0x4000, 0x2000, s_romData2nSetSAE10[2].data2n, 0xbac5cdd8, "rU3"}, // SAE10_U3.bin
    {NO_BANK_SWITCH, 0x6000, 0x2000, s_romData2nSetSAE10[3].data2n, 0xe18380f9, "rU4"}, // SAE10_U4.bin
    {NO_BANK_SWITCH, 0x8000, 0x2000, s_romData2nSetSAE10[4].data2n, 0x8a536cb0, "rU5"}, // SAE10_U5.bin
    {0} // end of list
};



IGame*
CSpaceAceGame::createInstanceSetA(
                                   )
{
    return (new CSpaceAceGame(s_romData2nSetA, s_romRegion_SetA));
}

IGame*
CSpaceAceGame::createInstanceSetA2(
                                   )
{
    return (new CSpaceAceGame(s_romData2nSetA2, s_romRegion_SetA2));
}

IGame*
CSpaceAceGame::createInstanceSetA3(
                                   )
{
    return (new CSpaceAceGame(s_romData2nSetA3, s_romRegion_SetA3));
}

IGame*
CSpaceAceGame::createInstanceSetSAE10(
                                      )
{
    return (new CSpaceAceGame(s_romData2nSetSAE10, s_romRegion_SetSAE10));
}

CSpaceAceGame::CSpaceAceGame(
                             const ROM_DATA2N *romData2n,
                             const ROM_REGION *romRegion
                             ) : CDragonsLairBaseGame( romData2n, romRegion )
{
}


