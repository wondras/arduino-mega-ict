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
#include "CDragonsLairGame.h"

//
// See the base game cpp comments for details about this platform.
//


//
// ROMs for version F
//
static const ROM_DATA2N s_romData2nSetF[] PROGMEM  = {
    // 01   02   04   08   10   20   40   80  100  200  400  800 1000
    {0x47,0x11,0xff,0xff,0xff,0xff,0x00,0x7e,0x21,0x01,0x04,0xff,0xff}, // dl_f_u1.bin - 0x06fc6941
    {0x06,0x08,0x09,0x00,0x00,0x01,0x07,0x88,0x0a,0x53,0x0a,0x0b,0x4a}, // dl_f_u2.bin - 0xdcc1dff2
    {0x00,0x00,0x65,0x04,0x00,0x16,0x66,0x5f,0x69,0x01,0x06,0x04,0x51}, // dl_f_u3.bin - 0xab514e5b
    {0x10,0x17,0x00,0x10,0x00,0x80,0x88,0x0f,0x40,0x64,0x00,0x18,0xff}, // dl_f_u4.bin - 0xa817324e
    {0}  // end of list
};

static const ROM_REGION s_romRegion_SetF[] PROGMEM = {
    {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nSetF[0].data2n, 0x06fc6941, "rU1"}, // dl_f_u1.bin
    {NO_BANK_SWITCH, 0x2000, 0x2000, s_romData2nSetF[1].data2n, 0xdcc1dff2, "rU2"}, // dl_f_u2.bin
    {NO_BANK_SWITCH, 0x4000, 0x2000, s_romData2nSetF[2].data2n, 0xab514e5b, "rU3"}, // dl_f_u3.bin
    {NO_BANK_SWITCH, 0x6000, 0x2000, s_romData2nSetF[3].data2n, 0xa817324e, "rU4"}, // dl_f_u4.bin
    {0}  // end of list
};


//
// ROMs for version F2 (dlair from MAME)
//
static const ROM_DATA2N s_romData2nSetF2[] PROGMEM  = {
    // 01   02   04   08   10   20   40   80  100  200  400  800 1000
    {0x47,0x11,0xff,0xff,0xff,0xff,0x00,0x7e,0x21,0x01,0x04,0xff,0xff}, // dl_f2_u1.bin - 0xf5ea3b9d
    {0x06,0x08,0x09,0x00,0x00,0x01,0x07,0x88,0x0a,0x53,0x0a,0x0b,0x4a}, // dl_f2_u2.bin - 0xdcc1dff2
    {0x00,0x00,0x65,0x04,0x00,0x16,0x66,0x5f,0x69,0x01,0x06,0x04,0x51}, // dl_f2_u3.bin - 0xab514e5b
    {0x10,0x17,0x00,0x10,0x00,0x80,0x88,0x0f,0x40,0x64,0x00,0x18,0xff}, // dl_f2_u4.bin - 0xf5ec23d2
    {0}  // end of list
};

static const ROM_REGION s_romRegion_SetF2[] PROGMEM = {
    {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nSetF2[0].data2n, 0xf5ea3b9d, "rU1"}, // dl_f2_u1.bin
    {NO_BANK_SWITCH, 0x2000, 0x2000, s_romData2nSetF2[1].data2n, 0xdcc1dff2, "rU2"}, // dl_f2_u2.bin
    {NO_BANK_SWITCH, 0x4000, 0x2000, s_romData2nSetF2[2].data2n, 0xab514e5b, "rU3"}, // dl_f2_u3.bin
    {NO_BANK_SWITCH, 0x6000, 0x2000, s_romData2nSetF2[3].data2n, 0xf5ec23d2, "rU4"}, // dl_f2_u4.bin
    {0}  // end of list
};


//
// ROMs for Dragons Lair Enhancement v2.1 (not in MAME)
//                                                01   02   04   08   10   20   40   80   100  200  400  800  1000
static const ROM_DATA2N s_romData2nSetDLE21[] PROGMEM  = {
    // 01   02   04   08   10   20   40   80  100  200  400  800 1000
    {0xa8,0x0b,0xff,0xff,0xff,0xff,0x00,0x7e,0x21,0x01,0x04,0xcb,0xff}, // DLE21_U1.BIN - 0x4f8af481
    {0x72,0x61,0x6f,0x20,0x45,0x53,0x30,0x63,0x20,0x00,0x04,0x80,0x59}, // DLE21_U2.BIN - 0x5fca04c5
    {0x72,0x61,0x6f,0x20,0x45,0x53,0x30,0x63,0x20,0x0b,0x45,0x09,0x91}, // DLE21_U3.BIN - 0xc14f36b3
    {0}  // end of list
};
static const ROM_REGION s_romRegion_SetDLE21[] PROGMEM = {
    {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nSetDLE21[0].data2n, 0x4f8af481, "rU1"}, // DLE21_U1.bin
    {NO_BANK_SWITCH, 0x2000, 0x2000, s_romData2nSetDLE21[1].data2n, 0x5fca04c5, "rU2"}, // DLE21_U2.bin
    {NO_BANK_SWITCH, 0x4000, 0x2000, s_romData2nSetDLE21[2].data2n, 0xc14f36b3, "rU3"}, // DLE21_U3.bin
    {0}  // end of list
};


IGame*
CDragonsLairGame::createInstanceSetF(
                                     )
{
    return (new CDragonsLairGame(s_romRegion_SetF));
}

IGame*
CDragonsLairGame::createInstanceSetF2(
                                      )
{
    return (new CDragonsLairGame(s_romRegion_SetF2));
}

IGame*
CDragonsLairGame::createInstanceSetDLE21(
                                         )
{
    return (new CDragonsLairGame(s_romRegion_SetDLE21));
}


CDragonsLairGame::CDragonsLairGame(
                                   const ROM_REGION *romRegion
                                   ) : CDragonsLairBaseGame( romRegion )
{
}


