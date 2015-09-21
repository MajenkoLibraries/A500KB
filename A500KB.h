#ifndef _A500KB_H
#define _A500KB_H

#include <Arduino.h>

class A500KB {
    private:
        uint8_t _clock;
        uint8_t _data;
        uint8_t _reset;
        bool _clockState;
        bool _dataState;
        uint32_t _lastClock;
        uint8_t _val;
        uint8_t _clocks;
        static const uint8_t _keys[128];

        
    public:
        /*! Constructor to make a new A500KB object.  It takes
         *  (and stores for later use) the clock, data and reset
         *  pins the A500 keyboard is connected to.
         */
        A500KB(uint8_t clock, uint8_t data, uint8_t reset) : 
            _clock(clock), 
            _data(data), 
            _reset(reset), 
            _clockState(true),
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
};

#endif
