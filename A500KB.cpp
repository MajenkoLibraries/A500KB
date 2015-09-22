/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
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

#include <A500KB.h>

void A500KB::begin() {
    pinMode(_clock, INPUT);
    pinMode(_data, INPUT);
    pinMode(_reset, OUTPUT);
    if (_drive != 255) {
        pinMode(_drive, OUTPUT);
    }
    if (_power != 255) {
        pinMode(_power, OUTPUT);
    }

    digitalWrite(_reset, HIGH);
    delay(10);
    digitalWrite(_reset, LOW);
    delay(10);
    digitalWrite(_reset, HIGH);
}

int A500KB::scan() {
    bool cs = digitalRead(_clock);
    bool ds = digitalRead(_data);

    // Start sequence
    if (ds != _dataState && _lastClock == 0) {
        _dataState = ds;

        if (_dataState == HIGH) {
            _lastClock = micros();
            _val = 0;
            _clocks = 0;
            return -1;
        }
    }

    // Byte clocking
    if (cs != _clockState) {
        _clockState = cs;

        if (_clockState == HIGH) { // Rising edge of clock
            _lastClock = micros();
            _val >>= 1;
            _val |= (ds ? 0x80 : 0);
            _clocks++;
            if (_clocks == 8) {
                delayMicroseconds(120);
                pinMode(_data, OUTPUT);
                digitalWrite(_data, LOW);
                delayMicroseconds(170);
                pinMode(_data, INPUT);
                _lastClock = 0;
                return _val;
            }
        }
        
    } else if (_lastClock > 0) {
        if (micros() - _lastClock > 300) { // Sync pulse
            pinMode(_data, OUTPUT);
            digitalWrite(_data, LOW);
            delayMicroseconds(20);
            pinMode(_data, INPUT);
            _lastClock = 0;
        }
    }
    
    return -1;
}

uint8_t A500KB::translate(int scancode) {
    if (scancode < 0) {
        return 0;
    }
    scancode &= 0x7F;
    return _keys[scancode];
}

void A500KB::power(uint8_t state) {
    if (_power != 255) {
        digitalWrite(_power, state);
    }
}

void A500KB::drive(uint8_t state) {
    if (_drive != 255) {
        digitalWrite(_drive, state);
    }
}


const uint8_t A500KB::_keys[128] = {
    0,                  //  0,
    '9',                //  1,      // KP 9
    0,                  //  2,      // Help
    '3',                //  3,      // KP 3
    0,                  //  4,
    '6',                //  5,      // KP 6
    KEY_LEFT_ARROW,     //  6,      // Left
    '0',                //  7,      // KP 0
    0,                  //  8,
    'm',                //  9,      // M
    KEY_F8,             //  10,     // F8
    'i',                //  11,     // I
    KEY_RIGHT_GUI,      //  12,     // Right Amiga
    'k',                //  13,     // K
    0,                  //  14,
    '7',                //  15,     // 7
    0,                  //  16,
    0,                  //  17,
    ')',                //  18,     // KP )
    ']',                //  19,     // ]
    0,                  //  20,
    0,                  //  21,
    0,                  //  22,
    '-',                //  23,     // Minus
    0,                  //  24,
    'c',                //  25,     // C
    KEY_F4,             //  26,     // F4
    'r',                //  27,     // R
    KEY_LEFT_CTRL,      //  28,     // CTRL
    'f',                //  29,     // F
    KEY_RETURN,         //  30,     // KP Enter
    '3',                //  31,     // 3
    0,                  //  32,
    '7',                //  33,     // KP 7
    '*',                //  34,     // KP *
    '1',                //  35,     // KP 1
    0,                  //  36,
    '4',                //  37,     // KP 4
    KEY_DOWN_ARROW,     //  38,     // Down
    '\\',               //  39,     // Backslash
    0,                  //  40,
    'b',                //  41,     // B
    KEY_F6,             //  42,     // F6
    'y',                //  43,     // Y
    KEY_RIGHT_ALT,      //  44,     // Right Alt
    'h',                //  45,     // H
    KEY_ESC,            //  46,     // Escape
    '5',                //  47,     // 5
    0,                  //  48,
    '.',                //  49,     // Fullstop
    KEY_F10,            //  50,     // F10
    'p',                //  51,     // P
    0,                  //  52,
    ';',                //  53,     // Semicolon
    0,                  //  54,
    '9',                //  55,     // 9
    0,                  //  56,
    'z',                //  57,     // Z
    KEY_F2,             //  58,     // F2
    'w',                //  59,     // W
    KEY_RIGHT_SHIFT,    //  60,     // Right shift
    's',                //  61,     // S
    KEY_BACKSPACE,      //  62,     // Backspace
    '1',                //  63,     // 1
    0,                  //  64,
    '8',                //  65,     // KP 8
    '+',                //  66,     // KP Plus
    '2',                //  67,     // KP 2
    0,                  //  68,
    '5',                //  69,     // KP 5
    KEY_RIGHT_ARROW,    //  70,     // Right
    0,                  //  71,
    0,                  //  72,
    'n',                //  73,     // N
    KEY_F7,             //  74,     // F7
    'u',                //  75,     // U
    KEY_LEFT_GUI,       //  76,     // Left Amiga
    'j',                //  77,     // J
    KEY_DELETE,         //  78,     // Del
    '6',                //  79,     // 6
    0,                  //  80,
    '/',                //  81,     // Slash
    '(',                //  82,     // KP (
    '[',                //  83,     // [
    0,                  //  84,
    '\'',               //  85,     // Apostrophe
    '-',                //  86,     // KP Minus
    '0',                //  87,     // 0
    0,                  //  88,
    'x',                //  89,     // X
    KEY_F3,             //  90,     // F3
    'e',                //  91,     // E
    KEY_CAPS_LOCK,      //  92,     // Caps
    'd',                //  93,     // D
    KEY_TAB,            //  94,     // Tab
    '2',                //  95,     // 2
    0,                  //  96,
    '.',                //  97,     // KP Dot
    '/',                //  98,     // KP Slash
    0,                  //  99,
    0,                  //  100,
    0,                  //  101,
    KEY_UP_ARROW,       //  102,    // Up
    '=',                //  103,    // Equals
    0,                  //  104,
    'v',                //  105,    // V
    KEY_F5,             //  106,    // F5
    't',                //  107,    // T
    KEY_LEFT_ALT,       //  108,    // Left Alt
    'g',                //  109,    // G
    KEY_RETURN,         //  110,    // Return
    '4',                //  111,    // 4
    0,                  //  112,
    ',',                //  113,    // Comma
    KEY_F9,             //  114,    // F9
    'o',                //  115,    // O
    0,                  //  116,
    'l',                //  117,    // L
    0,                  //  118,
    '8',                //  119,    // 8
    0,                  //  120,
    0,                  //  121,
    KEY_F1,             //  122,    // F1
    'q',                //  123,    // Q
    KEY_LEFT_SHIFT,     //  124,    // Left Shift
    'a',                //  125,    // A
    ' ',                //  126,    // Space
    '`'                 //  127     // Backtick
};

