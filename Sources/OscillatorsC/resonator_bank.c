/**
MIT License

Copyright (c) 2022-2025 Alexandre R. J. Francois

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

#include "resonator.h"

#include "resonator_bank.h"


struct _resonator_bank
{
    float _samplerate;

    resonator *_resonators;
    int _num_resonators;
};

resonator_bank
resonator_bank_new(int num_resonators, const float *frequencies,
                   const float *alphas, const float *betas, const float *gammas,
                   float samplerate)
{
    resonator_bank r = malloc(sizeof(struct _resonator_bank));
    int i;

    r->_samplerate = samplerate;
    r->_num_resonators = num_resonators;

    r->_resonators = malloc(r->_num_resonators*sizeof(resonator));
    for (i = 0; i < r->_num_resonators; i++)
        r->_resonators[i] = resonator_new(frequencies[i], alphas[i], betas[i], gammas[i], samplerate);

    return r;
}

void
resonator_bank_free(resonator_bank r)
{
    int i;

    for (i = 0; i < r->_num_resonators; i++)
        resonator_free(r->_resonators[i]);

    free(r->_resonators);
    free(r);
}

float
resonator_bank_get_samplerate(resonator_bank r)
{
    return r->_samplerate;
}

int
resonator_bank_get_num_resonators(resonator_bank r)
{
    return r->_num_resonators;
}

float
resonator_bank_get_frequency_value(resonator_bank r, int index)
{
    if (index >= r->_num_resonators)
        return 0.0; //bad index passed to resonator_bank_get_frequency_value()

    return resonator_get_frequency(r->_resonators[index]);
}

float
resonator_bank_get_alpha_value(resonator_bank r, int index)
{
    if (index >= r->_num_resonators)
        return 0.0; //bad index passed to resonator_bank_get_alpha_value()

    return resonator_get_alpha(r->_resonators[index]);
}

void
resonator_bank_set_all_alphas(resonator_bank r, float alpha)
{
    int i;

    if ((alpha < 0.0) || (alpha > 1.0))
        return; //bad alpha passed to resonator_bank_set_all_alphas()

    for (i = 0; i < r->_num_resonators; i++)
        resonator_set_alpha(r->_resonators[i], alpha);
}

void
resonator_bank_get_powers(resonator_bank r, float *dest, int size)
{
    int num_res = (size < r->_num_resonators) ? size : r->_num_resonators;
    int i;

    for (i = 0; i < num_res; i++)
        dest[i] = resonator_get_power(r->_resonators[i]);
}

void
resonator_bank_get_amplitudes(resonator_bank r, float *dest, int size)
{
    int num_res = (size < r->_num_resonators) ? size : r->_num_resonators;
    int i;

    for (i = 0; i < num_res; i++)
        dest[i] = resonator_get_amplitude(r->_resonators[i]);
}

void
resonator_bank_get_phases(resonator_bank r, float *dest, int size)
{
    int num_res = (size < r->_num_resonators) ? size : r->_num_resonators;
    int i;

    for (i = 0; i < num_res; i++)
        dest[i] = resonator_get_phase(r->_resonators[i]);
}

void
resonator_bank_get_delta_phases(resonator_bank r, float *dest, int size)
{
    int num_res = (size < r->_num_resonators) ? size : r->_num_resonators;
    int i;

    for (i = 0; i < num_res; i++)
        dest[i] = resonator_get_delta_phase(r->_resonators[i]);
}

void
resonator_bank_get_frequencies(resonator_bank r, float *dest, int size)
{
    int num_res = (size < r->_num_resonators) ? size : r->_num_resonators;
    int i;

    for (i = 0; i < num_res; i++)
        dest[i] = resonator_get_frequency(r->_resonators[i]);
}

void
resonator_bank_get_instantaneous_frequencies(resonator_bank r, float *dest, int size)
{
    int num_res = (size < r->_num_resonators) ? size : r->_num_resonators;
    int i;

    for (i = 0; i < num_res; i++)
        dest[i] = resonator_get_instantaneous_frequency(r->_resonators[i]);
}

void
resonator_bank_update(resonator_bank r, float sample)
{
    int i;

    for (i = 0; i < r->_num_resonators; i++)
        resonator_update(r->_resonators[i], sample);
}

void
resonator_bank_update_with_samples(resonator_bank r, const float *samples, int size)
{
    int num_res = (size < r->_num_resonators) ? size : r->_num_resonators;
    int i;

    for (i = 0; i < num_res; i++)
        resonator_update_with_samples(r->_resonators[i], samples, size);
}

void
resonator_bank_update_with_samples_stride(resonator_bank r, const float *frame_data,
                                          int frame_size, int sample_stride)
{
    int i;

    for (i = 0; i < r->_num_resonators; i++)
        resonator_update_with_samples_stride(r->_resonators[i], frame_data, frame_size, sample_stride);
}
