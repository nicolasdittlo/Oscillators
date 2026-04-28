/**
MIT License

Copyright (c) 2026 Alexandre R. J. Francois

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/**
   Ported to C by Nicolas Dittlo (2026) - peuple.des.limbes@gmail.com
*/

#ifndef OSCILLATOR_H
#define OSCILLATOR_H


// oscillator module: simple signal generator

typedef struct _oscillator *oscillator;


extern oscillator oscillator_new(float frequency, float samplerate, float amplitude, int angular);
extern void oscillator_free(oscillator o);

extern float oscillator_get_amplitude(oscillator o);
extern void oscillator_set_amplitude(oscillator o, float amplitude);

extern float oscillator_get_power(oscillator o);

extern float oscillator_get_sample(oscillator o);

// inherited from phasor

extern float oscillator_get_omega(oscillator o);
extern void oscillator_set_omega(oscillator o, float omega);

extern float oscillator_get_frequency(oscillator o);
extern void oscillator_set_frequency(oscillator o, float frequency);

extern float oscillator_get_samplerate(oscillator o);
extern void oscillator_set_samplerate(oscillator o, float samplerate);

extern float oscillator_get_Zc(oscillator o);
extern float oscillator_get_Zs(oscillator o);

extern void oscillator_increment_phase(oscillator o);
extern void oscillator_stabilize(oscillator o);

#endif
