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

#ifndef RESONATOR_H
#define RESONATOR_H


typedef struct _resonator *resonator;


extern resonator resonator_new(float frequency, float alpha, float beta, float gamma, float samplerate);
extern void resonator_free(resonator r);

extern float resonator_get_power(resonator r);
extern float resonator_get_amplitude(resonator r);

extern float resonator_get_alpha(resonator r);
extern void resonator_set_alpha(resonator r, float alpha);

extern float resonator_get_omAlpha(resonator r);

extern float resonator_get_beta(resonator r);
extern void resonator_set_beta(resonator r, float beta);

extern float resonator_get_omBeta(resonator r);

extern float resonator_get_gamma(resonator r);
extern void resonator_set_gamma(resonator r, float gamma);

extern float resonator_get_omGamma(resonator r);

extern float resonator_get_c(resonator r);
extern float resonator_get_s(resonator r);
extern float resonator_get_cc(resonator r);
extern float resonator_get_ss(resonator r);
extern float resonator_get_dpc(resonator r);
extern float resonator_get_dps(resonator r);

extern float resonator_get_phase(resonator r);
extern float resonator_get_phaseX(resonator r);
extern float resonator_get_phaseY(resonator r);

extern float resonator_get_delta_phase(resonator r);
extern float resonator_get_delta_phaseX(resonator r);
extern float resonator_get_delta_phaseY(resonator r);

extern float resonator_get_frequency(resonator r);
extern float resonator_get_instantaneous_frequency(resonator r);

extern void resonator_update_with_sample(resonator r, float sample);
extern void resonator_update(resonator r, float sample);
extern void resonator_update_with_samples(resonator r, const float *samples, int size);
extern void resonator_update_with_samples_stride(resonator r, const float *frame_data,
                                                 int frame_size, int sample_stride);

#endif
