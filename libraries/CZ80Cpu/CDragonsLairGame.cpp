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
// ROMs for version A (PR-7820 only; dlaira in MAME)
//
static const ROM_DATA2N s_romData2nSetA[] PROGMEM  = {
    // 01   02   04   08   10   20   40   80  100  200  400  800 1000
    {0x03,0x20,0xff,0xff,0xff,0xff,0xff,0xff,0x21,0x9d,0x00,0xff,0x3f}, // dl_a_u1.bin - 0xd76e83ec
    {0x6f,0x26,0xff,0xa0,0x00,0xe0,0x78,0xcd,0x28,0xcb,0x29,0xc9,0xe5}, // dl_a_u2.bin - 0xa6a723d8
    {0x13,0x18,0x0b,0x3b,0x18,0xe0,0x78,0xcd,0x28,0xcb,0x29,0xeb,0x41}, // dl_a_u3.bin - 0x52c59014
    {0x10,0x17,0x00,0x10,0x17,0x00,0x01,0x11,0x01,0x00,0x8a,0xeb,0xc3}, // dl_a_u4.bin - 0x924d12f2
    {0x81,0x3a,0x41,0x91,0xdb,0x20,0x25,0x0f,0xd8,0x0e,0x00,0x26,0xb8}, // dl_a_u5.bin - 0x6ec2f9c1
    {0}  // end of list
};
static const ROM_REGION s_romRegion_SetA[] PROGMEM = {
    {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nSetA[0].data2n, 0xd76e83ec, "rU1"}, // dl_a_u1.bin
    {NO_BANK_SWITCH, 0x2000, 0x2000, s_romData2nSetA[1].data2n, 0xa6a723d8, "rU2"}, // dl_a_u2.bin
    {NO_BANK_SWITCH, 0x4000, 0x2000, s_romData2nSetA[2].data2n, 0x52c59014, "rU3"}, // dl_a_u3.bin
    {NO_BANK_SWITCH, 0x6000, 0x2000, s_romData2nSetA[3].data2n, 0x924d12f2, "rU4"}, // dl_a_u4.bin
    {NO_BANK_SWITCH, 0x8000, 0x2000, s_romData2nSetA[4].data2n, 0x6ec2f9c1, "rU5"}, // dl_a_u5.bin
    {0}  // end of list
};

//
// ROMs for version B (PR-7820 only; dlairb in MAME)
//
static const ROM_DATA2N s_romData2nSetB[] PROGMEM  = {
    // 01   02   04   08   10   20   40   80  100  200  400  800 1000
    {0x03,0x20,0xff,0xff,0xff,0xff,0xff,0xff,0x21,0x9d,0x00,0xff,0x3f}, // dl_b_u1.bin - 0xd76e83ec
    {0x6f,0x26,0xff,0xa0,0x00,0xe0,0x78,0xcd,0x28,0xcb,0x29,0xc9,0xe5}, // dl_b_u2.bin - 0x6751103d
    {0x13,0x18,0x0b,0x3b,0x18,0xe0,0x78,0xcd,0x28,0xcb,0x29,0xeb,0x41}, // dl_b_u3.bin - 0x52c59014
    {0x10,0x17,0x00,0x10,0x17,0x00,0x01,0x11,0x01,0x00,0x8a,0xeb,0xc3}, // dl_b_u4.bin - 0x924d12f2
    {0x81,0x3a,0x41,0x91,0xdb,0x20,0x25,0x0f,0xd8,0x0e,0x00,0x26,0xb8}, // dl_b_u5.bin - 0x6ec2f9c1
    {0}  // end of list
};
static const ROM_REGION s_romRegion_SetB[] PROGMEM = {
    {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nSetB[0].data2n, 0xd76e83ec, "rU1"}, // dl_b_u1.bin
    {NO_BANK_SWITCH, 0x2000, 0x2000, s_romData2nSetB[1].data2n, 0x6751103d, "rU2"}, // dl_b_u2.bin
    {NO_BANK_SWITCH, 0x4000, 0x2000, s_romData2nSetB[2].data2n, 0x52c59014, "rU3"}, // dl_b_u3.bin
    {NO_BANK_SWITCH, 0x6000, 0x2000, s_romData2nSetB[3].data2n, 0x924d12f2, "rU4"}, // dl_b_u4.bin
    {NO_BANK_SWITCH, 0x8000, 0x2000, s_romData2nSetB[4].data2n, 0x6ec2f9c1, "rU5"}, // dl_b_u5.bin
    {0}  // end of list
};

