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
#include "CDragonsLairBaseGame.h"
#include "CZ80ACpu.h"
#include "CAY38910.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   Not needed
//
// Board Designations:
//   'r' - ROM board.
//   ...everything else main board.
//
// Z80 Compatibility Notes
//   * IO space is not used.
//   * /M1 (1st machine cycle) is only used in combination with /IORQ to clear interrupt
//   * /BUSRQ (bus request) is not used
//   * /NMI is not used
//   * /INT fires every 33 ms
//   * Manual says that /WAIT adds one wait state (250ns) when accessing the sound chip
//     and "sometimes for other reasons". I think it happens any time memory-mapped I/O
//     is read or written, which is at 0xC000 and above. (MREQ + A14 + A15)
//
//   ICT MEMORY ACCESS/TIMING TBD (probably not critical)

/*
 Z80A CPU, 4 MHz
 MC68705 MCU, 4 MHz (Rev A only)
 AY-3-8910 sound (2 MHz)
 
 MEMORY MAP:
 
 0x0000-0x9FFF  ROM (rU1-rU5, 2764 EPROMs, 8kx8)
 0xA000-0xA7FF  RAM (U3, AM9128 statc RAM, 2kx8)
 0xC000         read AY-3-8910 data
 0xC008         read CP A
 D0 UP
 D1 DOWN
 D2 LEFT
 D3 RIGHT
 D4 ACTION
 D5 AUX1 (Space Ace skill select?)
 D6 AUX2 (Space Ace skill select?)
 D7 AUX3 (Space Ace skill select?)
 0xC010         read CP B / system
 D0 P1 START
 D1 P2 START
 D2 COIN1
 D3 COIN2
 D4 AUX4 (unused?)
 D5 AUX5 (unused?)
 D6 FAN (unused?)
 D7 laserdisc READY
 0xC020         read laserdisc data
 0xE000         write AY-3-8910 data
 0xE008         write misc I/O (active low)
 D0 coin counter
 D1 enable output mode on LD data bus
 D2 LD /ENTER (PR-7820 only)
 D3 LD /EXT (external control enable, PR-7820 only)
 D4 unused ROM board I/O pin 8
 D5 unused ROM board I/O pin 7
 D6 unused ROM board I/O pin 6
 D7 unused ROM board I/O pin 5
 0xE010         write AY-3-8910 address (register selection)
 0xE020         write laserdisc data to latch U21 (does not enable output)
 0xE030-0xE037  write scoreboard player 1 score/lives (enables DEN1; A0-A3 selects digit; D0-D3 sets character)
 0xE038-0xE03F  write scoreboard player 2 score/lives (enables DEN2; A0-A3 selects digit; D0-D3 sets character)
 
 The MC68705 MCU generates an /INT for the Z80 every 33ms. It looks like it was also originally intended to
 be a security device, since there is support logic that would allow it to take over the bus. However, this
 wasn't actually implemented; the Z80 /BUSRQ input and the bus switching logic are hard-wired to be disabled,
 instead of being controlled by the MCU. Later revisions removed the MCU and most of the support logic, and
 instead generate the /INT with a simple counter, re-purposing two 74LS393s retained from the previous design.
 
 For MCU/counter testing purposes, all we need is:
 - a bus check to make sure there are no address/data/control lines being driven by the unused support logic on older boards
 - an interrupt test to verify that the /INT line fires every ~33ms
 
 Things we would like to test:
 Bus Check:
 Address and data lines not stuck/driven (A0-A15, D0-D7)
 Control lines not stuck/driven (/RD, /WR, /MREQ, /RFSH, /M1, /IORQ, /BUSAK)
 /RESET high
 /NMI high
 /BUSRQ high
 /INT mostly high
 /CLK toggling
 /WAIT ???
 Standard ICT tests:
 ROM
 RAM
 Player joystick/button/start inputs
 Skill level switch inputs (Space Ace only)
 Coin switch inputs
 Coin counter outputs
 Custom Tests:
 /INT every ~33ms (set /IORQ + /M1 active to clear /INT)
 Sound chip
 Register read/write test
 Individual channel tone/noise Sound outputs
 replicated DL beep/boop/chime sounds, if possible
 Read I/O ports (DIP switches)
 Scoreboard output
 Skill level switch LED outputs (via scoreboard interface, Space Ace only)
 LD-V1000 control signals (COMMAND, STATUS)
 LD-V1000 data (input and output)
 PR-7820 control signals (ENTER, READY)
 PR-7820 data (output only?)
 '   Laserdisc commands (play/stop/search/still)
 Laserdisc loopback to aux ICT pins?
 ROM version detection (read CRC32 of one or more ROMs, compare to known values)
 Laserdisc Player type detection/validation (through some combination of control signals, ROM version and/or DIP switches)
 
 */

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                        "012", "012345"
    {NO_BANK_SWITCH, 0xA000L, 0xA7FFL, 1, 0xFF, " U3", "Prog. "}, // Program RAM U3, AM9128 2048x8 static RAM
    {0} // end of list
};

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                        "012", "012345"
    {NO_BANK_SWITCH, 0xA000L, 0xA7FFL, 1, 0xFF, " U3", "Prog. "}, // Program RAM U3
    {0} // end of list
};

