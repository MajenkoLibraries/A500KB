/*
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _A500KB_H
#define _A500KB_H

#include <Arduino.h>

// Amiga keyboard scancodes

#define AK_KP0          7
#define AK_KP1          35
#define AK_KP2          67
#define AK_KP3          3
#define AK_KP4          37
#define AK_KP5          69
#define AK_KP6          5
#define AK_KP7          33
#define AK_KP8          65
#define AK_KP9          1
#define AK_KP_OPENPAREN 82
#define AK_KP_CLOSEPAREN 16
#define AK_KP_SLASH     98
#define AK_KP_ASTERISK  34
#define AK_KP_MINUS     86
#define AK_KP_PLUS      66
#define AK_KP_FULLSTOP  97
#define AK_KP_ENTER     30

#define AK_UP           102
#define AK_DOWN         38
#define AK_LEFT         6
#define AK_RIGHT        70

#define AK_HELP         2
#define AK_ESC          46
#define AK_BACKSPACE    62
#define AK_DEL          78
#define AK_TAB          94

#define AK_CTRL         28
#define AK_LSHIFT       124
#define AK_LALT         108
#define AK_LAMIGA       76
#define AK_RSHIFT       60
#define AK_RALT         44
#define AK_RAMIGA       12
#define AK_CAPSLOCK     92

#define AK_F1           122
#define AK_F2           58
#define AK_F3           90
#define AK_F4           26
#define AK_F5           106
#define AK_F6           42
#define AK_F7           74
#define AK_F8           10
#define AK_F9           114
#define AK_F10          50

#define AK_0            87
#define AK_1            63
#define AK_2            95
#define AK_3            31
#define AK_4            111
#define AK_5            47
#define AK_6            79
#define AK_7            15
#define AK_8            119
#define AK_9            55

#define AK_A            125
#define AK_B            41
#define AK_C            25
#define AK_D            93
#define AK_E            91
#define AK_F            29
#define AK_G            109
#define AK_H            45
#define AK_I            11
#define AK_J            77
#define AK_K            13
#define AK_L            117
#define AK_M            9
#define AK_N            73
#define AK_O            115
#define AK_P            51
#define AK_Q            123
#define AK_R            27
#define AK_S            61
#define AK_T            107
#define AK_U            75
#define AK_V            105
#define AK_W            59
#define AK_X            89
#define AK_Y            43
#define AK_Z            57

#define AK_OPENBRACKET  83
#define AK_CLOSEBRACKET 19
#define AK_MINUS        23
#define AK_BACKSLASH    39
#define AK_FULLSTOP     49
#define AK_SEMICOLON    53
#define AK_SLASH        81
#define AK_APOSTROPHE   85
#define AK_ACCENT       127
#define AK_COMMA        113
#define AK_EQUALS       103

#define AK_SPACE        126
#define AK_RETURN       110

class A500KB {
    private:
        uint8_t _clock;
        uint8_t _data;
        uint8_t _reset;
        uint8_t _drive; 
        uint8_t _power;
        bool _clockState;
        bool _dataState;
        bool _resetState;
        uint32_t _lastClock;
        uint8_t _val;
        uint8_t _clocks;
        static const uint8_t _keys[128];
        void (*_onReset)();
        
    public:
        /*! Constructor to make a new A500KB object.  It takes
         *  (and stores for later use) the clock, data and reset
         *  pins the A500 keyboard is connected to.  Can also take
         *  two additional pins to control the two LEDs on the keyboard.
         *
         *  Note: controlling the LEDs requires a 5V logic level output,
         *  so will only work with a 5V Arduino or with logic level shifting
         *  added to your system.
         */
        A500KB(uint8_t clock, uint8_t data, uint8_t reset, uint8_t drive = 255, uint8_t power = 255) : 
            _clock(clock), 
            _data(data), 
            _reset(reset), 
            _drive(drive),
            _power(power),
            _clockState(true),
            _dataState(true),
            _resetState(true),
            _lastClock(0)
        {}

        /*! Initialize the keyboard system.  Basically this sets
         *  the IO pins to the right direction.
         */
        void begin();

        /*! Scan for an incoming keypress.  Must be called as regularly
         *  as possible.  It's (mostly) non-blocking and returns -1 if
         *  there is no keypress available.  A keypress is an 8-bit
         *  value with the most significant bit set for a press and
         *  cleared for a release.
         */
        int scan();
        
        /*! Translate a keypress code into a character or keycode suitable
         *  for Keyboard.press() or Keyboard.release().
         */
        uint8_t translate(int scancode);

        /*! Control the POWER led.  Takes a HIGH or LOW for ON or OFF.
         */
        void power(uint8_t state);

        /*! Control the DRIVE led.  Takes a HIGH or LOW for ON or OFF.
         */
        void drive(uint8_t state);

        /*! Register a reset event callback.  Prototype is:
         *
         *      void myResetCallback();
         *
         *  Note that the keypress events for the individual CTRL-A-A keys
         *  will have been sent, and you may not get the release events
         *  so you will have to assume that the release events will arrive
         *  and act as if they had already arrived.
         */
         void onReset(void (*func)());
};

#endif
