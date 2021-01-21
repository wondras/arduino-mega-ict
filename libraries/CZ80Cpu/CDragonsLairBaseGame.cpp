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
//   * /WAIT is only used when reading or writing memory-mapped I/O devices (anything
//     at 0xC000 and above). When /MREQ goes low, /WAIT goes low for one clock cycle (250ns).
//     This is needed to access the AY-3-8910, which is clocked at 2 MHz. We'll need to make
//     sure we don't miss this single pulse.
//
// Z80 Cycle Timing
//
// Dragon's Lair needs a wait state for sound I/O, but standard cycle timing with
// 0x10xxxx addresses to trigger IS_WAIT_SPACE behavior doesn't work for it.
// (IS_WAIT_SPACE expects a long WAIT period while video is busy, which Dragon's
// Lair doesn't have, causing it to hang while waiting for WAIT to be low.)
//
// Puckman timing always inserts a wait state, and does not depend on the /WAIT
// line going low. (Some timing tweaks to allow ROM/RAM access with no wait state
// might make the tests more thorough, but they at least pass reliably on good
// hardware at this point.)
//

// TO-DO: current WAIT_FOR_CLK_RISING_EDGE() macro sometimes gets stuck when the CPU
// clock is 4 MHz. May need to tweak this in some way (without breaking anything else.)

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
     Laserdisc commands (play/stop/search/still)
     Laserdisc loopback to aux ICT pins?
     ROM version detection (read CRC32 of one or more ROMs, compare to known values)
     Laserdisc Player type detection/validation (through some combination of control signals, ROM version and/or DIP switches)
     
 */

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = {
    //                                        "012", "012345"
    {NO_BANK_SWITCH, 0xA000, 0xA7FF, 1, 0xFF, " U3", "Prog. "}, // Program RAM U3, AM9128 2048x8 static RAM
    {0} // end of list
};

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = {
    //                                        "012", "012345"
    {NO_BANK_SWITCH, 0xA000, 0xA7FF, 1, 0xFF, " U3", "Prog. "}, // Program RAM U3
    {0} // end of list
};

//
// No write-only RAM on this platform. Yay!
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = {
    //                              "012", "012345"
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
static const OUTPUT_REGION s_outputRegion[] PROGMEM = {
    //                                    "012", "012345"
    {NO_BANK_SWITCH, 0xE020, 0x02, 0xFF,  "LD ", "LD-Ply"}, // Latch LD PLAY command (0xFD)
    {NO_BANK_SWITCH, 0xE020, 0x04, 0xFF,  "LD ", "LD-Stp"}, // Latch LD STOP command (0xFB)
    {NO_BANK_SWITCH, 0xE008, 0x0D, 0xFF,  "LD ", "LD-Wrt"}, // 0x02 data bus + 0x0 8 /EXT + 0x04 /ENTER
    {NO_BANK_SWITCH, 0xE008, 0x01, 0xFF,  "U16", "CoinCt"}, // Coin Counter
    {NO_BANK_SWITCH, 0xE038, 0x07, 0x0F,  "J3 ", "Score1"}, // Set first digit of P1 score to "8" (0x08 = "8", 0x0F = Blank)
    {NO_BANK_SWITCH, 0xE030, 0x07, 0x0F,  "J3 ", "Score2"}, // Set first digit of P2 score to "8"
    {0} // end of list
};

//
// Memory-mapped I/O
//
// AY-3-8910 sound chip
static const UINT32 AY38910_ADDR_REG = 0x00E010;
static const UINT32 AY38910_ADDR_RD  = 0x00C000;
static const UINT32 AY38910_ADDR_WR  = 0x00E000;
// LED scoreboard base address
static const UINT32 SCORE_ADDR_BASE  = 0x00E030;

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {
    //                                       "0123456789"
    {CDragonsLairBaseGame::readSW1,          "Read SW1  "},
    {CDragonsLairBaseGame::readSW2,          "Read SW2  "},
    {CDragonsLairBaseGame::scoreboardTest,   "Scoreboard"},
    {NO_CUSTOM_FUNCTION} // end of list
};


CDragonsLairBaseGame::CDragonsLairBaseGame(
                                           const ROM_DATA2N *romData2n,
                                           const ROM_REGION *romRegion
                                           ) : CGame(romData2n,
                                                     romRegion,
                                                     s_ramRegion,
                                                     s_ramRegionByteOnly,
                                                     s_ramRegionWriteOnly,
                                                     s_inputRegion,
                                                     s_outputRegion,
                                                     s_customFunction)
{
    m_cpu = new CZ80ACpu(
                         0,
                         NO_ADDRESS_REMAP,
                         NULL,
                         NO_DATA_REMAP,
                         NULL,
                         CZ80ACpu::CYCLE_TYPE_PUCKMAN
                         );
    m_cpu->idle();
    
    // 30 Hz interrupt on the INT pin.
    m_interrupt = ICpu::IRQ0;
    
    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;
    
    m_ay = new CAY38910(m_cpu, AY38910_ADDR_REG, AY38910_ADDR_RD, AY38910_ADDR_WR);
}


CDragonsLairBaseGame::~CDragonsLairBaseGame(
                                            )
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
    delete m_ay;
    m_ay = (CAY38910 *) NULL;
}