//
// No write-only RAM on this platform. Yay!
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                               "012", "012345"
    {NO_BANK_SWITCH, 0xC008, 0xFF,  " U8", "IN CPA"}, // Read Inputs (CP A)
    {NO_BANK_SWITCH, 0x6010, 0xFF,  "U14", "IN CPB"}, // Read Inputs (CP B)
    {NO_BANK_SWITCH, 0xC008, 0x01,  " U8", "Up    "}, // Joystick UP
    {NO_BANK_SWITCH, 0xC008, 0x02,  " U8", "Down  "}, // Joystick DOWN
    {NO_BANK_SWITCH, 0xC008, 0x04,  " U8", "Left  "}, // Joystick LEFT
    {NO_BANK_SWITCH, 0xC008, 0x08,  " U8", "Right "}, // Joystick RIGHT
    {NO_BANK_SWITCH, 0xC008, 0x10,  " U8", "Action"}, // Sword/Energize
    {NO_BANK_SWITCH, 0xC008, 0x20,  " U8", "Skill1"}, // AUX1 - Skill 1
    {NO_BANK_SWITCH, 0xC008, 0x40,  " U8", "Skill2"}, // AUX2 - Skill 2
    {NO_BANK_SWITCH, 0xC008, 0x80,  " U8", "Skill3"}, // AUX2 - Skill 2
    {NO_BANK_SWITCH, 0xC008, 0x01,  "U14", "Start1"}, // 1 Player Start
    {NO_BANK_SWITCH, 0xC008, 0x02,  "U14", "Start2"}, // 2 Player Start
    {NO_BANK_SWITCH, 0xC008, 0x04,  "U14", "Coin1 "}, // Left Coin
    {NO_BANK_SWITCH, 0xC008, 0x08,  "U14", "Coin2 "}, // Right Coin
    {NO_BANK_SWITCH, 0xC008, 0x10,  "U14", "AUX4  "}, // AUX4 (unused)
    {NO_BANK_SWITCH, 0xC008, 0x20,  "U14", "AUX5  "}, // AUX5 (unused)
    {NO_BANK_SWITCH, 0xC008, 0x40,  "U14", "FAN   "}, // FAN (unused)
    {NO_BANK_SWITCH, 0xC008, 0x80,  "U14", "LD-RDY"}, // Laserdisc READY
    {NO_BANK_SWITCH, 0xC020, 0xFF,  "U20", "LD-DAT"}, // Laserdisc Data Bus
    {0} // end of list
};

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                    "012", "012345"
    {NO_BANK_SWITCH, 0xE020, 0x02, 0xFF,  "LD ", "LD-Ply"}, // Latch LD PLAY command (0xFD)
    {NO_BANK_SWITCH, 0xE020, 0x04, 0xFF,  "LD ", "LD-Stp"}, // Latch LD STOP command (0xFB)
    {NO_BANK_SWITCH, 0xE008, 0x0D, 0xFF,  "LD ", "LD-Wrt"}, // 0x02 data bus + 0x0 8 /EXT + 0x04 /ENTER
    {NO_BANK_SWITCH, 0xE008, 0x01, 0xFF,  "U16", "CoinCt"}, // Coin Counter
    {NO_BANK_SWITCH, 0xE030, 0x07, 0x0F,  "J3 ", "Score1"}, // Set first digit of P1 score to "8" (0x08 = "8", 0x0F = Blank)
    {NO_BANK_SWITCH, 0xE038, 0x07, 0x0F,  "J3 ", "Score2"}, // Set first digit of P2 score to "8"
    {0} // end of list
};

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                       "0123456789"
    {CDragonsLairBaseGame::clearVideo,       "Clear Vid."},
    {CDragonsLairBaseGame::shellMissileTest, "Shell Mis."},
    {NO_CUSTOM_FUNCTION} // end of list
};


CDragonsLairBaseGame::CDragonsLairBaseGame(
                                           const ROM_REGION    *romRegion
                                           ) : CGame( romRegion,
s_ramRegion,
                                                     s_ramRegionByteOnly,
                                                     s_ramRegionWriteOnly,
                                                     s_inputRegion,
                                                     s_outputRegion,
                                                     s_customFunction)
{
    m_cpu = new CZ80ACpu();
    m_cpu->idle();
    
    // 30 Hz interrupt on the INT pin.
    m_interrupt = ICpu::IRQ0;
    
    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;
    
}


