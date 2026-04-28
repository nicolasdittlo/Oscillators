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

/**
   Ported to C by Nicolas Dittlo (2026) - peuple.des.limbes@gmail.com
*/

#include <stdlib.h>
#include <math.h>

#include "phasor.h"

#include "tracking_resonator.h"


#define MIN_MAX_POWER 0.001


struct _tracking_resonator
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

    float _natural_frequency;
    float _track_frequency_power_threshold;
};

tracking_resonator
tracking_resonator_new(float natural_frequency,
                       float alpha, float beta, float gamma, float samplerate)
{
    tracking_resonator t = malloc(sizeof(struct _tracking_resonator));

    t->_phasor = phasor_new(natural_frequency, samplerate, 0);

    t->_natural_frequency = natural_frequency;
    t->_alpha = alpha;
    t->_omAlpha = 1.0 - alpha;
    t->_beta = beta;
    t->_omBeta = 1.0 - beta;
    t->_gamma = gamma;
    t->_omGamma = 1.0 - gamma;
    t->_dpc = 1.0;
    t->_dps = 0.0;
    t->_track_frequency_power_threshold = 0.001;

    return t;
}

void
tracking_resonator_free(tracking_resonator t)
{
    free(t->_phasor);
    free(t);
}

float
tracking_resonator_get_frequency(tracking_resonator t)
{
    return phasor_get_frequency(t->_phasor);
}

float
tracking_resonator_get_natural_frequency(tracking_resonator t)
{
    return t->_natural_frequency;
}

void tracking_resonator_set_natural_frequency(tracking_resonator t,
                                              float frequency, float alpha, float beta, float gamma)
{
    t->_natural_frequency = frequency;

    tracking_resonator_set_alpha(t, alpha);
    tracking_resonator_set_beta(t, beta);
    tracking_resonator_set_gamma(t, gamma);
}

float
tracking_resonator_get_resonant_frequency(tracking_resonator t)
{
    return phasor_get_frequency(t->_phasor);
}

float
tracking_resonator_get_power(tracking_resonator t)
{
    return t->_cc*t->_cc + t->_ss*t->_ss;
}

float
tracking_resonator_get_amplitude(tracking_resonator t)
{
    return sqrt(t->_cc*t->_cc + t->_ss*t->_ss);
}

float
tracking_resonator_get_alpha(tracking_resonator t)
{
    return t->_alpha;
}

void
tracking_resonator_set_alpha(tracking_resonator t, float alpha) {
    if ((alpha < 0.0) || (alpha > 1.0))
        return; //bBad alpha passed to tracking_resonator_set_alpha()

    t->_alpha = alpha;
    t->_omAlpha = 1.0 - t->_alpha;
}

float
tracking_resonator_get_omAlpha(tracking_resonator t)
{
    return t->_omAlpha;
}

float
tracking_resonator_get_beta(tracking_resonator t)
{
    return t->_beta;
}

void
tracking_resonator_set_beta(tracking_resonator t, float beta)
{
    if ((beta < 0.0) || (beta > 1.0))
        return; //bad beta passed to tracking_resonator_set_beta()

    t->_beta = beta;
    t->_omBeta = 1.0 - t->_beta;
}

void tracking_resonator_set_gamma(tracking_resonator t, float gamma)
{
    if ((gamma < 0.0) || (gamma > 1.0))
        return; //bad gamma passed to tracking_resonator_set_gamma()

    t->_gamma = gamma;
    t->_omGamma = 1.0 - t->_gamma;
}

float
tracking_resonator_get_omGamma(tracking_resonator t)
{
    return t->_omGamma;
}

float
tracking_resonator_get_c(tracking_resonator t)
{
    return t->_cos;
}

float
tracking_resonator_get_s(tracking_resonator t)
{
    return t->_sin;
}

float
tracking_resonator_get_cc(tracking_resonator t)
{
    return t->_cc;
}

float
tracking_resonator_get_ss(tracking_resonator t)
{
    return t->_ss;
}

