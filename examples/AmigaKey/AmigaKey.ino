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

/* This example will turn an Amiga keyboard into a PC keyboard (just) */

// Connect your Amiga keyboard up with the clock line to A3, data to A4
// and reset to A5.

A500KB kb(A3, A4, A5);

void doReset() {
    Keyboard.release(KEY_LEFT_CTRL);
    Keyboard.release(KEY_LEFT_GUI);
    Keyboard.release(KEY_RIGHT_GUI);
    Keyboard.println("Reset");
}

void setup() {
    Keyboard.begin();
    kb.begin();
    kb.onReset(doReset);
}

void loop() {
    int s = kb.scan();    
    if (s >= 0) {
        uint8_t key = kb.translate(s);
        if (key > 0) {
            if ((s & 0x80) == 0x80) {
                Keyboard.press(key);
                // Caps lock is toggled by the keyboard, so we need to
                // simulate a press and release for it.
                if (key == KEY_CAPS_LOCK) {
                    Keyboard.release(key);
                }
            } else {
                // Caps lock is toggled by the keyboard, so we need to
                // simulate a press and release for it.
                if (key == KEY_CAPS_LOCK) {
                    Keyboard.press(key);
                }
                Keyboard.release(key);
            }
        }
    }
}