CDragonsLairBaseGame::~CDragonsLairBaseGame(
                                            )
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


// This is a specific implementation for Galaxian that uses an externally
// maskable/resetable latch for the VBLANK interrupt on the NMI line.
PERROR
CDragonsLairBaseGame::interruptCheck(
                                     )
{
    PERROR error = errorSuccess;
    
    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "OK:";
    
    for (int i = 0 ; i < 4 ; i++)
    {
        // Unmask the interrupt.
        m_cpu->memoryWrite(0x7001L, 0x01);
        
        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        1000);
        if (FAILED(error))
        {
            break;
        }
        
        // Mask the interrupt (also resets the latch)
        m_cpu->memoryWrite(0x7001L, 0x00);
        
        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        0);
        if (SUCCESS(error))
        {
            error = errorUnexpected;
            break;
        }
        else
        {
            error = errorSuccess;
        }
        
        // Unmask the interrupt. Irq should have been cleared.
        m_cpu->memoryWrite(0x7001L, 0x01);
        
        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        0);
        if (SUCCESS(error))
        {
            error = errorUnexpected;
            break;
        }
        else
        {
            error = errorSuccess;
        }
        
        // Mask the interrupt (also resets the latch)
        m_cpu->memoryWrite(0x7001L, 0x00);
        
        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        1000);
        if (SUCCESS(error))
        {
            error = errorUnexpected;
            break;
        }
        else
        {
            error = errorSuccess;
        }
    }
    
    return error;
}


//
// Clear the video display to blank (background & sprites)
//
PERROR
CDragonsLairBaseGame::clearVideo(
                                 void *context
                                 )
{
    CDragonsLairBaseGame *thisGame = (CDragonsLairBaseGame *) context;
    ICpu *cpu = thisGame->m_cpu;
    PERROR error = errorSuccess;
    UINT32 count;
    
    static const UINT32 objRamStart  = 0x5800;
    static const UINT32 objRamLen    = 0x100;
    static const UINT8  objBlankChar = (0x160 >> 5); // 16x16
    static const UINT32 bkVRamStart  = 0x105000;
    static const UINT32 bkVRamLen    = 0x400;
    static const UINT8  bkVBlankChar = (0x160 >> 3); // 8x8
    
    //
    // Clear the background RAM to a blank character.
    //
    for (count = 0 ; count < bkVRamLen ; count++)
    {
        (void) cpu->memoryWrite(bkVRamStart + count, bkVBlankChar);
    }
    
    //
    // Clear the object RAM to a blank character.
    //
    for (count = 0 ; count < objRamLen ; count++)
    {
        (void) cpu->memoryWrite(objRamStart + count, objBlankChar);
    }
    
    return error;
}

//
// Custom function for testing the shell/missile hardware.
//
PERROR
CDragonsLairBaseGame::shellMissileTest(
                                       void *context
                                       )
{
    CDragonsLairBaseGame *thisGame = (CDragonsLairBaseGame *) context;
    ICpu *cpu = thisGame->m_cpu;
    PERROR error = errorSuccess;
    UINT32 count;
    
    static const UINT32 objRamStart  = 0x5800;
    static const UINT32 intEnable    = 0x7001L;
    
    for (count = 0 ; ((count < 10) && !FAILED(error)) ; count++)
    {
        UINT8 position;
        
        for (position = 0 ; position < 0xE0 ; position++)
        {
            UINT8 projectile;
            
            //
            // Reset and enable the VBLANK interrupt.
            //
            cpu->memoryWrite(intEnable, 0x00);
            cpu->memoryWrite(intEnable, 0x01);
            
            error = cpu->waitForInterrupt(thisGame->m_interrupt, true, 1000);
            if (FAILED(error))
            {
                break;
            }
            
            //
            // Set shell X+Y to be the same to cause a diagonal sweep.
            // This will take 255 VBLANKs and thus a few seconds. The 7
            // shells (white) and 1 missile (yellow) are separated by 1 line
            // to make them easier to see individually.
            //
            // Note that the first 3 white shells match Y-1 and thus on screen
            // there is a diagnonal group of 3, a gap, and then a diagonal
            // group of 4 white shells plus the yellow missile.
            //
            for (projectile = 0 ; projectile < 8 ; projectile++)
            {
                (void) cpu->memoryWrite(objRamStart + 0x60 + 1 + (4 * projectile),
                                        position + (2 * projectile));
                
                (void) cpu->memoryWrite(objRamStart + 0x60 + 3 + (4 * projectile),
                                        position + (2 * projectile));
            }
        }
    }
    
    return error;
}

