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
   Ported to C by Nicolas Dittlo - peuple.des.limbes@gmail.com
*/

#include <stdlib.h>
#include <math.h>

#include "phasor.h"


struct _phasor
{
    float _omega;

    float _samplerate;

    // phasor
    float _Zc;
    float _Zs;
    float _Wc;
    float _Ws;
    float _Wcps;
};

phasor
phasor_new(float frequency, float samplerate, int angular)
{
    phasor p = malloc(sizeof(struct _phasor));

    p->_omega = angular ? frequency : -M_PI*2.0*frequency/samplerate;
    p->_samplerate = samplerate;
    p->_Zc = 1.0;
    p->_Zs = 0.0;

    phasor_update_multiplier(p);

    return p;
}

void
phasor_free(phasor p)
{
    free(p);
}

void
phasor_update_multiplier(phasor p)
{
    p->_Wc = cos(p->_omega);
    p->_Ws = sin(p->_omega);
    p->_Wcps = p->_Wc + p->_Ws;
}

float
phasor_get_omega(phasor p)
{
    return p->_omega;
}

void
phasor_set_omega(phasor p, float omega)
{
    p->_omega = omega;
    phasor_update_multiplier(p);
}

float
phasor_get_frequency(phasor p)
{
    return -p->_samplerate*p->_omega/(2.0*M_PI);
}

void
phasor_set_frequency(phasor p, float frequency)
{
    p->_omega = -2.0*M_PI*frequency/p->_samplerate;

    phasor_update_multiplier(p);
}

float
phasor_get_samplerate(phasor p)
{
    return p->_samplerate;
}

void
phasor_set_samplerate(phasor p, float samplerate)
{
    p->_samplerate = samplerate;

    phasor_update_multiplier(p);
}

float
phasor_get_Zc(phasor p)
{
    return p->_Zc;
}

float
phasor_get_Zs(phasor p)
{
    return p->_Zs;
}

void
phasor_increment_phase(phasor p)
{
    // complex multiplication with 3 real multiplications
    float ac = p->_Wc * p->_Zc;
    float bd = p->_Ws * p->_Zs;
    float abcd = p->_Wcps * (p->_Zc + p->_Zs);

    p->_Zc = ac - bd;
    p->_Zs = abcd - ac - bd;
}

void
phasor_stabilize(phasor p)
{
    // approximation for 1 / sqrt(x) around 1 (Taylor expansion)
    // sqrt(_Zc*_Zc + _Zs*_Zs) should be 1
    float k = (3.0 - p->_Zc*p->_Zc - p->_Zs*p->_Zs)/2.0;

    p->_Zc *= k;
    p->_Zs *= k;
}