//
// Custom interrupt test for Dragon's Lair
//
PERROR
CDragonsLairBaseGame::interruptCheck(
                                     )
{
    PERROR error = errorSuccess;
    
    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "OK:";
    UINT16 data16 = 0x0000;
    UINT8 passes = 10;
    unsigned long startTime, duration;
    
    // /INT fires every 33ms (for frame counting), so we are considering 31-35ms to be okay
    // cleared by asserting /M1 and /IORQ together, which is done by calling acknowledgeInterrupt()
    error = m_cpu->acknowledgeInterrupt(&data16);
    if SUCCESS(error) {
        for (int i = 0 ; i < passes ; i++) {
            startTime  = millis();
            error = m_cpu->waitForInterrupt(m_interrupt, true, 1000);
            duration  = millis() - startTime;
            if SUCCESS(error) {
                error = m_cpu->acknowledgeInterrupt(&data16);
            }
            if (FAILED(error) || ((i > 0)  &&  ((duration < 31) || (duration > 35)) )) {
                error = errorCustom;
                error->code = ERROR_FAILED;
                error->description = "E:";
                STRING_UINT32_HEX(error->description, duration);
                break;
            }
        }
    }

    return error;
}

PERROR
CDragonsLairBaseGame::readSwitches(
                                   CAY38910::Port portNum
                                   )
{
        PERROR error = errorSuccess;
        CAY38910 *ay = this->m_ay;
        UINT8 data = 0x00;
        
        error = ay->readPort(portNum, &data);

        if SUCCESS(error)
        {
            error = errorCustom;
            error->code = ERROR_SUCCESS;
            error->description = "OK:";
            STRING_UINT8_HEX(error->description, data);
        }

        return error;
}

//
// Custom function to read DIP switch block 1 (IO port B on AY-3-8910 sound chip)
//
PERROR
CDragonsLairBaseGame::readSW1(
                                 void *context
                                 )
{
    CDragonsLairBaseGame *thisGame = (CDragonsLairBaseGame *) context;
  //  CAY38910::Port port = IOB;
    return thisGame->readSwitches(CAY38910::IOB);
}

//
// Custom function to read DIP switch block 2 (IO port A on AY-3-8910 sound chip)
//
PERROR
CDragonsLairBaseGame::readSW2(
                                 void *context
                                 )
{
    CDragonsLairBaseGame *thisGame = (CDragonsLairBaseGame *) context;
    return thisGame->readSwitches(CAY38910::IOA);
}

//
// Custom function for testing the LED scoreboard
//
PERROR
CDragonsLairBaseGame::scoreboardTest(
                                       void *context
                                       )
{
    CDragonsLairBaseGame *thisGame = (CDragonsLairBaseGame *) context;
    ICpu *cpu = thisGame->m_cpu;
    PERROR error = errorSuccess;

    // cycle all positions together
    for (UINT8 character = 0 ; character < 16 ; character++) {   // 0 1 2 3 4 5 6 7 8 9 - E H L P <blank>
        for (UINT8 position = 0 ; position < 16 ; position++) {  // all score/lives/credit digits
            error = cpu->memoryWrite(SCORE_ADDR_BASE + (UINT32)position, character);
            if (!SUCCESS(error)) {
                break;
            }
        }
        if (!SUCCESS(error)) {
            break;
        }
        delay(500);
    }
    
    // leave all segments on
    if SUCCESS(error) {
        for (UINT8 position = 0 ; position < 16 ; position++) {
            error = cpu->memoryWrite(SCORE_ADDR_BASE + position, 8);
            if FAILED(error) {
                break;
            }
        }
    }

    return error;
}

