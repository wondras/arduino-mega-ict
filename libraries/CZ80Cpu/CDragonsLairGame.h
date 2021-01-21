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
#ifndef CDragonsLairGame_h
#define CDragonsLairGame_h

#include "CDragonsLairBaseGame.h"

class CDragonsLairGame : public CDragonsLairBaseGame
{
public:
    
    //
    // Constructors for this game.
    //
    
    static IGame* createInstanceSetA(
                                     );
    
    static IGame* createInstanceSetB(
                                     );
    
    static IGame* createInstanceSetC(
                                     );
    
    static IGame* createInstanceSetD(
                                     );
    
    static IGame* createInstanceSetE(
                                     );
    
    static IGame* createInstanceSetF(
                                     );
    
    static IGame* createInstanceSetF2(
                                      );
    
    static IGame* createInstanceSetDLE21(
                                         );
    
    //
    // IGame Interface - wholly implemented in the Base game.
    //
    
private:
    
    //
    // Different ROM sets supplied.
    //
    CDragonsLairGame(
                     const ROM_DATA2N *romData2n,
                     const ROM_REGION *romRegion
                     );
    
};

#endif