float
tracking_resonator_get_dpc(tracking_resonator t)
{
    return t->_dpc;
}

float
tracking_resonator_get_dps(tracking_resonator t)
{
    return t->_dps;
}

float
tracking_resonator_get_phase(tracking_resonator t)
{
    return atan2(t->_ss, t->_cc); // returns value in [-pi,pi]
}

float
tracking_resonator_get_phaseX(tracking_resonator t)
{
    return t->_cc/sqrt(t->_cc*t->_cc + t->_ss*t->_ss);
}

float
tracking_resonator_get_phaseY(tracking_resonator t)
{
    return t->_ss/sqrt(t->_cc*t->_cc + t->_ss*t->_ss);
}

float
tracking_resonator_get_delta_phase(tracking_resonator t)
{
    return atan2(t->_dps, t->_dpc); // returns value in [-pi,pi]
}

float
tracking_resonator_get_delta_phaseX(tracking_resonator t)
{
    return t->_dpc/sqrt(t->_dpc*t->_dpc + t->_dps*t->_dps);
}

float
tracking_resonator_get_delta_phaseY(tracking_resonator t)
{
    return t->_dps/sqrt(t->_dpc*t->_dpc + t->_dps*t->_dps);
}

void
tracking_resonator_update_with_sample(tracking_resonator t, float sample)
{
    float lcc;
    float lss;
    float alpha_sample = t->_alpha*sample;
    float Zc = phasor_get_Zc(t->_phasor);
    float Zs = phasor_get_Zs(t->_phasor);

    t->_cos = t->_omAlpha*t->_cos + alpha_sample*Zc;
    t->_sin = t->_omAlpha*t->_sin + alpha_sample*Zs;

    lcc = t->_cc;
    lss = t->_ss;

    t->_cc = t->_omBeta*t->_cc + t->_beta*t->_cos;
    t->_ss = t->_omBeta*t->_ss + t->_beta*t->_sin;

    // compute current * conjugate(previous)
    // the phase time derivative estimate is the arg of this complex number
    t->_dpc = t->_cc*lcc + t->_ss*lss;
    t->_dps = t->_ss*lcc - t->_cc*lss;

    // Update tracking
    if(tracking_resonator_get_power(t) > t->_track_frequency_power_threshold)
    {
        // go towards instantaneous frequency
        // this is EWMA with parameter gamma
        float omega = phasor_get_omega(t->_phasor);

        phasor_set_omega(t->_phasor, omega - t->_gamma*atan2(t->_dps, t->_dpc));
    }
    else
    {
        // go back to natural frequency
        phasor_set_frequency(t->_phasor, t->_natural_frequency);
    }

    phasor_increment_phase(t->_phasor);
}

void
tracking_resonator_update(tracking_resonator t, float sample, float max_power)
{
    t->_track_frequency_power_threshold = fmax(MIN_MAX_POWER, max_power)/1000.0f;

    tracking_resonator_update_with_sample(t, sample);

    phasor_stabilize(t->_phasor); // this is overkill but necessary
}

void
tracking_resonator_update_with_samples(tracking_resonator t,
                                       const float *samples, int size, float max_power)
{
    int i;
    t->_track_frequency_power_threshold = fmax(MIN_MAX_POWER, max_power)/1000.0f;

    for (i = 0; i < size; i++)
    {
        float s = samples[i];

        tracking_resonator_update_with_sample(t, s);
    }

    phasor_stabilize(t->_phasor); // this is overkill but necessary
}

void
tracking_resonator_update_with_samples_stride(tracking_resonator t, const float *frame_data,
                                              int frame_size, int sample_stride, float max_power)
{
    int i;

    t->_track_frequency_power_threshold = fmax(MIN_MAX_POWER, max_power) / 1000.0f;

    for (i = 0; i < frame_size; i += sample_stride)
        tracking_resonator_update_with_sample(t, frame_data[i]);

    phasor_stabilize(t->_phasor); // this is overkill but necessary
}
