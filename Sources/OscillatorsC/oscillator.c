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

#include <stdlib.h>

#include "phasor.h"

#include "oscillator.h"


struct _oscillator
{
    phasor _phasor;

    float _amplitude;
};

oscillator
oscillator_new(float frequency, float samplerate, float amplitude, int angular)
{
    oscillator o = malloc(sizeof(struct _oscillator));

    o->_phasor = phasor_new(frequency, samplerate, angular);
    o->_amplitude = amplitude;

    return o;
}

void
oscillator_free(oscillator o)
{
    phasor_free(o->_phasor);
    free(o);
}

float
oscillator_get_amplitude(oscillator o)
{
    return o->_amplitude;
}

void
oscillator_set_amplitude(oscillator o, float amplitude)
{
    o->_amplitude = amplitude;
}

float
oscillator_get_power(oscillator o)
{
    return o->_amplitude*o->_amplitude;
}

float
oscillator_get_sample(oscillator o)
{
    return o->_amplitude*oscillator_get_Zc(o);
}

// inherited from phasor

float
oscillator_get_omega(oscillator o)
{
    return phasor_get_omega(o->_phasor);
}

void
oscillator_set_omega(oscillator o, float omega)
{
    phasor_set_omega(o->_phasor, omega);
}

float
oscillator_get_frequency(oscillator o)
{
    return phasor_get_frequency(o->_phasor);
}

void
oscillator_set_frequency(oscillator o, float frequency)
{
    phasor_set_frequency(o->_phasor, frequency);
}

float
oscillator_get_samplerate(oscillator o)
{
    return phasor_get_samplerate(o->_phasor);
}

void
oscillator_set_samplerate(oscillator o, float samplerate)
{
    phasor_set_samplerate(o->_phasor, samplerate);
}

float
oscillator_get_Zc(oscillator o)
{
    return phasor_get_Zc(o->_phasor);
}

float
oscillator_get_Zs(oscillator o)
{
    return phasor_get_Zs(o->_phasor);
}

void
oscillator_increment_phase(oscillator o)
{
    phasor_increment_phase(o->_phasor);
}

void
oscillator_stabilize(oscillator o)
{
    phasor_stabilize(o->_phasor);
}
