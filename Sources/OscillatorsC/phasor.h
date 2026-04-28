/**
MIT License

Copyright (c) 2022-2026 Alexandre R. J. Francois

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

#ifndef PHASOR_H
#define PHASOR_H


// phasor module: base for individual oscillators

typedef struct _phasor *phasor;


extern phasor phasor_new(float frequency, float samplerate, int angular);
extern void phasor_free(phasor p);

extern void phasor_update_multiplier(phasor p);

extern float phasor_get_omega(phasor p);
extern void phasor_set_omega(phasor p, float omega);

extern float phasor_get_frequency(phasor p);
extern void phasor_set_frequency(phasor p, float frequency);

extern float phasor_get_samplerate(phasor p);
extern void phasor_set_samplerate(phasor p, float samplerate);

extern float phasor_get_Zc(phasor p);
extern float phasor_get_Zs(phasor p);

extern void phasor_increment_phase(phasor p);
extern void phasor_stabilize(phasor p);

#endif
