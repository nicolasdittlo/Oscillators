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
#include <math.h>

#include "phasor.h"

#include "resonator.h"


#define INSTANTANEOUS_FREQUENCY_POWER_THRESHOLD 0.000001


struct _resonator
{
    phasor _phasor;

    float _alpha;
    float _omAlpha;
    float _cos;
    float _sin;

    // smoothed
    float _beta;
    float _omBeta;
    float _cc;
    float _ss;

    // multipled by conjugate, smoothed (delta-phase)
    float _dpc;
    float _dps;
    float _gamma;
    float _omGamma;
};

resonator
resonator_new(float frequency, float alpha, float beta, float gamma, float samplerate)
{
    resonator r = malloc(sizeof(struct _resonator));

    r->_phasor = phasor_new(frequency, samplerate, 0);

    r->_alpha = alpha;
    r->_omAlpha = 1.0 - alpha;
    r->_beta = beta;
    r->_omBeta = 1.0 - beta;
    r->_gamma = gamma;
    r->_omGamma = 1.0 - gamma;
    r->_dpc = 1.0;
    r->_dps = 0.0;

    return r;
}

void
resonator_free(resonator r)
{
    phasor_free(r->_phasor);
    free(r);
}

float
resonator_get_power(resonator r)
{
    return r->_cc*r->_cc + r->_ss*r->_ss;
}

float
resonator_get_amplitude(resonator r)
{
    return sqrt(r->_cc*r->_cc + r->_ss*r->_ss);
}

float
resonator_get_alpha(resonator r)
{
    return r->_alpha;
}

void
resonator_set_alpha(resonator r, float alpha)
{
    if ((alpha < 0.0) || (alpha > 1.0))
        return; //bad alpha passed to resonator_set_alpha()

    r->_alpha = alpha;
    r->_omAlpha = 1.0 - r->_alpha;
}

float
resonator_get_omAlpha(resonator r)
{
    return r->_omAlpha;
}

float
resonator_get_beta(resonator r)
{
    return r->_beta;
}

void
resonator_set_beta(resonator r, float beta)
{
    if ((beta < 0.0) || (beta > 1.0))
        return; //bad beta passed to resonator_set_beta()

    r->_beta = beta;
    r->_omBeta = 1.0 - r->_beta;
}

float
resonator_get_omBeta(resonator r)
{
    return r->_omBeta;
}

float
resonator_get_gamma(resonator r)
{
    return r->_gamma;
}

void
resonator_set_gamma(resonator r, float gamma)
{
    if (gamma < 0.0 || gamma > 1.0)
        return; //bad gamma passed to resonator_set_gamma()

    r->_gamma = gamma;
    r->_omGamma = 1.0 - r->_gamma;
}

float
resonator_get_omGamma(resonator r)
{
    return r->_omGamma;
}

float
resonator_get_c(resonator r)
{
    return r->_cos;
}

float
resonator_get_s(resonator r)
{
    return r->_sin;
}

float
resonator_get_cc(resonator r)
{
    return r->_cc;
}

float
resonator_get_ss(resonator r)
{
    return r->_ss;
}

float
resonator_get_dpc(resonator r)
{
    return r->_dpc;
}

float
resonator_get_dps(resonator r)
{
    return r->_dps;
}

float
resonator_get_phase(resonator r)
{
    return atan2(r->_ss, r->_cc); // returns value in [-pi,pi]
}

float
resonator_get_phaseX(resonator r)
{
    return r->_cc/sqrt(r->_cc*r->_cc + r->_ss*r->_ss);
}

float
resonator_get_phaseY(resonator r)
{
    return r->_ss/sqrt(r->_cc*r->_cc + r->_ss*r->_ss);
}

float
resonator_get_delta_phase(resonator r)
{
    return atan2(r->_dps, r->_dpc); // returns value in [-pi,pi]
}

float
resonator_get_delta_phaseX(resonator r)
{
    return r->_dpc/sqrt(r->_dpc*r->_dpc + r->_dps*r->_dps);
}

float
resonator_get_delta_phaseY(resonator r)
{
    return r->_dps/sqrt(r->_dpc*r->_dpc + r->_dps*r->_dps);
}

float
resonator_get_frequency(resonator r)
{
    return phasor_get_frequency(r->_phasor);
}

float
resonator_get_instantaneous_frequency(resonator r)
{
    float samplerate;

    if (resonator_get_power(r) < INSTANTANEOUS_FREQUENCY_POWER_THRESHOLD)
        return phasor_get_frequency(r->_phasor);

    samplerate = phasor_get_samplerate(r->_phasor);

    return phasor_get_frequency(r->_phasor) + (atan2(r->_dps, r->_dpc)*samplerate)/(2.0*M_PI);
}

void
resonator_update_with_sample(resonator r, float sample)
{
    float lcc;
    float lss;
    float alpha_sample = r->_alpha*sample;
    float Zc = phasor_get_Zc(r->_phasor);
    float Zs = phasor_get_Zs(r->_phasor);

    r->_cos = r->_omAlpha*r->_cos + alpha_sample*Zc;
    r->_sin = r->_omAlpha*r->_sin + alpha_sample*Zs;
    lcc = r->_cc;
    lss = r->_ss;
    r->_cc = r->_omBeta*r->_cc + r->_beta*r->_cos;
    r->_ss = r->_omBeta*r->_ss + r->_beta*r->_sin;

    // compute current * conjugate(previous)
    // the phase time derivative estimate is the arg of this complex number
    // Smoothing (EWMA) with gamma = alpha
    r->_dpc = r->_omAlpha*r->_dpc + r->_alpha*(r->_cc*lcc + r->_ss*lss);
    r->_dps = r->_omAlpha*r->_dps + r->_alpha*(r->_ss*lcc - r->_cc*lss);

    phasor_increment_phase(r->_phasor);
}

void
resonator_update(resonator r, float sample)
{
    resonator_update_with_sample(r, sample);

    phasor_stabilize(r->_phasor); // this is overkill but necessary
}

void
resonator_update_with_samples(resonator r, const float *samples, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        float s = samples[i];

        resonator_update_with_sample(r, s);
    }

    phasor_stabilize(r->_phasor); // this is overkill but necessary
}

void
resonator_update_with_samples_stride(resonator r, const float *frame_data, int frame_size, int sample_stride)
{
    int i;

    for (i = 0; i < frame_size; i += sample_stride)
        resonator_update_with_sample(r, frame_data[i]);

    phasor_stabilize(r->_phasor); // this is overkill but necessary
}
