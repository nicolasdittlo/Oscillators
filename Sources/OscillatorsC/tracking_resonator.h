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
   Ported to C by Nicolas Dittlo (2026) peuple.des.limbes@gmail.com
*/

#ifndef TRACKING_RESONATOR_H
#define TRACKING_RESONATOR_H


typedef struct _tracking_resonator *tracking_resonator;


extern tracking_resonator tracking_resonator_new(float natural_frequency,
                                                 float alpha, float beta, float gamma, float samplerate);
extern void tracking_resonator_free(tracking_resonator t);

extern float tracking_resonator_get_frequency(tracking_resonator t);
extern float tracking_resonator_get_natural_frequency(tracking_resonator t);
extern void tracking_resonator_set_natural_frequency(tracking_resonator t,
                                                     float frequency, float alpha, float beta, float gamma);
extern float tracking_resonator_get_resonant_frequency(tracking_resonator t);

extern float tracking_resonator_get_power(tracking_resonator t);
extern float tracking_resonator_get_amplitude(tracking_resonator t);

extern float tracking_resonator_get_alpha(tracking_resonator t);
extern void tracking_resonator_set_alpha(tracking_resonator t, float alpha);

extern float tracking_resonator_get_omAlpha(tracking_resonator t);

extern float tracking_resonator_get_beta(tracking_resonator t);
extern void tracking_resonator_set_beta(tracking_resonator t, float beta);

extern float tracking_resonator_omBeta(tracking_resonator t);

extern float tracking_resonator_gamma(tracking_resonator t);
extern void tracking_resonator_set_gamma(tracking_resonator t, float gamma);

extern float tracking_resonator_get_omGamma(tracking_resonator t);

extern float tracking_resonator_get_c(tracking_resonator t);
extern float tracking_resonator_get_s(tracking_resonator t);
extern float tracking_resonator_get_cc(tracking_resonator t);
extern float tracking_resonator_get_ss(tracking_resonator t);
extern float tracking_resonator_get_dpc(tracking_resonator t);
extern float tracking_resonator_get_dps(tracking_resonator t);

extern float tracking_resonator_get_phase(tracking_resonator t);

extern float tracking_resonator_get_phaseX(tracking_resonator t);
extern float tracking_resonator_get_phaseY(tracking_resonator t);

extern float tracking_resonator_get_delta_phase(tracking_resonator t);
extern float tracking_resonator_get_delta_phaseX(tracking_resonator t);
extern float tracking_resonator_get_delta_phaseY(tracking_resonator t);

extern void tracking_resonator_update_with_sample(tracking_resonator t, float sample);
extern void tracking_resonator_update(tracking_resonator t, float sample, float max_power);
extern void tracking_resonator_update_with_samples(tracking_resonator t,
                                                   const float *samples, int size, float max_power);
extern void tracking_resonator_update_with_samples_stride(tracking_resonator t, const float *frame_data,
                                                          int frame_size, int sample_stride, float max_power);

#endif