//
// ROMs for version C (PR-7820 only; dlairc in MAME)
//
static const ROM_DATA2N s_romData2nSetC[] PROGMEM  = {
    // 01   02   04   08   10   20   40   80  100  200  400  800 1000
    {0x03,0x20,0xaa,0x55,0xaa,0x55,0xaa,0x55,0x21,0x9d,0x00,0x55,0x3f}, // dl_c_u1.bin - 0xcebfe26a
    {0x6f,0x26,0xff,0xa0,0x00,0xe0,0x78,0xcd,0x28,0xcb,0x29,0xc9,0xe5}, // dl_c_u2.bin - 0x6751103d
    {0x13,0x18,0x0b,0x3b,0x18,0xe0,0x78,0xcd,0x28,0xcb,0x29,0xeb,0x41}, // dl_c_u3.bin - 0x52c59014
    {0x10,0x17,0x00,0x10,0x17,0x00,0x01,0x11,0x01,0x00,0x8a,0xeb,0xc3}, // dl_c_u4.bin - 0x924d12f2
    {0x81,0x3a,0x41,0x91,0xdb,0x20,0x25,0x0f,0xd8,0x0e,0x00,0x26,0xb8}, // dl_c_u5.bin - 0x6ec2f9c1
    {0}  // end of list
};
static const ROM_REGION s_romRegion_SetC[] PROGMEM = {
    {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nSetC[0].data2n, 0xcebfe26a, "rU1"}, // dl_c_u1.bin
    {NO_BANK_SWITCH, 0x2000, 0x2000, s_romData2nSetC[1].data2n, 0x6751103d, "rU2"}, // dl_c_u2.bin
    {NO_BANK_SWITCH, 0x4000, 0x2000, s_romData2nSetC[2].data2n, 0x52c59014, "rU3"}, // dl_c_u3.bin
    {NO_BANK_SWITCH, 0x6000, 0x2000, s_romData2nSetC[3].data2n, 0x924d12f2, "rU4"}, // dl_c_u4.bin
    {NO_BANK_SWITCH, 0x8000, 0x2000, s_romData2nSetC[4].data2n, 0x6ec2f9c1, "rU5"}, // dl_c_u5.bin
    {0}  // end of list
};

//
// ROMs for version D (LD-V1000 only; dlaird in MAME)
//
static const ROM_DATA2N s_romData2nSetD[] PROGMEM  = {
    // 01   02   04   08   10   20   40   80  100  200  400  800 1000
    {0x03,0x20,0x41,0x91,0xdb,0x20,0x25,0x0f,0x21,0xc9,0xcd,0x26,0x3f}, // dl_d_u1.bin - 0x0b5ab120
    {0x6f,0x26,0xff,0xa0,0x00,0xe0,0x78,0xcd,0x28,0xcb,0x29,0xc9,0xe5}, // dl_d_u2.bin - 0x93ebfffb
    {0x13,0x18,0x0b,0x3b,0x18,0xe0,0x78,0xcd,0x28,0xcb,0x29,0xeb,0x41}, // dl_d_u3.bin - 0x22e6591f
    {0x10,0x17,0x00,0x10,0x17,0x00,0x01,0x11,0x01,0x00,0x8a,0xeb,0xc3}, // dl_d_u4.bin - 0x5f7212cb
    {0x81,0x3a,0x41,0x91,0xdb,0x20,0x25,0x0f,0xd8,0x0e,0x00,0x26,0xb8}, // dl_d_u5.bin - 0x2b469c89
    {0}  // end of list
};
static const ROM_REGION s_romRegion_SetD[] PROGMEM = {
    {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nSetD[0].data2n, 0x0b5ab120, "rU1"}, // dl_d_u1.bin
    {NO_BANK_SWITCH, 0x2000, 0x2000, s_romData2nSetD[1].data2n, 0x93ebfffb, "rU2"}, // dl_d_u2.bin
    {NO_BANK_SWITCH, 0x4000, 0x2000, s_romData2nSetD[2].data2n, 0x22e6591f, "rU3"}, // dl_d_u3.bin
    {NO_BANK_SWITCH, 0x6000, 0x2000, s_romData2nSetD[3].data2n, 0x5f7212cb, "rU4"}, // dl_d_u4.bin
    {NO_BANK_SWITCH, 0x8000, 0x2000, s_romData2nSetD[4].data2n, 0x2b469c89, "rU5"}, // dl_d_u5.bin
    {0}  // end of list
};

