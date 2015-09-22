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
