//
// Copyright (c) 2016, Paul R. Swan
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
#include <CZ80GenericGame.h>
#include <CDragonsLairGame.h>
#include <CSpaceAceGame.h>

//
// The initial selector to select the game to test.
//
static const SELECTOR s_gameSelector[] PROGMEM = {//0123456789abcde
    {"DrgnsLair F    ",  onSelectGame,    (void*) (CDragonsLairGame::createInstanceSetF),     false},
    {"DrgnsLair F2   ",  onSelectGame,    (void*) (CDragonsLairGame::createInstanceSetF2),    false},
    {"DrgnsLair DLE2 ",  onSelectGame,    (void*) (CDragonsLairGame::createInstanceSetDLE21), false},
    {"Space Ace A3   ",  onSelectGame,    (void*) (CSpaceAceGame::createInstanceSetA3),       false},
    {"Space Ace SAE10",  onSelectGame,    (void*) (CSpaceAceGame::createInstanceSetSAE10),    false},
    {"Generic 2716   ",  onSelectGeneric, (void*) (CZ80GenericGame::createInstance2716),      false},
    {"Generic 2732   ",  onSelectGeneric, (void*) (CZ80GenericGame::createInstance2732),      false},
    { 0, 0 } // end of list
};