//
// ROMs for version E (dlaire in MAME)
//
static const ROM_DATA2N s_romData2nSetE[] PROGMEM  = {
    // 01   02   04   08   10   20   40   80  100  200  400  800 1000
    {0x47,0x11,0xff,0xff,0xff,0xff,0x00,0x7e,0x21,0x32,0x3a,0xff,0xff}, // dl_e_u1.bin - 0x02980426
    {0x21,0x2e,0x2e,0x2e,0x37,0x2e,0x47,0x64,0x24,0xac,0x00,0x09,0x24}, // dl_e_u2.bin - 0x979d4c97
    {0x0a,0x71,0x12,0xff,0x3b,0x07,0x06,0x11,0x48,0x3a,0x3f,0x4c,0x0a}, // dl_e_u3.bin - 0x897bf075
    {0x51,0x00,0x07,0x12,0x54,0x0f,0x11,0xc0,0x03,0x0d,0x8a,0x08,0x0a}, // dl_e_u4.bin - 0x4ebffba5
    {0}  // end of list
};
static const ROM_REGION s_romRegion_SetE[] PROGMEM = {
    {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nSetE[0].data2n, 0x02980426, "rU1"}, // dl_e_u1.bin
    {NO_BANK_SWITCH, 0x2000, 0x2000, s_romData2nSetE[1].data2n, 0x979d4c97, "rU2"}, // dl_e_u2.bin
    {NO_BANK_SWITCH, 0x4000, 0x2000, s_romData2nSetE[2].data2n, 0x897bf075, "rU3"}, // dl_e_u3.bin
    {NO_BANK_SWITCH, 0x6000, 0x2000, s_romData2nSetE[3].data2n, 0x4ebffba5, "rU4"}, // dl_e_u4.bin
    {0}  // end of list
};

//
// ROMs for version F (dlairf in MAME)
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
// ROMs for version F2 (dlair in MAME)
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
// ROMs for Dragons Lair Enhancement v2.1 (unofficial mod, not in MAME)
// http://www.dragons-lair-project.com/tech/enhancements/dle.asp
//
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
CDragonsLairGame::createInstanceSetA(
                                     )
{
    return (new CDragonsLairGame(s_romRegion_SetA));
}

IGame*
CDragonsLairGame::createInstanceSetB(
                                     )
{
    return (new CDragonsLairGame(s_romRegion_SetB));
}

IGame*
CDragonsLairGame::createInstanceSetC(
                                     )
{
    return (new CDragonsLairGame(s_romRegion_SetC));
}

IGame*
CDragonsLairGame::createInstanceSetD(
                                     )
{
    return (new CDragonsLairGame(s_romRegion_SetD));
}

IGame*
CDragonsLairGame::createInstanceSetE(
                                     )
{
    return (new CDragonsLairGame(s_romRegion_SetE));
}

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

