/**
MIT License

Copyright (c) 2025-2026 Alexandre R. J. Francois

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

#include <stdlib.h>
#include <math.h>

#include "tracking_resonator.h"

#include "tracking_resonator_bank.h"


struct _tracking_resonator_bank
{
    float _samplerate;

    tracking_resonator *_resonators;
    int _num_resonators;

    // max power accumulation
    float _sigma;
    float _omSigma;
    float _accPower;
};

tracking_resonator_bank
tracking_resonator_bank_new(int num_resonators, const float *natural_frequencies,
                            const float *alphas,
                            const float *betas,
                            const float *gammas,
                            float samplerate)
{
    tracking_resonator_bank t = malloc(sizeof(struct _tracking_resonator_bank));
    int i;

    t->_samplerate = samplerate;
    t->_num_resonators = num_resonators;

    t->_resonators = malloc(t->_num_resonators*sizeof(tracking_resonator));
    for (i = 0; i < t->_num_resonators; i++)
        t->_resonators[i] = tracking_resonator_new(natural_frequencies[i],
                                                   alphas[i], betas[i], gammas[i], samplerate);

    t->_sigma = 1.0;
    t->_omSigma = 0.0;
    t->_accPower = 0.0001;

    return t;
}

void
tracking_resonator_bank_free(tracking_resonator_bank t)
{
    int i;

    for (i = 0; i < t->_num_resonators; i++)
        tracking_resonator_free(t->_resonators[i]);

    free(t->_resonators);
    free(t);
}

float
tracking_resonator_bank_get_sample_rate(tracking_resonator_bank t)
{
    return t->_samplerate;
}

int
tracking_resonator_bank_get_num_resonators(tracking_resonator_bank t)
{
    return t->_num_resonators;
}

void
tracking_resonator_bank_get_frequencies(tracking_resonator_bank r, float *dest, int size)
{
    int num_res = (size < r->_num_resonators) ? size : r->_num_resonators;
    int i;

    for (i = 0; i < num_res; i++)
        dest[i] = tracking_resonator_get_frequency(r->_resonators[i]);
}

void
tracking_resonator_bank_get_natural_frequencies(tracking_resonator_bank t, float *dest, int size)
{
    int num_res = (size < t->_num_resonators) ? size : t->_num_resonators;
    int i;

    for (i = 0; i < num_res; i++)
        dest[i] = tracking_resonator_get_natural_frequency(t->_resonators[i]);
}

void
tracking_resonator_bank_get_resonant_frequencies(tracking_resonator_bank t, float *dest, int size)
{
    int num_res = (size < t->_num_resonators) ? size : t->_num_resonators;
    int i;

    for (i = 0; i < num_res; i++)
        dest[i] = tracking_resonator_get_resonant_frequency(t->_resonators[i]);
}

float
tracking_resonator_bank_get_resonant_frequency_value(tracking_resonator_bank t, int index)
{
    if (index >= t->_num_resonators)
        return 0.0; //bad index passed to tracking_resonator_bank_get_resonant_frequency_value()

    return tracking_resonator_get_frequency(t->_resonators[index]);
}

void
tracking_resonator_bank_get_powers(tracking_resonator_bank t, float *dest, int size)
{
    int num_res = (size < t->_num_resonators) ? size : t->_num_resonators;
    int i;

    for (i = 0; i < num_res; i++)
        dest[i] = tracking_resonator_get_power(t->_resonators[i]);
}

void
tracking_resonator_bank_get_amplitudes(tracking_resonator_bank t, float *dest, int size)
{
    int num_res = (size < t->_num_resonators) ? size : t->_num_resonators;
    int i;

    for (i = 0; i < num_res; i++)
        dest[i] = tracking_resonator_get_amplitude(t->_resonators[i]);
}

void
tracking_resonator_bank_get_phases(tracking_resonator_bank t, float *dest, int size)
{
    int num_res = (size < t->_num_resonators) ? size : t->_num_resonators;
    int i;

    for (i = 0; i < num_res; i++)
        dest[i] = tracking_resonator_get_phase(t->_resonators[i]);
}

void
tracking_resonator_bank_get_delta_phases(tracking_resonator_bank t, float *dest, int size)
{
    int num_res = (size < t->_num_resonators) ? size : t->_num_resonators;
    int i;

    for (i = 0; i < num_res; i++)
        dest[i] = tracking_resonator_get_delta_phase(t->_resonators[i]);
}

float
tracking_resonator_bank_get_accPower(tracking_resonator_bank t)
{
    return t->_accPower;
}

void
tracking_resonator_bank_update(tracking_resonator_bank t, float sample)
{
    // 1. Snapshot the current value
    float lastAccPower = t->_accPower;
    float maxPower = 0.0;
    float nextAccPower;
    int i;

    // 2. Sequential processing loop
    for (i = 0; i < t->_num_resonators; i++)
    {
        float power;

        tracking_resonator_update(t->_resonators[i], sample, t->_accPower);

        power = tracking_resonator_get_power(t->_resonators[i]);

        if(power > maxPower)
            maxPower = power;
    }

    // 3. Calculate the new moving average
    nextAccPower = t->_omSigma*lastAccPower + t->_sigma*maxPower;

    // 4. Store the result back to the variable
    t->_accPower = nextAccPower;
}

void
tracking_resonator_bank_update_with_samples(tracking_resonator_bank t,
                                            const float *samples, int size)
{
    // 1. Snapshot the current value
    float lastAccPower = t->_accPower;
    float maxPower = 0.0;
    float nextAccPower;
    int i;

    // 2. Sequential processing loop
    for (i = 0; i < t->_num_resonators; i++)
    {
        float power;

        tracking_resonator_update_with_samples(t->_resonators[i], samples, size, t->_accPower);

        power = tracking_resonator_get_power(t->_resonators[i]);

        if(power > maxPower)
            maxPower = power;
    }

    // 3. Calculate the new moving average
    nextAccPower = t->_omSigma*lastAccPower + t->_sigma*maxPower;

    // 4. Store the result back to the variable
    t->_accPower = nextAccPower;
}

void
tracking_resonator_bank_update_with_samples_stride(tracking_resonator_bank t,
                                                   const float *frame_data,
                                                   int frame_size,
                                                   int sample_stride)
{
    // 1. Snapshot the current value
    float lastAccPower = t->_accPower;
    float maxPower = 0.0;
    float nextAccPower;
    int i;

    // 2. Sequential processing loop
    for (i = 0; i < t->_num_resonators; i++)
    {
        float power;

        tracking_resonator_update_with_samples_stride(t->_resonators[i],
                                                      frame_data, frame_size, sample_stride, lastAccPower);

        power = tracking_resonator_get_power(t->_resonators[i]);

        if(power > maxPower)
            maxPower = power;
    }

    // 3. Calculate the new moving average
    nextAccPower = t->_omSigma*lastAccPower + t->_sigma*maxPower;

    // 4. Store the result back to the atomic variable
    t->_accPower = nextAccPower;
}

void
tracking_resonator_bank_set_time_constant(tracking_resonator_bank t,
                                          float tau, int frame_length, int sample_stride,
                                          float samplerate)
{
    float frame_duration = (((float)frame_length)/sample_stride)/samplerate;

    t->_sigma = 1.0f - exp(-frame_duration/tau);
    t->_omSigma = 1.0f - t->_sigma;
}
